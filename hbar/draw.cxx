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
          res.emplace_back(line.substr(p, p2 - p));
          p = p2 + 1;
       }
    }

    return res;
}

std::string remap_title(const std::string &src)
{
   std::map<std::string, std::string> repl = {

   { "market for natural gas, from low pressure network" , "Market for natural gas"},
   {"market for tap water","Market for tap water"},
   {"hydrogen peroxide production","Hydrogen peroxide production"},
   {"sulfuric acid production","Sulfuric acid production"},
   {"11.6.7.2. optimierte Direkte Emissionen","Optimierte Direkte Emissionen"},
   {"11.1.4. Aktivkohle aus Steinkohel","Aktivkohle aus Steinkohle"},
   {"11.6.1.2. solare trocknung mit pv strom","Solare trocknung mit pv strom"},
   {"11.6.2.2. Netzauslegung mit DOPPELT ","Netzauslegung mit DOPPELT verstärktem rohr"},
   {"soda production, solvay process","Soda production, solvay process"},
   {"treatment of hazardous waste,","Treatment of hazardous waste"},
   {"market for ammonia, liquid |","Market for ammonia, liquid ammonia"},
   {"market for sodium hydroxide,", "Market for sodium hydroxide"},
   {"11.6.6. Direkte Verwendung der Soda", "Direkte Verwendung der Soda"},
   {"treatment of hard coal ash, ", "Treatment of hard coal ash"},
   {"single superphosphate production ", "Single superphosphate production"},
   {"transport, freight, lorry >32 metric ton, EURO4", "Transport, freight, lorry >32 metric ton"},
   {"xxx", "yyy"}
   };

   for (auto &entry : repl)
     if (src.find(entry.first) == 0)
        return entry.second;

   return src.substr(0, 40);
}

void draw(const std::string &fname = "test.xlsx")
{

   std::string csv_name;

   auto pp = fname.find(".xlsx");
   if (pp == std::string::npos) {
      csv_name = fname;
   } else {
      csv_name = fname.substr(0, pp) + ".csv";
      auto exec = TString::Format("./xlsx2csv.py %s %s", fname.c_str(), csv_name.c_str());
      int res = gSystem->Exec(exec);
      printf("Did EXEC %s res %d\n", exec.Data(), res);
   }

   std::ifstream f(csv_name);

   if (!f) {
      printf("fail to open file %s\n", csv_name.c_str());
      return;
   }

   std::vector<double> main, positive, negative;
   std::vector<std::string> main_labels;

   std::vector<double> last_sub;

   std::string line, current_label;

   double current_value = 0., current_direct = 0.,current_sum = 0.,
          current_pos = 0., current_negative = 0.;
   double hmin = 0., hmax = 0.;
   std::vector<double> current_sub;

   auto finish_category = [&]() {
      if (current_sub.size() == 0)
         return;

      printf("   sum: %f diff: %f\n", current_sum, current_sum + current_direct - current_value);

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
         double direct = 0.;
         if (vect.size() > 4 && !vect[4].empty()) {
            direct = std::stod(vect[4]);
            // subv += direct;
         }

         if (subv > 0) {
            current_pos += subv;
         } else {
            current_negative += subv;
         }
         current_sum += subv;
         current_sub.push_back(subv);

         printf("          %f %f info:%s\n", subv, direct, vect[2].substr(0,10).c_str());

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
      current_pos = current_negative = 0.;
      current_value = std::stod(vect[3]);
      current_direct = 0.;
      if (vect.size() > 4 && !vect[4].empty()) {
         current_direct = std::stod(vect[4]);
      }
      if (current_direct > 0)
         current_pos = current_direct;
      else
         current_negative = current_direct;
      current_label = vect[1];
      printf("v = %f %f title: %s\n", current_value, current_direct,  vect[1].c_str());
   }

   finish_category();

   double scale = 1e-6,
          scale_min = (hmin - (hmax-hmin) * 0.1) * scale,
          scale_max = (hmax + (hmax-hmin) * 0.1) * scale,
          frame_left = 0.35, frame_right = 0.98,
          frame_top = 0.8, frame_bottom = 0.05,
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
      double y = frame_bottom + (frame_top - frame_bottom) * (n+0.5) / main.size();
      TLatex *l = new TLatex(0.02, y, remap_title(main_labels[n]).c_str());
      l->SetNDC(true);
      l->SetTextAlign(12);
      l->SetTextSize(0.02);
      l->SetTextColor(kBlue);
      c1->Add(l);

      l = new TLatex(frame_left - 0.01, y, TString::Format("%6.4g", main[n]*scale).Data());
      l->SetNDC(true);
      l->SetTextAlign(32);
      l->SetTextSize(0.03);
      l->SetTextColor(kBlue);
      c1->Add(l);
   }

   TLatex *title = new TLatex((frame_left + frame_right) * 0.5, 0.96, "Graphics title");
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

      std::vector<double>  xpos = {x1,  x2, x2, x3, x2, x2, x1, x1 };
      std::vector<double>  ypos = {y0 + wy, y0 + wy, y0 + wy*1.5, y0, y0 - wy*1.5, y0 - wy, y0 - wy, y0 + wy };
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

   // c1->SaveAs("hbar.root");
}