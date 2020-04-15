#include <cstdio>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdlib>

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


int ProcessFile(const char *fname)
{
   std::string content = ReadFile(fname);

   printf("Processing %s len %d\n", fname, (int) content.length());

   if (content.empty()) return 0;

   auto lastpos = std::string::npos;

   int nremoved = 0;
   bool modified = false;

   while (true) {

      auto pos = content.rfind("#include", lastpos);

      if (pos == std::string::npos) break;

      content.insert(pos,"//");

      WriteFile(fname, content);

      int res = system(exec_cmd);

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

