// macro must be here while cling is not capable to load
// library automatically for outlined function see ROOT-10336
R__LOAD_LIBRARY(libROOTHistDraw)

using namespace ROOT::Experimental;

void draw_rh1_log2()
{
   // Create the histogram.
   RAxisConfig xaxis(25, 1., 11.);
   auto pHist1 = std::make_shared<RH1D>(xaxis);

   for (int n=0;n<1000;n++) {
      pHist1->Fill(gRandom->Gaus(5.5,1.));
   }

   // Create a canvas to be displayed.
   auto canvas = RCanvas::Create("RH1 drawing with log 2");

   canvas->Draw<RFrameTitle>("RH1 drawing with log 2");

   canvas->Draw(pHist1);

   // frame should exists after histogram is drawn
   canvas->GetFrame()->AttrX().SetLog(2.);

   canvas->Show();
}
