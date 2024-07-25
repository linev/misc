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

std::string remap_title(const std::string &src)
{
   std::map<std::string, std::string> repl = {

   {"market for natural gas, from low pressure network" , "Erdgas"},
   {"market for tap water", "Wasser"},
   {"hydrogen peroxide production", "Wasserstoff Peroxid"},
   {"sulfuric acid production", "Schwefelsäure Trocknung"},
   {"11.6.7.2. optimierte Direkte Emissionen", "Direkte Emissionen"},
   {"11.1.4. Aktivkohle aus Steinkohel", "Aktivkohle"},
   {"11.6.1.2. solare trocknung mit pv strom", "Solare Trocknung, PV"},
   {"11.6.2.2. Netzauslegung mit DOPPELT ", "Wärmenutzung Nahwärme"},
   {"soda production, solvay process", "sekundäres Soda"},
   {"treatment of hazardous waste,", "Zyklonasche DK4"},
   {"market for ammonia, liquid |", "Amonium"},
   {"market for sodium hydroxide,", "Natrium Hydroxid"},
   {"11.6.6. Direkte Verwendung der Soda", "Direkte Düngemittelvermarktung"},
   {"treatment of hard coal ash, ", "Filterasche DK2"},
   {"single superphosphate production ", "Substitution Phosphordünger"},
   {"transport, freight, lorry >32 metric ton, EURO4", "LKW Transport"},
   {"market for hard coal", "Substituierte Kohle"},
   {"market for lime", "Substituierter Kalkstein"},
   {"market for clay", "Substituierter Ton"},
   {"11.5.2. Aufwendungen ", "Aufwendungen Stuttgarter V."},
   {"11.1.5. Natriumhydrogencarbonat", "Natriumhydrogencarbonat"},
   {"electricity, high voltage, production mix", "elektrischer Energiemix"},
   {"market for lignite ash", "Asche Braunkohlekraftwerk"},
   {"market for lignite | lignite", "Braunkohle Gewinnung"},
   {"electricity production, lignite | electricity", "elektrische E. Braunkohle"},
   {"11.1.3.2. Direkte Emissionen P-Extract Anlage", "Direkte Emissionen"},
   {"11.1.3.1. Direkte Emissionen P-Extract Anlage", "Direkte Emissionen"},
   {"market for sand | sand |", "Sand"},
   {"11.1.6. Gutschrift Polymervorheizung", "Polymervorheizung"},
   {"market for phosphate rock, as P2O5", "Substituiertes Phosphatgestein"},
   {"11.5.1. Aufwendungen für das Stuttgarter Verfahren", "Aufwendungen Stuttgarter V."},
   {"11.4.3.11111 - CH", "Direkte Emissionen"},
   {"market for concrete, 30-32MPa", "Beton"},
   {"market for forging, steel |", "Stahl"},
   {"flat glass production, uncoated |", "Glass"},
   {"electricity production, photovoltaic, 3kWp", "elektrische Energie PV"},
   {"market for floor heating from borehole heat pump", "subst. Wärme, Sole-WP"},
   {"heat production, air-water heat pump 10kW", "subst. Wärme, Luft-WP"},
   {"chromium steel pipe production |", "Stahlrohr"},
   {"market for mastic asphalt |", "Asphalt"},
   {"polyurethane production, rigid foam | ", "PU-Schaum"},
   {"market for gravel, round |", "Sand und Kies"},
   {"polyethylene production, high density, granulate", "PE-Mantel"},
   {"heat production, borehole heat exchanger, brine-water", "subst. Wärme, Sole-WP"},
   {"market for transport, freight, sea, transoceanic ship |", "Transport Übersee"},
   {"transport, freight, inland waterways, barge | ", "Transport Binnenschiff"},
   {"market for chemical factory, organics |", "Infrastruktur Düngemittelfabrik"},
   {"Natriumhydrogencabonat Natron", "Natriumhydrogencabonat"}
   };

   for (auto &entry : repl)
     if (src.find(entry.first) == 0)
        return entry.second;

   return src.substr(0, 40);
}

