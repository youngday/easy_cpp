set_xmakever("3.0.0")
set_project("xmake-example")
set_languages("c++20")
-- set_policy("build.warning", true)
-- set_warnings("all", "extra")
add_rules("mode.debug", "mode.release")
-- https://xmake.io/mirror/zh-cn/plugin/more_plugins.html
add_rules("plugin.compile_commands.autoupdate")

-- plot 组开关。imgui 的 glfw 后端会拉取 glfw -> libx11/libxext/libxrandr/...
-- 整棵 X11 依赖树，而且 `xmake f` 阶段就会解析/安装。只构建 core 的场景
-- （如 CI）用 `xmake f --plot=n` 跳过，避免在没有图形库的机器上安装失败。
option("plot")
    set_default(true)
    set_showmenu(true)
    set_description("Build implot/implot3d GUI targets (requires system GL/GLFW/X11)")
option_end()

-- ============================================================
-- 依赖声明
--
-- 这里只做 add_requires（声明 + 拉取），不在这里 add_packages。
-- 原实现把所有包 add_packages 到根作用域，等于把 xtensor-blas / sqlite3 /
-- libhv / quill 等重量级库链接进每一个可执行文件（含 concurrentqueue demo、
-- 串口 demo）。现在改为在具体 target 内按需 add_packages：
--   * 每个 target 只编译/链接自己真正 #include 的包；
--   * 未使用的包不再进入链接行，增量构建与 `xmake f` 都更快。
-- ============================================================
add_requires("yyjson 0.12.0")
add_requires("nlohmann_json 3.12.0")
add_requires("simdutf 8.2.0")
add_requires("tomlc17 2026.08.21")
add_requires("quill 12.0.0")
add_requires("fmt 12.2.0")
add_requires("concurrentqueue 1.0.5")
add_requires("libhv 1.3.4")
add_requires("xsimd 14.3.0")
add_requires("xtensor 0.27.1")
add_requires("xtensor-blas 0.23.0")
add_requires("xtl 0.8.2")
add_requires("sqlite_orm 1.9.1")
add_requires("sqlite3 3.53.0+400")
add_requires("cppzmq 4.11.0")
-- plot 系列（implot / implot3d）：仅在 --plot=y 时解析。
-- imgui 的 glfw 后端经 glfw 拉取 libx11/libxext/libxrandr/libxinerama/... 
-- 原实现还开了 sdl2（本项目未使用，sdl2 同样依赖 libxext），一并去掉。
if has_config("plot") then
    add_requires("glad 2.0.8")
    add_requires("nativefiledialog-extended 1.3.0")
    add_requires("cxxopts 3.3.1")
    add_requires("imgui 1.92.9+b", {configs = {glfw = true, opengl3 = true}})
end

-- ------------------------------------------------------------
-- 备用依赖（当前源码未使用，保留声明以便按需启用，避免无谓下载/编译）
-- 需要时把对应行取消注释，并在 target 内 add_packages：
--   yaml-cpp        : 配置若改用 YAML
--   argparse        : 命令行解析备选（现用 cxxopts）
--   atomic_queue    : lock-free 队列备选（现用 concurrentqueue）
--   matplotplusplus : 数据可视化备选（现用 implot），2.15.2023 曾用
--   libcurl         : HTTP 客户端备选（现用 libhv）
--   openssl         : 如需 libhv https/ssl 再显式打开
-- ------------------------------------------------------------

-- add_requires("yaml-cpp 0.8.0")
-- add_requires("argparse 3.2")
-- add_requires("atomic_queue 1.9.2")
-- add_requires("matplotplusplus 1.2.2")
-- add_requires("libcurl 8.21.0")
-- add_requires("openssl", {alias = "openssl", configs = {options = "OpenSSL:shared=True"}})

-- ffmpeg：xmake-repo 没有 libav* 独立包，如需使用直接用系统 ffmpeg，
-- 由对应 target 的 add_links(avfilter/avcodec/...) 链接（需系统装有 ffmpeg 开发库）

