void TestClass(const std::string &clname)
{
   printf("Checking class %s\n", clname.c_str());
   if ((clname == "TTreeCache") ||
       (clname == "TTreeCacheUnzip") ||
       (clname == "TMVA::VarTransformHandler"))
      return; // fails in GetStreamerInfo

   TClass *cl = TClass::GetClass(clname.c_str());
   if (!cl) return;

   auto si = cl->GetStreamerInfo();
   if (!si) return;

   TIter elem_iter(si->GetElements());
   TStreamerElement *elem = nullptr;

   std::vector<TClass *> base_classes;

   while ((elem = (TStreamerElement *) elem_iter()) != nullptr) {
      if (elem->IsBase()) {
         base_classes.emplace_back(elem->GetClassPointer());
      } else {
         for (auto bcl : base_classes) {
            auto offset = bcl->GetDataMemberOffset(elem->GetName());
            if (offset)
               printf("\nDUPL: class: %s member: %s found also in class %s\n\n", cl->GetName(), elem->GetName(), bcl->GetName());

         }
      }
   }

}


void TestLinkDef(const char *fname)
{
   std::ifstream f(fname);
   if (!f) return;

   std::string line;

   std::string search = "#pragma link C++ class";

   while (std::getline(f, line)) {
      if (line.empty()) continue;
      if (line.find(search) != 0) continue;
      line.erase(0, search.length());

      if (line.find("+protected") == 0) line.erase(0, 10); // there are class+protected declaration for TSelector

      auto len = line.length();

      while (len > 0) {
         len--;
         if (line[len] == ' ') line.erase(len, 1);
      }

      len = line.length();
      while (len > 3) {
         len--;
         if ((line[len] == ';') || (line[len] == '+') || (line[len] == '!')) {
            line.resize(len);
            continue;
         }
         break;
      }

      if (line[len] != '-') TestClass(line);
   }
}

void TestAllLinkdef(const char *fname)
{
   std::ifstream f(fname);
   if (!f) return;

   std::string line;

   while (std::getline(f, line)) {
      TestLinkDef(line.c_str());
   }
}


void check_si()
{
   TestClass("TStyle");

   // TestLinkDef("/home/linev/git/root/hist/hist/inc/LinkDef.h");

   TestAllLinkdef("files.txt");
   return;


   TIter iter(gROOT->GetListOfClasses());

   TClass *cl = nullptr;

   while ((cl = (TClass *) iter()) != nullptr) {
      TestClass(cl->GetName());
   }

}
