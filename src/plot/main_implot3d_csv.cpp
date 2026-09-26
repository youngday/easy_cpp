//--------------------------------------------------
// ImPlot3D Example
// main.cpp
// Date: 2024-11-17
// Author: Breno Cunha Queiroz (brenocq.com)
//--------------------------------------------------
// #include "App.h"

#include "main.hpp"
#include "rapidcsv.h"

#include <glad/gl.h>
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "implot3d.h"

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

// Callback to handle GLFW errors
void glfw_error_callback(int error, const char *description) {
  std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// struct ImPlotDemo : App {
//   using App::App;
//   int i{0};
//   void Update() override {
//     // update your data here , fps=30
//     LOG_INFO(logger, "update.i: ", i++);
//     if (i > 100) {
//       i = 0;
//     }
//     rapidcsv::Document doc("nohdr.csv", rapidcsv::LabelParams(-1, -1));
//     //  rapidcsv::SeparatorParams(';'));
//     vector<float> open = doc.GetColumn<float>(0);
//     LOG_INFO(logger, "Read open.size:{0},data:{1}\n", open.size(), open);
//     vector<float> close = doc.GetColumn<float>(5);
//     LOG_INFO(logger, "Read close.size:{0},data:{1}\n", close.size(), close);
//     long long volume = doc.GetCell<long long>(4, 2);
//     LOG_INFO(logger, "Read volume,data:{0}\n", volume);
//     xarray<float> x = linspace(0, 50 - 1, 50);
//     // cout<<"x:"<<x<<endl;
//     vector<float> vx;
//     std::copy(x.cbegin(),x.cend(),std::back_inserter(vx));
//     LOG_INFO(logger, "Read x.size:{0},data:{1}\n", x.size(), vx);

//     // vector<double> y1 = transform(x, [](auto x) { return sin(x); });
//     // vector<double> y2 = transform(x, [](auto x) { return sin(x - 0.25);
//     });
//     // vector<double> y3 = transform(x, [](auto x) { return sin(x - 0.5); });
//     // plot(x, y1, "g", x, y2, "b--o", x, y3, "c*");
//     // plot(x, open, "g", x, close, "b--o");

//     // 🔑: std::vector to char * :open.data()
//     if (ImPlot::BeginPlot("Scatter Plot")) {
//       ImPlot::PlotScatter("Data 1", x.data(), open.data(), open.size());
//       ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
//       ImPlot::SetNextMarkerStyle(ImPlotMarker_Square, 6,
//                                  ImPlot::GetColormapColor(1), IMPLOT_AUTO,
//                                  ImPlot::GetColormapColor(1));
//       ImPlot::PlotScatter("Data 2", x.data(), close.data(), close.size());
//       ImPlot::PopStyleVar();
//       ImPlot::EndPlot();
//     }

//     ImGui::End();
//   }
// };

int main(int argc, char const *argv[]) {
  quill_init();
  auto time_str = mylocal_time();
  LOG_INFO(logger, "Starting at {}!\n", time_str);

  // Setup error callback
  glfwSetErrorCallback(glfw_error_callback);

  // Initialize GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create window
  GLFWwindow *window = glfwCreateWindow(800, 600, "Example", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);

  // Load OpenGL functions using glad
  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize OpenGL loader (glad)" << std::endl;
    glfwTerminate();
    return -1;
  }

  // Setup context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImPlot3D::CreateContext();

  // Setup style
  ImGui::StyleColorsDark();

  // Setup backend
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // Start frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Demo windows
    // ImGui::ShowDemoWindow();
    // ImPlot::ShowDemoWindow();
    ImPlot3D::ShowDemoWindow();

    // Render
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap buffers
    glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImPlot3D::DestroyContext();
  ImPlot::DestroyContext();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