-- ============================================================
-- 公共配置
-- ============================================================
-- clang 不自动链接 atomic，否则报 undefined reference to `__atomic_is_lock_free'
add_links("atomic")

-- ============================================================
-- 公共静态库
-- ============================================================

-- utils：src/utils（log / mytime + main.hpp 及子头）。
-- 抽成静态库后 log.cpp / mytime.cpp 只编译一次，而不是被 ~10 个 target 各编译一遍。
-- public 的 includedirs / packages 会传递给依赖它的 target，
-- 因此依赖方只需 add_deps("utils")，无需重复声明。
target("utils")
    set_kind("static")
    add_files("src/utils/*.cpp")
    add_includedirs("src/utils", "src/", {public = true})
    add_packages("simdutf", "fmt", "quill", {public = true})

-- plot_common：implot / implot3d / common / Fonts 公共源码。
-- 5 个 plot 程序原本各自编译一遍这些文件（implot 及其 demo/items 体积不小），
-- 现在只编译一次。静态库按需抽取目标文件，2d 程序不会真的链接进 implot3d 的符号。
if has_config("plot") then
    target("plot_common")
        set_kind("static")
        add_files("src/plot/common/*.cpp",
                  "src/plot/common/Fonts/*.cpp",
                  "src/plot/implot/*.cpp",
                  "src/plot/implot3d/*.cpp")
        add_includedirs("src/plot/common", "src/plot/implot",
                        "src/plot/common/Fonts", "src/plot/implot3d", {public = true})
        add_packages("imgui", "glad", "nativefiledialog-extended", "cxxopts", {public = true})
        add_deps("utils", {public = true})
end

-- ============================================================
-- targets —— 组 "core"（无 GUI 依赖，可在 CI 中构建）
-- ============================================================

target("test")
    set_kind("binary")
    set_group("core")
    add_files("src/async/test.cpp")
    add_deps("utils")

target("async")
    set_kind("binary")
    set_group("core")
    add_files("src/async/async.cpp")
    add_deps("utils")

target("base64")
    set_kind("binary")
    set_group("core")
    add_files("src/base64/base64.cpp")
    add_deps("utils")

target("json")
    set_kind("binary")
    set_group("core")
    add_files("src/json/json.cpp")
    add_deps("utils")
    add_packages("yyjson")

-- config log
target("config_toml")
    set_kind("binary")
    set_group("core")
    add_files("src/config/config_toml.cpp")
    add_deps("utils")
    add_packages("tomlc17")

target("sqlite_orm")
    set_kind("binary")
    set_group("core")
    add_files("src/sqlite_orm/main.cpp")
    add_deps("utils")
    add_packages("sqlite_orm", "sqlite3")

-- lock-free queue demo：noblock / block / bulk
target("queue_nonblock")
    set_kind("binary")
    set_group("core")
    add_files("src/concurrentqueue/nonblock.cpp")
    add_deps("utils")
    add_packages("concurrentqueue")

target("queue_block")
    set_kind("binary")
    set_group("core")
    add_files("src/concurrentqueue/block.cpp")
    add_deps("utils")
    add_packages("concurrentqueue")

target("queue_bulk")
    set_kind("binary")
    set_group("core")
    add_files("src/concurrentqueue/bulk.cpp")
    add_packages("concurrentqueue")
target("cppzmq")
    set_kind("binary")
    set_group("core")
    add_files("src/cppzmq/cppzmq.cpp")
    add_deps("utils")
    add_packages("cppzmq", "nlohmann_json")

-- xtensor / xtensor-blas：xtensor.cpp 直接 include xtensor-blas，需显式链接
target("xtensor")
    set_kind("binary")
    set_group("core")
    add_files("src/xtensor/xtensor.cpp")
    add_deps("utils")
    add_packages("xtensor", "xtensor-blas", "xtl", "xsimd")

-- libhv：udp / tcp / http / websocket，仅依赖 libhv，不需要 utils
local hv_targets = {
    {"hv_udp_client",       "src/hv_udp/UdpClient_test.cpp"},
    {"hv_udp_server",       "src/hv_udp/UdpServer_test.cpp"},
    {"hv_tcp_client",       "src/hv_tcp/TcpClient_test.cpp"},
    {"hv_tcp_client_loop",  "src/hv_tcp/TcpClientEventLoop_test.cpp"},
    {"hv_tcp_server",       "src/hv_tcp/TcpServer_test.cpp"},
    {"hv_http_client",      "src/hv_http/http_client_test.cpp"},
    {"hv_http_server",      "src/hv_http/http_server_test.cpp"},
    {"hv_websocket_client", "src/hv_websocket/websocket_client_test.cpp"},
    {"hv_websocket_server", "src/hv_websocket/websocket_server_test.cpp"},
}
for _, t in ipairs(hv_targets) do
    target(t[1])
        set_kind("binary")
        set_group("core")
        add_files(t[2])
        add_packages("libhv")
end

-- 串口：只依赖本地 include/（atomicops / readerwriterqueue / SerialPort），无外部包
-- sudo chmod 666 /dev/ttyS0
local serial_targets = {
    {"async_serial2_hex",   "src/async_serial2/src/main.cpp"},
    {"async_serial2_asiic", "src/async_serial2/src/main_ascii.cpp"},
}
for _, t in ipairs(serial_targets) do
    target(t[1])
        set_kind("binary")
        set_group("core")
        add_files(t[2], "src/async_serial2/src/SerialPort.cpp")
        add_includedirs("src/async_serial2/include")
end

-- ============================================================
-- ImPlot 系列 —— 组 "plot"（依赖 GL/GLFW，需系统图形库）
-- 公共源码在 plot_common 静态库中，这里只编译各自的 main_*.cpp
-- 整个组随 --plot 开关（默认开）启停。
-- ============================================================
if has_config("plot") then

    -- 2d plot: static / dynamic / csv
    target("implot_dynamic")
        set_kind("binary")
        set_group("plot")
        add_files("src/plot/main_implot_dynamic.cpp")
        add_deps("plot_common")

    target("implot_static")
        set_kind("binary")
        set_group("plot")
        add_files("src/plot/main_implot_static.cpp")
        add_deps("plot_common")

    target("implot_csv")
        set_kind("binary")
        set_group("plot")
        add_files("src/plot/main_implot_csv.cpp")
        add_deps("plot_common")
        add_packages("xtensor", "xtensor-blas", "xtl", "xsimd")

    -- 3d plot
    target("implot3d")
        set_kind("binary")
        set_group("plot")
        add_files("src/plot/main_implot3d.cpp")
        add_deps("plot_common")

    target("implot3d_csv")
        set_kind("binary")
        set_group("plot")
        add_files("src/plot/main_implot3d_csv.cpp")
        add_deps("plot_common")
        add_packages("xtensor", "xtensor-blas", "xtl", "xsimd")

end
