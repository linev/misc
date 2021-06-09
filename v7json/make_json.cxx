using namespace ROOT::Experimental;

std::string selectJson;

void do_macro(const char *macroname, const char *jsonname)
{
   if (!selectJson.empty() && (selectJson != jsonname)) return;

   printf("Producing %s ...\n", jsonname);

   gInterpreter->ExecuteMacro(Form("$HOME/git/webgui/tutorials/v7/%s",macroname));

   auto canv = RCanvas::GetCanvases()[0];

   canv->SaveAs(jsonname);

   RCanvas::ReleaseHeldCanvases();

   gSystem->Exec(Form("gzip -f %s", jsonname));
}


void make_json(const char *sel = nullptr)
{
   // call macro in webgui batch mode to avoid real start of web browser
   // like root --web=batch make_json.cxx

   if (sel) selectJson = sel;

   do_macro("box.cxx", "box.json");
   do_macro("draw_axes.cxx", "axes.json");
   do_macro("draw_frame.cxx", "frame.json");
   do_macro("draw_legend.cxx", "legend.json");
   do_macro("draw_pave.cxx", "pave.json");
   do_macro("draw_rh1.cxx", "rh1.json");
   do_macro("draw_rh1_twoaxes.cxx", "rh1_twoaxes.json");
   do_macro("draw_rh2.cxx", "rh2.json");
   do_macro("draw_rh2_colz.cxx", "rh2_colz.json");
   do_macro("draw_rh3.cxx", "rh3.json");
   do_macro("draw_subpads.cxx", "subpads.json");
   do_macro("draw_symlog.cxx", "symlog.json");
   do_macro("draw_text.cxx", "text.json");
   do_macro("draw_v6.cxx", "v6.json");
   do_macro("line.cxx", "line.json");
   do_macro("lineRStyle.cxx", "lineRStyle.json");
   do_macro("lineStyle.cxx", "lineStyle.json");
   do_macro("lineWidth.cxx", "lineWidth.json");
   do_macro("markerStyle.cxx", "markerStyle.json");
   do_macro("pad.cxx", "pad.json");
}
