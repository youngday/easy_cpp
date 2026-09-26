#include "App.h"

#include "main.hpp"
#include "rapidcsv.h"
#include "implot.h"
#include <cmath>

#include <algorithm>
#include <ostream>
#include <xtensor-blas/xblas.hpp>
#include <xtensor-blas/xlapack.hpp>
#include <xtensor-blas/xlinalg.hpp>
#include <xtensor/containers/xarray.hpp>
#include <xtensor/containers/xtensor.hpp>
#include <xtensor/generators/xbuilder.hpp>
#include <xtensor/io/xcsv.hpp>
#include <xtensor/io/xio.hpp>
#include <xtensor/io/xnpy.hpp>
#include <xtensor/misc/xpad.hpp>
#include <xtensor/views/xview.hpp>

using namespace std;
using namespace xt;

struct ImPlotDemo : App {
  using App::App;
  int i{0};
  void Update() override {
    // update your data here , fps=30
    LOG_INFO(logger, "update.i: ", i++);
    if (i > 100) {
      i = 0;
    }
    rapidcsv::Document doc("nohdr.csv", rapidcsv::LabelParams(-1, -1));
    //  rapidcsv::SeparatorParams(';'));
    vector<float> open = doc.GetColumn<float>(0);
    LOG_INFO(logger, "Read open.size:{0},data:{1}\n", open.size(), open);
    vector<float> close = doc.GetColumn<float>(5);
    LOG_INFO(logger, "Read close.size:{0},data:{1}\n", close.size(), close);
    long long volume = doc.GetCell<long long>(4, 2);
    LOG_INFO(logger, "Read volume,data:{0}\n", volume);
    xarray<float> x = linspace(0, 50 - 1, 50);
    // cout<<"x:"<<x<<endl;
    vector<float> vx;
    std::copy(x.cbegin(),x.cend(),std::back_inserter(vx));
    LOG_INFO(logger, "Read x.size:{0},data:{1}\n", x.size(), vx);

    // vector<double> y1 = transform(x, [](auto x) { return sin(x); });
    // vector<double> y2 = transform(x, [](auto x) { return sin(x - 0.25); });
    // vector<double> y3 = transform(x, [](auto x) { return sin(x - 0.5); });
    // plot(x, y1, "g", x, y2, "b--o", x, y3, "c*");
    // plot(x, open, "g", x, close, "b--o");

    // 🔑: std::vector to char * :open.data()
    if (ImPlot::BeginPlot("Scatter Plot")) {
      ImPlot::PlotScatter("Data 1", x.data(), open.data(), open.size());
      ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
      ImPlot::SetNextMarkerStyle(ImPlotMarker_Square, 6,
                                 ImPlot::GetColormapColor(1), IMPLOT_AUTO,
                                 ImPlot::GetColormapColor(1));
      ImPlot::PlotScatter("Data 2", x.data(), close.data(), close.size());
      ImPlot::PopStyleVar();
      ImPlot::EndPlot();
    }

    ImGui::End();
  }
};

int main(int argc, char const *argv[]) {
    quill_init();
  auto time_str = mylocal_time();
  LOG_INFO(logger, "Starting at {}!\n", time_str);
  ImPlotDemo app("ImPlot Demo", 1920, 1080, argc, argv);
  app.Run();

  return 0;
}
