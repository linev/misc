// macro must be here while cling is not capable to load
// library automatically for outlined function see ROOT-10336
R__LOAD_LIBRARY(libROOTHistDraw)

using namespace ROOT::Experimental;

void draw_rh1_lines()
{
   // Create the histogram.
   RAxisConfig xaxis(25, 0., 10.);
   auto pHist1 = std::make_shared<RH1D>(xaxis);

   for (int n=0;n<1000;n++) {
      pHist1->Fill(gRandom->Gaus(3.5,1.));
   }

   // Create a canvas to be displayed.
   auto canvas = RCanvas::Create("RH1 drawing options");

   canvas->Draw<RFrameTitle>("Default RH1 drawing");

   canvas->Draw(pHist1);

   canvas->Draw<RLine>()->SetP1({0.07_normal, 0.1_normal - 10_px}).SetP2({0.95_normal, 0.1_normal - 10_px}).AttrLine().SetColor(RColor::kRed);
   canvas->Draw<RLine>()->SetP1({0.07_normal, 0.9_normal + 25_px}).SetP2({0.95_normal, 0.9_normal + 25_px}).AttrLine().SetColor(RColor::kBlue);

   canvas->Show();
}
