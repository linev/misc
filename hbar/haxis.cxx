/// \file
/// \ingroup tutorial_graphics
/// \notebook -js
/// Swap X/Y axes drawing and use to draw TH1 as bar and as markers
/// Option "haxisg;y+" draw histogram axis as for "hbar" plus allow to draw grids plus draw Y labels on other side
/// Option "bar,base0,same" draws histogram as bars with 0 as reference value
/// Option "P,same" draws histogram as markers
/// Macro also shows how frame margins can be configured and poly-line drawing like filled arrow can be
/// placed relative to frame.
///
/// \macro_image (tcanvas_js)
/// \macro_code
///
/// \author Sergey Linev

#include <fstream>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TH1.h"
#include "TPolyLine.h"

void haxis()
{
   std::vector<double> negative = { -5, -7, -3, -1 };
   std::vector<double> positive = { 2, 15, 2, 10 };
   std::vector<std::string> labels = { "Category 1", "Category 2", "Category 3", "Category 4" };

   // position of frame
   double frame_left = 0.15, frame_right = 0.9,
          frame_top = 0.8, frame_bottom = 0.05;

   auto nbins = negative.size();

   auto hmain = new TH1D("hmain", "title", nbins, 0, nbins);
   hmain->SetMarkerSize(2);
   hmain->SetMarkerStyle(33);
   hmain->SetMarkerColor(kBlue);

   auto hpos = new TH1D("hpos", "title", nbins, 0, nbins);
   hpos->SetFillColor(kRed);
   hpos->SetBarOffset(0.2);
   hpos->SetBarWidth(0.6);

   auto hneg = new TH1D("hneg", "title", nbins, 0, nbins);
   hneg->SetFillColor(kGreen);
   hneg->SetBarOffset(0.2);
   hneg->SetBarWidth(0.6);

   double vmin = 0, vmax = 0;

   for (unsigned n = 0; n < nbins; n++) {
      hmain->SetBinContent(n + 1, negative[n] + positive[n]);
      hpos->SetBinContent(n + 1, positive[n]);
      hneg->SetBinContent(n + 1, negative[n]);
      if (negative[n] < vmin) vmin = negative[n];
      if (positive[n] > vmax) vmax = positive[n];
   }

   double scale_min = (vmin - (vmax-vmin) * 0.1),
          scale_max = (vmax + (vmax-vmin) * 0.1),
          frame_0 = (0 - scale_min) / (scale_max - scale_min) * (frame_right - frame_left) + frame_left;

   auto haxis = new TH1D("haxis", "title", nbins, 0, nbins);
   haxis->SetMinimum(scale_min);
   haxis->SetMaximum(scale_max);
   haxis->SetStats(false);
   for (unsigned n = 0; n < nbins; n++)
      haxis->GetXaxis()->SetBinLabel(n + 1, labels[n].c_str());
   haxis->GetXaxis()->SetTickSize(0);
   haxis->GetXaxis()->SetLabelSize(0.07);
   haxis->GetXaxis()->SetLabelOffset(0.02);

   auto c1 = new TCanvas("c1", "title", 1500, 800);
   c1->SetLeftMargin(frame_left);
   c1->SetRightMargin(1 - frame_right);
   c1->SetTopMargin(1 - frame_top);
   c1->SetBottomMargin(frame_bottom);
   c1->SetGridy(1);
   //c1->SetGridx(1);

   c1->Add(haxis, "haxisg;y+"); // swap x/y axis, let draw grids, y on top

   c1->Add(hpos, "bar,base0,same"); // draw as bar, 0 as base, use drawn frame
   c1->Add(hneg, "bar,base0,same"); // draw as bar, 0 as base, use drawn frame
   c1->Add(hmain, "P,same");        // draw as marker, , use drawn frame

   TLatex *title = new TLatex((frame_left + frame_right)*0.5, 0.96, "Example of haxis with overlayed histograms");
   title->SetNDC(true);
   title->SetTextAlign(22);
   title->SetTextSize(0.08);
   title->SetTextColor(kBlue);
   c1->Add(title);

   auto add_arrow = [&](bool left_side, const char *txt) {
      double x1 = left_side ? frame_0 - 0.02 : frame_0 + 0.02,
             x2 = left_side ? frame_left + 0.05 : frame_right - 0.05,
             x3 = left_side ? frame_left + 0.03 : frame_right - 0.03,
             y0 = frame_top + 0.08, wy = 0.02;

      std::vector<double>  xpos = { x1,  x2, x2, x3, x2, x2, x1, x1 };
      std::vector<double>  ypos = { y0 + wy, y0 + wy, y0 + wy*1.5, y0, y0 - wy*1.5, y0 - wy, y0 - wy, y0 + wy };
      TPolyLine *pleft = new TPolyLine(xpos.size(),xpos.data(),ypos.data());
      pleft->SetFillColor(left_side ? kGreen : kRed);
      pleft->SetNDC();
      c1->Add(pleft, "f");

      TLatex *l = new TLatex(left_side ? x1 - 0.03 : x1 + 0.03, y0, txt);
      l->SetNDC(true);
      l->SetTextAlign(left_side ? 32 : 12);
      l->SetTextSize(0.03);
      l->SetTextColor(kWhite);
      c1->Add(l);
   };

   add_arrow(true, "Reduction");

   add_arrow(false, "Increase");
}