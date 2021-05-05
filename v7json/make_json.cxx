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
   do_macro("draw_frame.cxx", "frame.json");
   do_macro("draw_legend.cxx", "legend.json");
   do_macro("draw_pave.cxx", "pave.json");
   do_macro("draw_rh1.cxx", "rh1.json");
   do_macro("draw_rh2.cxx", "rh2.json");
   do_macro("draw_rh2_colz.cxx", "rh2_colz.json");
   do_macro("draw_rh3.cxx", "rh3.json");
   do_macro("draw_subpads.cxx", "subpads.json");
   do_macro("draw_text.cxx", "text.json");
   do_macro("draw_v6.cxx", "v6.json");
   do_macro("line.cxx", "line.json");
   do_macro("lineRStyle.cxx", "lineRStyle.json");
   do_macro("lineStyle.cxx", "lineStyle.json");
   do_macro("lineWidth.cxx", "lineWidth.json");
   do_macro("markerStyle.cxx", "markerStyle.json");
   do_macro("pad.cxx", "pad.json");
}