void draw(const std::string &fname = "11-1-3-1.xlsx")
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
   std::vector<TBox *> boxes;
   std::vector<TLatex *> labels;

   std::vector<double> last_sub;

   std::string line, current_label,
                graph_title = "Graphics title";

   double current_value = 0., current_direct = 0., current_sum = 0.,
          current_pos = 0., current_negative = 0.;
   double hmin = 0., hmax = 0., glimit = 0., scale = 1e-3,
           total_result = 0, total_direct_contr = 0;
   std::vector<double> current_sub;
   int first_line = 0, main_column = 6;

   auto finish_category = [&]() {
      //if (current_sub.size() == 0)
      //   return;

      if (!current_value && current_label.empty())
         return;

      if (!current_pos && !current_negative) {
         if (current_value > 0)
            current_pos = current_value;
         else
            current_negative = current_value;
      }

      printf("   value: %f sum: %f diff: %f\n", current_value, current_sum, current_sum + current_direct - current_value);

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

      auto vect = split_line(line);
      if (vect.size() < main_column + 1)
         continue;

      if (!vect[0].empty()) {
         // use first line for extra config
         if (!vect[main_column].empty() && first_line == 0) {
            glimit = std::stod(vect[main_column]);
            printf("Global limits %f\n", glimit);
         }

         if (first_line == 2) {
           graph_title = vect[0];
           printf("main result %s\n", vect[main_column].c_str());
           total_result = std::stod(vect[main_column]);
           if (vect.size() > main_column+1 && !vect[main_column+1].empty())
              total_direct_contr = std::stod(vect[main_column + 1]);
         }
         first_line++;

         continue;
      }

      // ignore sub-sub-sub category
      if (vect[0].empty() && vect[1].empty() && vect[2].empty())
         continue;


      if (vect[0].empty() && vect[1].empty()) {
         // sub-sub category

         // printf("subv %s\n", vect[main_column].c_str());

         auto subv = std::stod(vect[main_column]);
         double direct = 0.;
         if (vect.size() > main_column + 1 && !vect[main_column + 1].empty()) {
           direct = std::stod(vect[main_column + 1]);
         }

         if ((vect.size() > main_column + 2) && !vect[main_column + 2].empty()) {
            double x1 = (subv < 0) ? current_negative : current_pos,
                   x2 = x1 + subv;

            TBox *box = new TBox(main.size() + 0.25, x1*scale, main.size() + 0.75, x2*scale);
            if (subv > 0)
               box->SetFillColor(TColor::GetColor((Float_t) 1., (Float_t) .3, (Float_t) 1. - boxes.size()*0.2));
            else
               box->SetFillColor(TColor::GetColor((Float_t) .3, (Float_t) 1., (Float_t) 1. - boxes.size()*0.2));
            boxes.push_back(box);

            TLatex *lbl = new TLatex(main.size() + 0.5, (x1+x2)*0.5*scale,  vect[main_column + 2].c_str());
            lbl->SetTextAlign(22);
            lbl->SetTextSize(0.015);
            labels.push_back(lbl);
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

      if (!vect[0].empty() || vect[1].empty())
         continue;
      // start of new category

      finish_category();

      current_sum = 0;
      current_pos = current_negative = 0.;
      current_value = std::stod(vect[main_column]);
      current_direct = 0.;
      if (vect.size() > main_column + 1 && !vect[main_column + 1].empty()) {
         current_direct = std::stod(vect[main_column + 1]);
      }
      if (current_direct > 0)
         current_pos = current_direct;
      else
         current_negative = current_direct;
      current_label = vect[1];
      printf("v = %f %f title: %s\n", current_value, current_direct,  vect[1].c_str());
   }

   finish_category();

   if (total_direct_contr != 0) {
      main.push_back(total_direct_contr);
      if (total_direct_contr < 0)
         main_labels.push_back("Eingespartes CO2");
      else if (graph_title.find("Trona") != std::string::npos)
         main_labels.push_back("Tronabergbau");
      else if (fname.find("11-6-7") != std::string::npos)
         main_labels.push_back("Direkte Emissionen");
      else
         main_labels.push_back("Direkte Emiss. Trocknung");
      positive.push_back(total_direct_contr > 0 ? total_direct_contr : 0);
      negative.push_back(total_direct_contr < 0 ? total_direct_contr : 0);
      if (total_direct_contr < hmin) hmin = total_direct_contr;
      if (total_direct_contr > hmax) hmax = total_direct_contr;
   }

   double scale_min = glimit != 0 ? -glimit : (hmin - (hmax-hmin) * 0.02) * scale,
          scale_max = glimit != 0 ? glimit : (hmax + (hmax-hmin) * 0.02) * scale,
          frame_left = 0.1, frame_right = 0.76,
          frame_top = 0.8, frame_bottom = 0.05,
          frame_0 = (0 - scale_min) / (scale_max - scale_min) * (frame_right - frame_left) + frame_left;

   bool small_h = fname.find("11-6-7") == 0 || (fname == "11-1-3-1.xlsx");

   double text_scale = fname.find("11-6-1") == 0 || fname.find("11-6-2") == 0
                 || fname.find("11-6-3") == 0 ? 1.3 : 1.;
   if (small_h)
      text_scale = 1.8;

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
      hmain->SetBinContent(n + 1, positive[n]!=0 && negative[n] != 0 ? main[n]*scale : 0);
      hpos->SetBinContent(n + 1, positive[n]*scale);
      hneg->SetBinContent(n + 1, negative[n]*scale);
   }

   auto c1 = new TCanvas("c1", "title", 1500, small_h ? 500 : 800);
   c1->SetLeftMargin(frame_left);
   c1->SetRightMargin(1 - frame_right);
   c1->SetTopMargin(1 - frame_top);
   c1->SetBottomMargin(frame_bottom);
   c1->SetGridy(1);
   //c1->SetGridx(1);

   c1->Add(haxis, "haxisg;y+"); // horizontal axis with grids, y on top

   c1->Add(hpos, "bar,base0,same");
   c1->Add(hneg, "bar,base0,same");

   // draw custom boxes after histograms
   for (auto &box : boxes)
      c1->Add(box);

   for (auto &lbl : labels)
      c1->Add(lbl);

   c1->Add(hmain, "P,same");

   // draw labels on the right side

   for (unsigned n = 0; n < main.size(); n++) {
      double y = frame_bottom + (frame_top - frame_bottom) * (n+0.5) / main.size();
      TLatex *l = new TLatex(frame_right + 0.01, y, remap_title(main_labels[n]).c_str());
      l->SetNDC(true);
      l->SetTextAlign(12);
      l->SetTextSize(0.03*text_scale);
      l->SetTextColor(kBlack);
      c1->Add(l);

      l = new TLatex(frame_left - 0.01, y, TString::Format("%8.2ft", main[n]*1e-3).Data());
      l->SetNDC(true);
      l->SetTextAlign(32);
      l->SetTextSize(0.03*text_scale);
      l->SetTextColor(kBlue);
      c1->Add(l);
   }

   TLatex *title = new TLatex(0.5 /* (frame_left + frame_right) * 0.5 */, 0.96, graph_title.c_str());
   title->SetNDC(true);
   title->SetTextAlign(22);

   //printf("LEN %u\n", graph_title.length());
   if (small_h)
      title->SetTextSize(0.08);
   else if (graph_title.length() > 100)
      title->SetTextSize(0.04);
   else if (graph_title.length() > 72)
      title->SetTextSize(0.055);
   else
      title->SetTextSize(0.06);
   title->SetTextColor(kBlue);
   c1->Add(title);

   TLatex *axis_title = new TLatex(frame_left - 0.01, 0.84, "t CO_{2} Eq");
   axis_title->SetNDC(true);
   axis_title->SetTextAlign(32);
   axis_title->SetTextSize(small_h ? 0.06 : 0.04);
   axis_title->SetTextColor(kBlack);
   c1->Add(axis_title);

   if (total_result != 0) {
      TLatex *res_title = new TLatex(frame_right + 0.01, 0.85, TString::Format("Ergebnis: #color[%d]{%7.1ft}", total_result > 0 ? kRed : kGreen, total_result * scale));
      res_title->SetNDC(true);
      res_title->SetTextAlign(12);
      res_title->SetTextSize(small_h ? 0.07 : 0.05);
      res_title->SetTextColor(kBlue);
      c1->Add(res_title);
   }

   auto add_arrow = [&](bool left_side, const char *txt) {
      double x1 = left_side ? frame_0 - 0.01 : frame_0 + 0.01,
             x2 = left_side ? frame_left + 0.04 : frame_right - 0.04,
             x3 = left_side ? frame_left + 0.02 : frame_right - 0.02,
             y0 = frame_top + 0.08, wy = 0.02;

      if (!left_side && (x1 > x2))
         return;
      if (left_side && (x1 < x2))
         return;

      std::vector<double>  xpos = {x1,  x2, x2, x3, x2, x2, x1, x1 };
      std::vector<double>  ypos = {y0 + wy, y0 + wy, y0 + wy*1.5, y0, y0 - wy*1.5, y0 - wy, y0 - wy, y0 + wy };
      TPolyLine *pleft = new TPolyLine(xpos.size(),xpos.data(),ypos.data());
      pleft->SetFillColor(left_side ? kGreen : kRed);
      pleft->SetNDC();
      c1->Add(pleft, "f");

      if (fabs(x2 - x1) < 0.05)
         return;

      TLatex *l = new TLatex(left_side ? x1 - 0.01 : x1 + 0.01, y0, txt);
      l->SetNDC(true);
      l->SetTextAlign(left_side ? 32 : 12);
      l->SetTextSize(0.03);
      l->SetTextColor(kWhite);
      c1->Add(l);
   };

   add_arrow(true, "Reduction");

   add_arrow(false, "Increase");

   TColor::DefinedColors(1);

   c1->SaveAs("hbar2.root");

   TColor::DefinedColors(-1);

   // c1->SaveAs("hbar2.png");

}