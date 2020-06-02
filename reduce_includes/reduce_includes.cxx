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

      if (pos > 5) {
         // simple check if include at the begin of the line
         auto p0 = pos - 1;
         while ((p0 > pos - 5) && ((content[p0] == ' ') || (content[p0] == '\t'))) p0--;
         if (content[p0] != '\n') { lastpos = pos + 11; continue; }
      }

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


int CheckRootHeader(const char *fname)
{
   std::string content = ReadFile(fname);

   const char* name1 = strrchr(fname, '/');
   if (!name1)
      name1 = fname;
   else
      name1++;
   const char* name2 = strrchr(fname, '.');
   if (!name2) return 0;
   std::string expected = std::string("ROOT_") + std::string(name1, name2-name1);

   if (content.find("#pragma once") != std::string::npos) return 0;

   auto pos0 = content.find(std::string("#ifndef ") + expected);
   if (pos0 == std::string::npos) {
      printf("%s not found #ifndef %s\n", fname, expected.c_str());
      return 1;
   }

   auto pos1 = content.find(std::string("#define ") + expected);
   if ((pos1 == std::string::npos) || (pos1 < pos0)) {
      printf("%s not found #define %s\n", fname, expected.c_str());
      return 1;
   }

   return 0;
}


int CheckRootSource(const char *fname)
{
   std::string content = ReadFile(fname);

   int res = 0;

   auto pos0 = content.find("TROOT.h");
   if (pos0 != std::string::npos) {
      if ((content.find("ROOT::") == std::string::npos) &&
          (content.find("gROOT") == std::string::npos) &&
          (content.find("TROOT", pos0+6) == std::string::npos)) {
         res = 1;
         printf("%s not used TROOT.h\n", fname);
      }
   }

   if (content.find("TMath.h") != std::string::npos) {
      if (content.find("TMath::") == std::string::npos) {
         res = 1;
         printf("%s not used TMath.h\n", fname);
      }
   }

   if (content.find("TStyle.h") != std::string::npos) {
      if (content.find("gStyle")==std::string::npos) {
         res = 1;
         printf("%s not used TStyle.h\n", fname);
      }
   }

   if (content.find("TEnv.h") != std::string::npos) {
      if (content.find("gEnv")==std::string::npos) {
         res = 1;
         printf("%s not used TEnv.h\n", fname);
      }
   }

   pos0 = content.find("TVirtualX.h");
   if (pos0 != std::string::npos) {
      if ((content.find("gVirtualX", pos0+10)==std::string::npos) && 
          (content.find("TVirtualX", pos0+10)==std::string::npos)) {
         res = 1;
         printf("%s not used TVirtualX.h\n", fname);
      }
   }

   if (content.find("RVersion.h") != std::string::npos) {
      if ((content.find("ROOT_RELEASE")==std::string::npos) && (content.find("ROOT_VERSION")==std::string::npos)) {
         res = 1;
         printf("%s not used RVersion.h\n", fname);
      }
   }

   if (content.find("TVirtualGL.h") != std::string::npos) {
      if (content.find("gGLManager")==std::string::npos) {
         res = 1;
         printf("%s not used TVirtualGL.h\n", fname);
      }
   }

   pos0 = content.find("TVirtualPad.h");
   if (pos0 != std::string::npos) {
      if ((content.find("gPad", pos0+9)==std::string::npos) &&
          (content.find("fPad", pos0+9)==std::string::npos) &&
          (content.find("GetSelectedPad()", pos0+9)==std::string::npos) &&
          (content.find("TVirtualPad", pos0+9)==std::string::npos)) {
         res = 1;
         printf("%s not used TVirtualPad.h\n", fname);
      }
   }

   if (content.find("TGClient.h") != std::string::npos) {
      if ((content.find("gClient")==std::string::npos) && (content.find("fClient")==std::string::npos)) {
         res = 1;
         printf("%s not used TGClient.h\n", fname);
      }
   }

   if ((content.find("TPad.h") != std::string::npos) && (content.find("TCanvas.h") != std::string::npos)) {
      printf("%s both TPad.h and TCanvas.h found\n", fname);
      res = 1;
   }

   pos0 = content.find("TPad.h");
   if (pos0 != std::string::npos) {
      bool has_gpad = (content.find("gPad", pos0+5) != std::string::npos);
      bool has_pad = (content.find("TPad", pos0+5) != std::string::npos);
      if (has_gpad && !has_pad) {
         printf("%s only gPad used with TPad.h, replace by TVirtualPad.h\n", fname);
         res = 1;
      }
      if (!has_gpad && !has_pad) {
         printf("%s not used TPad.h\n", fname);
         res = 1;
      }
   }

   pos0 = content.find("TCanvas.h");
   if (pos0 != std::string::npos) {
      bool has_gpad = (content.find("gPad", pos0+8) != std::string::npos);
      bool has_pad = (content.find("TPad", pos0+8) != std::string::npos);
      bool has_canvas = (content.find("TCanvas", pos0+8) != std::string::npos);
      bool has_get = (content.find("GetCanvas()", pos0+8) != std::string::npos);
      if (has_gpad && !has_pad && !has_canvas && !has_get) {
         printf("%s only gPad used with TCanvas.h, replace by TVirtualPad.h\n", fname);
         res = 1;
      }
      if (has_gpad && has_pad && !has_canvas && !has_get) {
         printf("%s only TPad used with TCanvas.h, replace by TPad.h\n", fname);
         res = 1;
      }
      if (!has_gpad && !has_pad && !has_canvas && !has_get) {
         printf("%s not used TCanvas.h\n", fname);
         res = 1;
      }
   }

   pos0 = content.find("TSystem.h");
   if (pos0 != std::string::npos) {
      bool has_gsys = (content.find("gSystem", pos0+8) != std::string::npos);
      bool has_sys = (content.find("TSystem", pos0+8) != std::string::npos);
      bool has_types = false;

      static const std::vector<std::string> sys_types =
         { "FileStat_t", "UserGroup_t", "SysInfo_t", "CpuInfo_t", "MemInfo_t", "ProcInfo_t",
           "RedirectHandle_t", "TProcessEventTimer", "gProgPath", "gProgName", "gRootDir", "gSystemMutex" };

      if (!has_gsys && !has_sys) {
         for (auto &name : sys_types)
            if (content.find(name, pos0+8) != std::string::npos) {
               has_types = true;
               break;
            }

         printf("%s not used TSystem.h, has types =  %s\n", fname, (has_types ? "true" : "false"));
         res = 1;
      }
   }

   pos0 = content.find("TDirectory.h");
   if (pos0 != std::string::npos) {
      if ((content.find("TDirectory",pos0+10)==std::string::npos) &&
          (content.find("GetDirectory", pos0+7) == std::string::npos) &&
          (content.find("gDirectory",pos0+10)==std::string::npos)) {
         res = 1;
         printf("%s not used TDirectory.h\n", fname);
      }
   }

   pos0 = content.find("TApplication.h");
   if (pos0 != std::string::npos) {
      if ((content.find("TApplication",pos0+12)==std::string::npos) &&
          (content.find("GetApplication()->",pos0+12)==std::string::npos) &&
          (content.find("gApplication",pos0+12)==std::string::npos)) {
         res = 1;
         printf("%s not used TApplication.h\n", fname);
      }
   }

   pos0 = content.find("TRint.h");
   if (pos0 != std::string::npos) {
      if (content.find("TRint", pos0+8) == std::string::npos) {
         printf("%s not used TRint.h\n", fname);
         res = 1;
      }
   }

   pos0 = content.find("TFile.h");
   if ((pos0 != std::string::npos) && (content.find("GetCurrentFile", pos0+7) == std::string::npos) &&
      (content.find("GetFile", pos0+7) == std::string::npos)) {
      bool has_gfile = (content.find("gFile", pos0+7) != std::string::npos);
      bool has_gdir = (content.find("gDirectory", pos0+7) != std::string::npos);
      bool has_file = (content.find("TFile", pos0+7) != std::string::npos);
      if (has_gdir && !has_gfile && !has_file) {
         printf("%s only gDirectory used with TFile.h, replace by TDirectory.h\n", fname);
         res = 1;
      }
      if (!has_gdir && !has_gfile && !has_file) {
         printf("%s not used TFile.h\n", fname);
         res = 1;
      }
   }

   pos0 = content.find("TKey.h");
   if (pos0 != std::string::npos) {
      if ((content.find("TKey",pos0+10)==std::string::npos) &&
          (content.find("GetKey",pos0+10)==std::string::npos)) {
         res = 1;
         printf("%s not used TKey.h\n", fname);
      }
   }


   pos0 = content.find("TTree.h");
   if (pos0 != std::string::npos) {
      bool has_gtree = (content.find("gTree", pos0+7) != std::string::npos);
      bool has_tree = (content.find("TTree", pos0+7) != std::string::npos);
      if (!has_gtree && !has_tree) {
         printf("%s not used TTree.h\n", fname);
         res = 1;
      }
   }

   pos0 = content.find("TColor.h");
   if (pos0 != std::string::npos) {
      if (content.find("TColor", pos0+8) == std::string::npos) {
         printf("%s not used TColor.h\n", fname);
         res = 1;
      }
   }


   pos0 = content.find("Riostream.h");
   if (pos0 != std::string::npos) {
      if ((content.find("ostream", pos0+8) == std::string::npos) &&
          (content.find("fstream", pos0+8) == std::string::npos) &&
          (content.find("std::cout", pos0+8) == std::string::npos) &&
          (content.find("std::cerr", pos0+8) == std::string::npos) &&
          (content.find("std::endl", pos0+8) == std::string::npos)) {
         printf("%s not used Riostream.h\n", fname);
         res = 1;
      }
   } else {
      pos0 = content.find("<iostream>");
      if (pos0 != std::string::npos) {
        if ((content.find("cout", pos0+8) == std::string::npos) &&
            (content.find("istream", pos0+8) == std::string::npos) &&
            (content.find("ostream", pos0+8) == std::string::npos) &&
            (content.find("cin", pos0+8) == std::string::npos) &&
            (content.find("cerr", pos0+8) == std::string::npos) &&
            (content.find("endl", pos0+8) == std::string::npos)) {
              printf("%s not used <iostream>\n", fname);
              res = 1;
           }
     }

      pos0 = content.find("<fstream>");
      if (pos0 != std::string::npos) {
        if ((content.find("fstream", pos0+8) == std::string::npos)) {
          printf("%s not used <fstream>\n", fname);
          res = 1;
        }
      }

      pos0 = content.find("<iomanip>");
      if (pos0 != std::string::npos) {
        if ((content.find("setw", pos0+8) == std::string::npos) && 
            (content.find("setprecision", pos0+8) == std::string::npos) && 
            (content.find("setfill", pos0+8) == std::string::npos) && 
            (content.find("setiosflags", pos0+8) == std::string::npos) && 
            (content.find("setbase", pos0+8) == std::string::npos)) {
          printf("%s not used <iomanip>\n", fname);
          res = 1;
        }
      }
   }

   pos0 = content.find("TClass.h");
   if (pos0 != std::string::npos) {
      if ((content.find("IsA()->", pos0+8) == std::string::npos) &&
          (content.find("Class()->", pos0+8) == std::string::npos) &&
          (content.find("TClass", pos0+8) == std::string::npos)) {
         printf("%s not uses TClass.h\n", fname);
         res = 1;
      }
   }

   pos0 = content.find("TView.h");
   if (pos0 != std::string::npos) {
      if ((content.find("TView", pos0+8) == std::string::npos) &&
          (content.find("GetView()", pos0+8) == std::string::npos)) {
         printf("%s not uses TView.h\n", fname);
         res = 1;
      }
   }


   pos0 = content.find("TRandom.h");
   if (pos0 != std::string::npos) {
      if ((content.find("gRandom", pos0+8) == std::string::npos) &&
          (content.find("TRandom", pos0+8) == std::string::npos)) {
         printf("%s not uses TRandom.h\n", fname);
         res = 1;
      }
   }

   pos0 = content.find("TGMenu.h");
   if (pos0 != std::string::npos) {
      if ((content.find("TGMenuBar", pos0+8) == std::string::npos) &&
          (content.find("TGPopupMenu", pos0+8) == std::string::npos) &&
          (content.find("TGMenuEntry", pos0+8) == std::string::npos) &&
          (content.find("TGMenuTitle", pos0+8) == std::string::npos)) {
         printf("%s not uses TGMenu.h\n", fname);
         res = 1;
      }
   }

   pos0 = content.find("TGSplitter.h");
   if (pos0 != std::string::npos) {
      if ((content.find("TGSplitter", pos0+8) == std::string::npos) &&
          (content.find("TGVSplitter", pos0+8) == std::string::npos) &&
          (content.find("TGHSplitter", pos0+8) == std::string::npos) &&
          (content.find("fSplitter", pos0+8) == std::string::npos)) {
         printf("%s not uses TGSplitter.h\n", fname);
         res = 1;
      }
   }

   pos0 = content.find("TGTab.h");
   if (pos0 != std::string::npos) {
      if ((content.find("TGTab", pos0+8) == std::string::npos) &&
          (content.find("fTab", pos0+8) == std::string::npos)) {
         printf("%s not uses TGTab.h\n", fname);
         res = 1;
      }
   }


   pos0 = content.find("TGDockableFrame.h");
   if (pos0 != std::string::npos) {
      if ((content.find("TGDockableFrame", pos0+8) == std::string::npos) &&
          (content.find("GetToolDock()->", pos0+8) == std::string::npos)) {
         printf("%s not uses TGDockableFrame.h\n", fname);
         res = 1;
      }
   }


   pos0 = content.find("TGDoubleSlider.h");
   if (pos0 != std::string::npos) {
      if ((content.find("TGDoubleSlider", pos0+8) == std::string::npos) &&
          (content.find("TGDoubleVSlider", pos0+8) == std::string::npos) &&
          (content.find("TGDoubleHSlider", pos0+8) == std::string::npos) &&
          (content.find("GetSlider()", pos0+8) == std::string::npos) &&
          (content.find("Slider->", pos0+8) == std::string::npos)) {
         printf("%s not uses TGDoubleSlider.h\n", fname);
         res = 1;
      }
   }

   pos0 = content.find("TGSlider.h");
   if (pos0 != std::string::npos) {
      if ((content.find("TGSlider", pos0+8) == std::string::npos) &&
          (content.find("TGHSlider", pos0+8) == std::string::npos) &&
          (content.find("TGVSlider", pos0+8) == std::string::npos) &&
          (content.find("fSlider", pos0+8) == std::string::npos)) {
         printf("%s not uses TGSlider.h\n", fname);
         res = 1;
      }
   }

   pos0 = content.find("TGButton.h");
   if (pos0 != std::string::npos) {
      if ((content.find("TGButton", pos0+8) == std::string::npos) &&
          (content.find("TGTextButton", pos0+8) == std::string::npos) &&
          (content.find("TGPictureButton", pos0+8) == std::string::npos) &&
          (content.find("TGCheckButton", pos0+8) == std::string::npos) &&
          (content.find("TGRadioButton", pos0+8) == std::string::npos) &&
          (content.find("TGSplitButton", pos0+8) == std::string::npos)) {
         printf("%s not uses TGButton.h\n", fname);
         res = 1;
      }
   }

   pos0 = content.find("TGFSContainer.h");
   if (pos0 != std::string::npos) {
      if ((content.find("TGFileItem", pos0+8) == std::string::npos) &&
          (content.find("TGFileContainer", pos0+8) == std::string::npos)) {
         printf("%s not uses TGFSContainer.h\n", fname);
         res = 1;
      }
   }

   pos0 = content.find("TGTextEditDialogs.h");
   if (pos0 != std::string::npos) {
      if ((content.find("TGSearchDialog", pos0+8) == std::string::npos) &&
          (content.find("TGPrintDialog", pos0+8) == std::string::npos) &&
          (content.find("TGGotoDialog", pos0+8) == std::string::npos)) {
         printf("%s not uses TGTextEditDialogs.h\n", fname);
         res = 1;
      }
   }

   static const std::vector<std::string> test_types = {
      "TObjString", "TTimer", "TUrl", "TBrowser", "TImage",
      "TDatime", "TTimeStamp", "TDate", "TVectorD", "TMatrixD",
      "THStack", "TArc", "TLine", "TText", "TLatex", "TCutG", "TBox", "TGaxis",
      "TPaveText", "TPaveStats", "TGraph", "TMarker", "TPoint", "TEllipse",
      "TPolyLine3D", "TPolyMarker3D", "TLegend", "TProfile",
      "TH1", "TH2", "TH3", "TF1", "TF2", "TF3", "TBuffer", "TMap",
      "TStopwatch", "TExec", "TMemFile", "TNamed", "TObject",

      // gui classes
      "TGCanvas", "TGColorDialog", "TGColorSelect", "TGComboBox",
      "TGFileBrowser", "TGFileDialog", "TGFontDialog",
      "TGFont", "TGFrame", "TGFSComboBox",  "TGIcon",
      "TGInputDialog", "TGLabel", "TGLayout", "TGListBox", "TGListTree", "TGListView",
      "TGMdiDecorFrame", "TGMdiFrame", "TGMdi", "TGMdiMainFrame",
      "TGMdiMenu", "TGMsgBox", "TGNumberEntry", "TGObject", "TGPack", "TGPasswdDialog",
      "TGPicture", "TGProgressBar", "TGScrollBar", "TGSimpleTable", "TGSimpleTableInterface",
      "TGSpeedo", "TGSplitFrame", "TGStatusBar", "TGTableCell", "TGTable",
      "TGTableHeader", "TGTableLayout", "TGTextBuffer", "TGTextEdit", "TGTextEditor",
      "TGTextEntry", "TGText", "TGTextView", "TGToolBar", "TGToolTip", "TGuiBuilder",
      "TGView", "TGXYLayout"
   };

   for(auto &clname : test_types) {
      std::string incname = clname + ".h";
      auto pos = content.find(incname);

      if (pos != std::string::npos) {
         if (content.find(clname, pos+incname.length()) == std::string::npos) {
            printf("%s not used %s\n", fname, incname.c_str());
            res = 1;
         }
      }
   }

   return res;
}

int main(int argc, const char **argv)
{
   printf("Reduce includes utility v0.5\n");

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
   } else if (!strcmp(exec_cmd,"roothdr")) {
      kind = "roothdr";
      for (int n=2; n<argc; ++n)
         sum += CheckRootHeader(argv[n]);
   } else if (!strcmp(exec_cmd,"rootsrc")) {
      kind = "rootsrc";
      for (int n=2; n<argc; ++n)
         sum += CheckRootSource(argv[n]);
   } else {
      for (int n=2; n<argc; ++n)
         sum += ProcessFile(argv[n]);
   }

   printf("Files %d Total %s includes %d\n", argc-2, kind, sum);

   return 0;

}

