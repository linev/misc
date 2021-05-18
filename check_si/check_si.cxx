void TestClass(const std::string &clname)
{
   printf("Checking class %s\n", clname.c_str());
   if ((clname == "TTreeCache") ||  (clname == "TTreeCacheUnzip"))
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
               printf("\n DUPL: class: %s member: %s found again in class %s\n\n", cl->GetName(), elem->GetName(), bcl->GetName());

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

      while ((line.length() > 0) && (line[0] == ' '))
         line.erase(0, 1);

      while ((line.length() > 0) && (line[line.length()-1] == ' '))
         line.resize(line.length()-1);

      if (line.find("-;") == line.length()-2) continue;

      if (line.length() < 3) continue;

      if (line[line.length()-1] == ';')
         line.resize(line.length()-1);

      if (line[line.length()-1] == '+')
         line.resize(line.length()-1);

      TestClass(line);
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
