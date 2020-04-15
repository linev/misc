#include <cstdio>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdlib>
#include <cstring>

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
         objname.resize(objname.length()-3); // for default cxx extension
         objname.append("o");  // replace extension by object file
         exec.insert(p1,objname);
         std::string rmfile = "rm -f ";
         rmfile.append(objname);
         system(rmfile.c_str());
      }

      int res = system(exec.c_str());

      // printf("Exec %s res %d place %s\n", exec.c_str(), res, content.substr(pos, 28).c_str());

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


int main(int argc, const char **argv)
{
   printf("Reduce includes utility v0.1\n");

   if (argc < 3) {
      printf("Too few arguments\n");
      return 1;
   }

   exec_cmd = argv[1];

   printf("Build cmd: %s\n", exec_cmd);

   int sum = 0;

   for (int n=2; n<argc; ++n)
      sum += ProcessFile(argv[n]);

   printf("Totally removed includes %d\n", sum);

   return 0;

}

