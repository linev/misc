#include <fstream>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TH1.h"


std::vector<std::string> split_line(const std::string &line)
{
    size_t p = 0;

    std::vector<std::string> res;

    int cnt = 0;

    while (p < line.length() && cnt++ < 100) {
      if (line[p] == ',') {
        res.emplace_back("");
        p++;
        continue;
      }

      if (line[p] == '"') {
         auto p2 = line.find("\"", p+1);
         res.emplace_back(line.substr(p+1, p2 - p - 1));
         p = p2+1;
         if (line[p] == ',') p++;
         continue;
      }

       auto p2 = line.find(",", p+1);
       if (p2 == std::string::npos) {
          res.emplace_back(line.substr(p));
          p = line.length();
       } else {
          res.emplace_back(line.substr(p, p2 - p - 1));
          p = p2 + 1;
       }
    }

    return res;
}

void draw(const char *fname = "test.csv")
{
   std::ifstream f(fname);

   if (!f) {
      printf("fail to open file %s\n", fname);
      return;
   }

   std::vector<double> main, positive, negative;
   std::vector<std::string> main_labels;

   std::vector<double> last_sub;

   std::string line, current_label;

   double current_value = 0., current_sum = 0., current_pos = 0., current_negative = 0.;
   double hmin = 0., hmax = 0.;
   std::vector<double> current_sub;

   auto finish_category = [&]() {
      if (current_sub.size() == 0)
         return;

      printf("   sum: %f diff: %f\n", current_sum, current_sum - current_value);

      main.push_back(current_value);
      main_labels.push_back(current_label);
      positive.push_back(current_pos);
      negative.push_back(current_negative);
      if (current_negative < hmin) hmin = current_negative;
      if (current_pos > hmax) hmax = current_pos;

      if (current_value < hmin) hmin = current_value;
      if (current_value > hmax) hmax = current_value;

      current_value = current_sum = current_pos = current_negative = 0;
      current_sub.clear();
      current_label.clear();

   };


   while (std::getline(f, line)) {

      if (line.empty())
         continue;

      if (line.find(",,") == 0) {
         auto vect = split_line(line);
         if (vect.size() < 4) {
            printf("Format failure in %s\n", line.c_str());
            return;
         }

         if (!vect[0].empty() || !vect[1].empty()) {
            printf("Format failure in %s\n", line.c_str());
            return;
         }

         auto subv = std::stod(vect[3]);

         if (subv > 0) {
            current_pos += subv;
         } else {
            current_negative += subv;

         }
         current_sum += subv;
         current_sub.push_back(subv);

         printf("          %f info:%s\n", subv, vect[2].substr(0,10).c_str());

         continue;
      }

      if (line[0] != ',')
         continue;

      finish_category();

      // start of new category

      // printf("%s\n", line.c_str());
      auto vect = split_line(line);
      // for (unsigned n = 0; n < vect.size(); n++)
      //    printf("   [%u]: %s\n", n, vect[n].c_str());

      if (vect.size() < 4) {
         printf("Format failure in %s\n", line.c_str());
         return;
      }

      current_sum = 0;
      current_value = std::stod(vect[3]);
      current_label = vect[1];
      printf("v = %f  title: %s\n", current_value, vect[1].c_str());
   }

   finish_category();

   double scale = 1e-6,
          scale_min = (hmin - (hmax-hmin) * 0.1) * scale,
          scale_max = (hmax + (hmax-hmin) * 0.1) * scale,
          frame_left = 0.3, frame_right = 0.9,
          frame_top = 0.85, frame_bottom = 0.05,
          frame_0 = (0 - scale_min) / (scale_max - scale_min) * (frame_right - frame_left) + frame_left;

   auto haxis = new TH1D("haxis", "title", main.size(), 0, main.size());
   haxis->SetMinimum(scale_min);
   haxis->SetMaximum(scale_max);
   haxis->SetStats(false);
   haxis->GetXaxis()->SetTickSize(0);
   haxis->GetXaxis()->SetLabelSize(0);


   auto hmain = new TH1D("hmain", "title", main.size(), 0, main.size());
   hmain->SetMarkerSize(2);
   hmain->SetMarkerStyle(33);
   hmain->SetMarkerColor(kBlue);


   auto hpos = new TH1D("hpos", "title", main.size(), 0, main.size());
   hpos->SetFillColor(kRed);
   hpos->SetBarOffset(0.25);
   hpos->SetBarWidth(0.5);

   auto hneg = new TH1D("hneg", "title", main.size(), 0, main.size());
   hneg->SetFillColor(kGreen);
   hneg->SetBarOffset(0.25);
   hneg->SetBarWidth(0.5);

   for (unsigned n = 0; n < main.size(); n++) {
      hmain->SetBinContent(n + 1, main[n]*scale);
      hpos->SetBinContent(n + 1, positive[n]*scale);
      hneg->SetBinContent(n + 1, negative[n]*scale);
   }

   auto c1 = new TCanvas("c1", "title", 1500, 800);
   c1->SetLeftMargin(frame_left);
   c1->SetRightMargin(1 - frame_right);
   c1->SetTopMargin(1 - frame_top);
   c1->SetBottomMargin(frame_bottom);
   c1->SetGridy(1);
   //c1->SetGridx(1);

   c1->Add(haxis, "haxisg;y+"); // horizontal axis with grids, y on top

   c1->Add(hpos, "bar,base0,same");
   c1->Add(hneg, "bar,base0,same");
   c1->Add(hmain, "P,same");

   // draw labels on the right side

   for (unsigned n = 0; n < main.size(); n++) {
      TLatex *l = new TLatex(0.05, frame_bottom + (frame_top - frame_bottom) * (n+0.5) / main.size(), main_labels[n].substr(1, 40).c_str());
      l->SetNDC(true);
      l->SetTextAlign(12);
      l->SetTextSize(0.02);
      l->SetTextColor(kBlue);
      c1->Add(l);
   }

   // draw left arrow

   Double_t xleft[8] = {frame_0 - 0.02,  frame_left + 0.05, frame_left + 0.05, frame_left + 0.03, frame_left + 0.05, frame_left + 0.05, frame_0 - 0.02, frame_0 - 0.02 };
   Double_t yleft[8] = {0.97,            0.97,              0.98,               0.95,      0.92,              0.93,              0.93,    0.97 };
   TPolyLine *pleft = new TPolyLine(8,xleft,yleft);
   pleft->SetFillColor(kGreen);
   pleft->SetNDC();
   c1->Add(pleft, "f");

   TLatex *lleft = new TLatex(frame_0 - 0.05, 0.95, "Left arrow");
   lleft->SetNDC(true);
   lleft->SetTextAlign(32);
   lleft->SetTextSize(0.02);
   lleft->SetTextColor(kWhite);
   c1->Add(lleft, "f");

   // draw right arrow

   Double_t xright[8] = {frame_0 + 0.02,  frame_right - 0.05, frame_right - 0.05, frame_right - 0.03, frame_right - 0.05, frame_right - 0.05, frame_0 + 0.02, frame_0 + 0.02 };
   Double_t yright[8] = {0.97,            0.97,              0.98,               0.95,      0.92,              0.93,              0.93,    0.97 };
   TPolyLine *pright = new TPolyLine(8,xright,yright);
   pright->SetFillColor(kRed);
   pright->SetNDC();
   c1->Add(pright, "f");

   TLatex *lright = new TLatex(frame_0 + 0.05, 0.95, "Right arrow");
   lright->SetNDC(true);
   lright->SetTextAlign(12);
   lright->SetTextSize(0.02);
   lright->SetTextColor(kWhite);
   c1->Add(lright, "f");

   c1->SaveAs("hbar.root");
}