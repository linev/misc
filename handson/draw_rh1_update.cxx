// macro must be here while cling is not capable to load
// library automatically for outlined function see ROOT-10336
R__LOAD_LIBRARY(libROOTHistDraw)

using namespace ROOT::Experimental;

void draw_rh1_update()
{
   // Create the histogram.
   RAxisConfig xaxis(25, 0., 10.);
   auto pHist1 = std::make_shared<RH1D>(xaxis);

   for (int n=0;n<1000;n++) {
      pHist1->Fill(gRandom->Gaus(3.5,1.));
   }

   // Create a canvas to be displayed.
   auto canvas = RCanvas::Create("RH1 drawing");

   canvas->Draw<RFrameTitle>("RH1 drawing");

   canvas->Draw(pHist1);

   canvas->Show();

   // will block until browser window is displayed
   canvas->Update();

    for (int n=0;n<1000;n++) {
      pHist1->Fill(gRandom->Gaus(7.5,1.));
   }

   canvas->Modified();
   canvas->Update();
}
