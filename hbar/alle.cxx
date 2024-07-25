#include <fstream>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TH1.h"
#include "TBox.h"
#include "TLatex.h"


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


int get_color(int n)
{
   switch(n) {
      case 0:  return kRed;
      case 1:  return kGreen;
      case 2:  return kBlue;
      case 3:  return kYellow;
      case 4:  return kMagenta;
      case 5:  return kCyan;
      case 6:  return kOrange;
      case 7:  return kSpring;
      case 8:  return kTeal;
      case 9:  return kAzure;
      case 10:  return kViolet;
      case 11:  return kPink;
   }
   return n+30;
}

void alle(const std::string &fname = "11-7-1.xlsx")
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

   std::vector<std::string> main_labels, short_labels, titles;
   std::vector<double> main, res_phos, res_ratio;

   std::string line, graph_title = "Graphics title";

   double hmin = 0., hmax = 0., glimit = 0., scale = 1e-3;
   int linecnt = -1;

   while (std::getline(f, line)) {

      if (line.empty())
         continue;

      linecnt++;

      auto vect = split_line(line);
      if (vect.size() < 10) {
         printf("Format error\n");
         continue;
      }

      if (linecnt == 0) {
         graph_title = vect[0];
         continue;
      }

      if (linecnt == 1) {
         titles = vect;
         continue;
      }

      main_labels.push_back(vect[0]);
      short_labels.push_back(vect[5]);
      printf("Values %s %s %s\n", vect[6].c_str(),vect[7].c_str(),vect[9].c_str());
      main.push_back(std::stod(vect[6]));
      res_phos.push_back(std::stod(vect[7]));
      res_ratio.push_back(std::stod(vect[9]));
   }

   auto draw_sub = [&](TVirtualPad *pad, const std::string &prefix, const std::string &title, std::vector<double> &vect, double scale = 1) {
      pad->SetLeftMargin(0.12);
      pad->SetRightMargin(0.02);
      pad->SetTopMargin(0.01);
      pad->SetBottomMargin(0.07);
      pad->SetGridy(1);

      double hmin = 0, hmax = 0;
      for(auto &v : vect) {
         if (v < hmin) hmin = v;
         if (v > hmax) hmax = v;
      }

      double scale_min = hmin < 0 ? (hmin - (hmax-hmin) * 0.02) * scale : 0,
             scale_max = (hmax + (hmax-hmin) * 0.02) * scale;

      std::string name = prefix + "axis";
      auto haxis = new TH1D(name.c_str(), "title", vect.size(), 0, vect.size());
      haxis->SetMinimum(scale_min);
      haxis->SetMaximum(scale_max);
      haxis->SetStats(false);
      haxis->GetXaxis()->SetLabelSize(0.06);
      haxis->GetXaxis()->SetLabelOffset(0.01);
      for (unsigned n = 0; n < vect.size(); ++n)
         haxis->GetXaxis()->SetBinLabel(n+1, short_labels[n].c_str());
      haxis->GetYaxis()->SetTitle(title.c_str());
      haxis->GetYaxis()->SetLabelSize(0.04);
      haxis->GetYaxis()->SetTitleSize(0.06);
      haxis->GetYaxis()->SetTitleOffset(0.8);

      pad->Add(haxis, "axisg");

      for (unsigned n = 0; n < vect.size(); ++n) {
         TString hname = TString::Format("hist_%s_%u", prefix.c_str(), n);
         auto h = new TH1D(hname.Data(), "title", vect.size(), 0, vect.size());
         h->SetBinContent(n + 1, vect[n]*scale);
         h->SetFillColor(get_color(n));
         h->SetBarOffset(0.3);
         h->SetBarWidth(0.4);
         pad->Add(h, "bar, base0,same");
      }

      //TLatex *l = new TLatex(0.5, 0.96, title.c_str());
      //l->SetNDC(true);
      //l->SetTextAlign(22);
      //l->SetTextColor(kBlue);
      // pad->Add(l);
   };

   auto c1 = new TCanvas("c1", "title", 1600, 2400);
   c1->SetLeftMargin(0);
   c1->SetRightMargin(0);
   c1->SetTopMargin(0);
   c1->SetBottomMargin(0);

   c1->Divide(1, 3, 0., 0.);

   draw_sub(c1->GetPad(1), "co2", titles[6], main, 1e-3);
   draw_sub(c1->GetPad(2), "phos", titles[7], res_phos, 1e-3);
   draw_sub(c1->GetPad(3), "ratio", titles[9], res_ratio);

   c1->SaveAs("11-7-1.png");
}
