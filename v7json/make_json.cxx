using namespace ROOT::Experimental;

void do_macro(const char *macroname, const char *jsonname)
{
   printf("Producing %s ...\n", jsonname);

   gInterpreter->ExecuteMacro(Form("$ROOTSYS/tutorials/v7/%s",macroname));

   auto canv = RCanvas::GetCanvases()[0];

   canv->SaveAs(jsonname);

   RCanvas::ReleaseHeldCanvases();

   gSystem->Exec(Form("gzip -f %s", jsonname));
}


void make_json()
{
   // call macro in webgui batch mode to avoid real start of web browser
   // like root --web=batch make_json.cxx

   do_macro("draw_axes.cxx", "axes.json");
   do_macro("draw_rh1.cxx", "draw_rh1.json");
   do_macro("draw_rh2.cxx", "draw_rh2.json");
}
