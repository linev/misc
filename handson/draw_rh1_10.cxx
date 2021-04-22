// macro must be here while cling is not capable to load
// library automatically for outlined function see ROOT-10336
R__LOAD_LIBRARY(libROOTHistDraw)

using namespace ROOT::Experimental;

/** should be outside,  otherwise will be destroyed when leave function scope */
auto style = RStyle::Parse("hist { line_color_name: red; line_width: 5; fill_color_name: green; }");

void draw_rh1_10()
{
   // Create the histogram.
   RAxisConfig xaxis(25, 0., 10.);
   auto pHist1 = std::make_shared<RH1D>(xaxis);

   for (int n=0;n<1000;n++) {
      pHist1->Fill(gRandom->Gaus(3.5,1.));
   }

   // Create a canvas to be displayed.
   auto canvas = RCanvas::Create("RH1 drawing options");

   auto subpads = canvas->Divide(2,5);

   auto lineattr = RAttrLine().SetColor(RColor::kRed).SetWidth(5);
   auto fillattr = RAttrFill().SetColor(RColor::kGreen);

   for (int i=0;i<2;i++)
      for(int j=0;j<5;j++) {
         subpads[i][j]->Draw<RFrameTitle>("RH1 drawing");
         auto hdraw = subpads[i][j]->Draw(pHist1);

         /** one can set attributes directly */
         // hdraw->AttrLine().SetColor(RColor::kRed).SetWidth(5);
         // hdraw->AttrFill().SetColor(RColor::kGreen);

         /** or set attributes objects */
         // hdraw->AttrLine() = lineattr;
         // hdraw->AttrFill() = fillattr;

      }

   /** or assign style which will be applied to all objects */
   canvas->UseStyle(style);

   canvas->SetSize(1000, 800);
   canvas->Show();

}
