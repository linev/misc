#include <cstdio>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdlib>
#include <cstring>
#include <streambuf>
#include <vector>
#include <algorithm>
#include <stdio.h>

std::string ReadFile(const char *fname)
{
   std::ifstream t(fname, std::ios::binary);
   std::string str;

   t.seekg(0, std::ios::end);
   str.reserve(t.tellg());
   t.seekg(0, std::ios::beg);

   str.assign((std::istreambuf_iterator<char>(t)),
               std::istreambuf_iterator<char>());

   return str;
}

void WriteFile(const char *fname, const std::string &content)
{
   std::ofstream ostrm(fname, std::ios::binary);
   ostrm.write(content.c_str(), content.length());
}

const char *exec_cmd = nullptr;

const char *go4arg = "$$go4obj$$";
const char *qtarg = "$$qtobj$$";

int ProcessFile(const char *fname)
{
   std::string content = ReadFile(fname);

   printf("Processing %s len %d\n", fname, (int) content.length());

   if (content.length() < 20) return 0;

   auto lastpos = content.length() - 8;

   int nremoved = 0;
   bool modified = false;

   while (true) {

      auto pos = content.rfind("#include", lastpos);

      if (pos == std::string::npos) break;

      content.insert(pos,"//");

      WriteFile(fname, content);

      std::string exec = exec_cmd;
      auto p1 = exec.find(go4arg);
      if (p1 != std::string::npos) {
         exec.erase(p1, strlen(go4arg));
         std::string objname = fname;
         auto dot = objname.rfind(".");
         objname.resize(dot+1);
         objname.append("o");  // replace extension by object file

         exec.insert(p1,objname);
         std::string rmfile = "rm -f ";
         rmfile.append(objname);
         system(rmfile.c_str());
      } else if ((p1 = exec.find(qtarg)) != std::string::npos) {
         exec.erase(p1, strlen(qtarg));

         std::string objname = fname;
         auto dot = objname.rfind(".");
         objname.resize(dot+1);
         objname.append("o");  // replace extension by object file
         auto slash = objname.rfind("/");
         if (slash != std::string::npos)
            objname.erase(0, slash+1);
         objname.insert(0,".obj/");

         exec.insert(p1,objname);
         std::string rmfile = "rm -f ";
         rmfile.append(objname);
         system(rmfile.c_str());
      }

      int res = system(exec.c_str());

      if ((p1 != std::string::npos) && (res==0)) {
         std::string info = content.substr(pos, 50);
         auto newline = info.find("\n");
         if (newline != std::string::npos) info.resize(newline);
         printf("Exec %s res %d place %s\n", exec.c_str(), res, info.c_str());
      }

      if (res == 0) {
         nremoved++;
      } else {
         content.erase(pos,2);
         modified = true;
      }

      if (pos < 8) break; // #include is 8 symbols long
      lastpos = pos-8;
   }

   if (modified)
      WriteFile(fname, content);

   if (nremoved > 0)
      printf("File %s removed %d\n", fname, nremoved);

   return nremoved;
}

bool SameInclude(const std::string &name1, const std::string &name2)
{
   if (name1 == name2) return true;

   if ((name1[0] != 'c') && (name2[0] != 'c')) return false;

   static const std::vector<std::string> vect1 =
      { "cassert",  "cctype",  "cerrno",  "cfenv",  "cfloat",  "inttypes.h",
        "climits", "clocale",  "cmath",  "csetjmp", "csignal", "cstdarg", "cstddef",
        "cstdint", "cstdio",  "cstring", "cstdlib", "ctime", "cuchar", "cwchar", "cwctype" };

   static const std::vector<std::string> vect2 =
      { "assert.h", "ctype.h", "errno.h", "fenv.h", "float.h", "inttypes.h",
        "limits.h", "locale.h", "math.h", "setjmp.h", "signal.h", "stdarg.h", "stddef.h",
        "stdint.h", "stdio.h", "string.h", "stdlib.h", "time.h", "uchar.h", "wchar.h", "wctype.h" };

   std::string n1, n2;
   if (name2[0] == 'c') { n1 = name2; n2 = name1; }
                   else { n1 = name1; n2 = name2; }

   auto pos1 = std::find(std::begin(vect1), std::end(vect1), n1);
   if (pos1 == std::end(vect1)) return false;

   int indx = pos1 - std::begin(vect1);

   return vect2[indx] == n2;
}

int FindDuplicate(const char *fname)
{
   std::string content = ReadFile(fname);

   auto len = content.length();
   if (len < 20) return 0;

   int lastpos = 0;

   std::vector<std::string> includes;

   while (lastpos < len) {

      auto pos = content.find("#include", lastpos);
      if (pos == std::string::npos) break;

      pos += 9;

      while ((pos < len) && (content[pos] == ' ')) pos++;
      if (pos>=len) break;

      char symb;

      while ((pos < len) && (content[pos] == ' ') && (content[pos] != '\"')) pos++;

      if (content[pos] == '<') symb = '>'; else
      if (content[pos] == '\"') symb = '\"'; else { lastpos = pos+1; continue; }

      auto pos2 = pos+1;
      while ((pos2 < len) && (content[pos2] != symb)) pos2++;
      if (pos2>=len) break;

      lastpos = pos2+1;
      if (pos2-pos>100) continue;

      std::string inclname = content.substr(pos+1, pos2-pos-1);

      includes.emplace_back(content.substr(pos+1, pos2-pos-1));
   }

   if (includes.size() < 2) return 0;

   int dupl = 0;

   for (int n1=0;n1<includes.size()-1;++n1) {
      auto &name1 = includes[n1];
      for (int n2=n1+1;n2<includes.size();++n2) {
         auto &name2 = includes[n2];
         if (SameInclude(name1, name2)) {
            printf("File %s has duplicated include %s\n", fname, name1.c_str());
            dupl++;
         }
      }
   }

   return dupl;
}


int main(int argc, const char **argv)
{
   printf("Reduce includes utility v0.3\n");

   if (argc < 3) {
      printf("Too few arguments\n");
      return 1;
   }

   exec_cmd = argv[1];

   printf("Build cmd: %s\n", exec_cmd);

   int sum = 0;

   const char *kind = "removed";

   if (!strcmp(exec_cmd,"duplicate")) {
      kind = "duplicated";
      for (int n=2; n<argc; ++n)
         sum += FindDuplicate(argv[n]);
   } else {
      for (int n=2; n<argc; ++n)
         sum += ProcessFile(argv[n]);
   }

   printf("Files %d Total %s includes %d\n", argc-2, kind, sum);

   return 0;

}

