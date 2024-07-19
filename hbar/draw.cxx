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

    if (!f) { printf("fail to open file %s\n", fname); return; }


    std::vector<double> main;

    std::string line;
    while (std::getline(f, line)) {

        if (line.empty()) continue;

        if (line.find(",,") == 0) {
            continue;
        }

        if (line[0] != ',')
           continue;

        printf("%s\n", line.c_str());
        auto vect = split_line(line);
        //for (unsigned n = 0; n < vect.size(); n++)
        //   printf("   [%u]: %s\n", n, vect[n].c_str());

        if (vect.size() > 3) {
            auto v = std::stod(vect[3]);
            printf("v = %f\n", v);
            main.push_back(v);
        }
    }

    auto h = new TH1D("main", "title", main.size(), 0, main.size());
    h->SetFillColor(kRed);

    h->SetBarOffset(0.25);
    h->SetBarWidth(0.5);

    for (unsigned n = 0; n < main.size(); n++)
       h->SetBinContent(n + 1, main[n]);

    auto c1 = new TCanvas("c1", "title", 1500, 800);
    c1->Add(h, "bar,base0");

}