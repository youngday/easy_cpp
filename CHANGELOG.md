# CHANGELOG

## 10.02 2026
* xmake.lua 重构：依赖按 target 声明，不再把全部包链接进每个可执行文件；新增 core / plot 两个 target group。
* 公共代码抽成静态库 utils、plot_common，避免重复编译（utils ×10、plot 公共源码 ×5）。
* 修复 .github/workflows/main.yml（原本是 action.yml 的副本，CI 从未运行），改为构建 core 组。
* 删除失效源码 src/main.cpp、src/sqlite_orm.cpp、src/test_cppzmq.cpp；修复 bulk.cpp 的 include 并补上 queue_bulk target。
* 未使用的依赖（yaml-cpp/argparse/atomic_queue/matplotplusplus/libcurl/openssl）改为注释保留。
* 新增 cppzmq target（src/cppzmq/cppzmq.cpp，用 nlohmann_json 构造报文）。
* compile_commands.json 取消跟踪（改由 .gitignore 忽略）。
* 新增 option `plot`：GUI 依赖（imgui→glfw→libx11/libxext/...）改为 `--plot=y` 才解析，
  CI 用 `xmake f --plot=n` 只构建 core，修复 GitHub Actions 安装 libxext 失败的问题；
  同时去掉 imgui 未使用的 `sdl2` 后端。

## 03.01 2025
replace coost with libhv.
## 12.21 2024
ormpp to sqlite_orm 
quill 6.1.2 to 7.5.0

## 09.08 2024
* json,back nlomann json , simdjson just parser,not have serializer, waiting for sonic-cpp soon.(cross arm64 check)
* add base64 lib: simdutf, simd depend

## 03.10 2024

* add atomic_queue,but just send one data at once .
* add launch file for vscode plug in of lldb.
* add concurrentqueue package in xmake.lua.

## 12.9 2023

add quill log lib ,to replace loguru.

## 6.12 2023

add fmt lib ,delete xpack,use nlonmann json stl

## 3.07 2023

add toml config ,modern config protocol.

## 3.06 2023

add coost flag_cli cmd arg parser

## 2.16 2023

* rapidcsv
read datas from csv file,and plot them out.

## 2.15 2023
add matplotplusplus, data analyzer.
