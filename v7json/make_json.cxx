using namespace ROOT::Experimental;

std::string selectJson;

void do_macro(const char *macroname, const char *jsonname)
{
   if (!selectJson.empty() && (selectJson != jsonname)) return;

   printf("Producing %s ...\n", jsonname);

   gInterpreter->ExecuteMacro(Form("$HOME/git/webgui/tutorials/rcanvas/%s",macroname));

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

   do_macro("raxis.cxx", "raxis.json");
   do_macro("rbox.cxx", "rbox.json");
   do_macro("rframe.cxx", "rframe.json");
   do_macro("rh1_large.cxx", "rh1_large.json");
   do_macro("rh1_twoaxes.cxx", "rh1_twoaxes.json");
   do_macro("rh1.cxx", "rh1.json");
   do_macro("rh2_colz.cxx", "rh2_colz.json");
   do_macro("rh2_large.cxx", "rh2_large.json");
   do_macro("rh2.cxx", "rh2.json");
   do_macro("rh3_large.cxx", "rh3_large.json");
   do_macro("rh3.cxx", "rh3.json");
   do_macro("rlegend.cxx", "rlegend.json");
   do_macro("rline_style.cxx", "rline_style.json");
   do_macro("rline_width.cxx", "rline_width.json");
   do_macro("rline.cxx", "rline.json");
   do_macro("rmarker.cxx", "rmarker.json");
   do_macro("rpad.cxx", "rpad.json");
   do_macro("rpave.cxx", "rpave.json");
   do_macro("rstyle.cxx", "rstyle.json");
   do_macro("rtext.cxx", "rtext.json");
   do_macro("subpads.cxx", "subpads.json");
   do_macro("symlog.cxx", "symlog.json");
   do_macro("tobject.cxx", "tobject.json");
}
