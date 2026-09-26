// Demo:   demo.cpp
// Author: Evan Pezent (evanpezent.com)
// Date:   3/26/2021
#include "main.hpp"
#include "App.h"

struct ImPlotDemo : App {
  using App::App;
  int i{0};
  void Update() override {
//update your data here , fps=30
    std::cout<<"update.i: "<<i++<<std::endl;
    if(i>100){
        i=0;
    }
// Demo_ScatterPlots();

    srand(0);
    static float xs1[100], ys1[100];
    for (int i = 0; i < 100; ++i) {
        xs1[i] = i * 0.01f;
        ys1[i] = xs1[i] + 0.1f * ((float)rand() / (float)RAND_MAX);
    }
    static float xs2[50], ys2[50];
    for (int i = 0; i < 50; i++) {
        xs2[i] = 0.25f + 0.2f * ((float)rand() / (float)RAND_MAX);
        ys2[i] = 0.75f + 0.2f * ((float)rand() / (float)RAND_MAX);
    }

    if (ImPlot::BeginPlot("Scatter Plot")) {
        ImPlot::PlotScatter("Data 1", xs1, ys1, 100);
        ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Square, 6, ImPlot::GetColormapColor(1), IMPLOT_AUTO, ImPlot::GetColormapColor(1));
        ImPlot::PlotScatter("Data 2", xs2, ys2, 50);
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
