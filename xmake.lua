set_xmakever("3.0.0")
set_project("xmake-example")
set_languages("c++20")
-- set_policy("build.warning", true)
-- set_warnings("all", "extra")
add_rules("mode.debug", "mode.release")
-- https://xmake.io/mirror/zh-cn/plugin/more_plugins.html
add_rules("plugin.compile_commands.autoupdate")

local libs = {
    "yyjson 0.12.0", "simdutf 8.2.0", "yaml-cpp 0.8.0", "tomlc17 2026.08.21",
    "quill 12.0.0", "fmt 12.2.0",
    "cppzmq 4.11.0", "argparse 3.2", "atomic_queue 1.9.2",
    "concurrentqueue 1.0.5", "libhv 1.3.4",
    "xsimd 14.3.0", "xtensor 0.27.1", "xtensor-blas 0.23.0", "xtl 0.8.2",
    "sqlite_orm 1.9.1", "sqlite3 3.53.0+400",
}
for _, spec in ipairs(libs) do
    add_requires(spec)                            -- 下载/安装
    add_packages(spec:match("^%S+") or spec)      -- 链接到所有 target
end

-- 仅按需链接到具体 target 的包（实际链接由对应 target 内的 add_packages 决定）
add_requires("matplotplusplus 1.2.2", "glad 2.0.8",
             "nativefiledialog-extended 1.3.0", "cxxopts 3.3.1", "libcurl 8.21.0")

-- 需要 configs 的包，单独声明
add_requires("openssl", {alias = "openssl", configs = {options = "OpenSSL:shared=True"}})
add_requires("imgui 1.92.9+b", {configs = {glfw = true, opengl3 = true, sdl2 = true}})

-- ffmpeg：xmake-repo 没有 libav* 独立包，这里直接用系统 ffmpeg，
-- 由 ffmpeg target 的 add_links(avfilter/avcodec/...) 链接（需系统装有 ffmpeg 开发库）

-- ============================================================
-- 公共配置
-- ============================================================
-- clang 不自动链接 atomic，否则报 undefined reference to `__atomic_is_lock_free'
add_links("atomic")

add_includedirs("src/utils", "src/")
add_files("src/utils/*.cpp")

-- ============================================================
-- targets
-- ============================================================

target("test")
set_kind("binary")
add_files("src/async/test.cpp")

target("sqlite_orm")
set_kind("binary")
add_files("src/sqlite_orm/main.cpp")

-- config log
target("config_toml")
set_kind("binary")
add_files("src/config/config_toml.cpp")

-- sudo chmod 666 /dev/ttyS0
target("async_serial_hex")
set_kind("binary")
-- add_linkdirs("lib")
-- add_links("CppLinuxSerial")
add_files("src/async_serial/src/main.cpp" )
add_files("src/async_serial/src/SerialPort.cpp" )
add_includedirs("src/async_serial/include")

target("async_serial_asiic")
set_kind("binary")
-- add_linkdirs("lib")
-- add_links("CppLinuxSerial")
add_files("src/async_serial/src/main_ascii.cpp" )
add_files("src/async_serial/src/SerialPort.cpp" )
add_includedirs("src/async_serial/include")

-- noblock mpmc_block mpmc_bulk
target("queue_nonblock")
set_kind("binary")
add_files("src/concurrentqueue/nonblock.cpp")

target("queue_block")
set_kind("binary")
add_files("src/concurrentqueue/block.cpp")

target("json")
    set_kind("binary")
    add_files("src/json/json.cpp")

target("async")
    set_kind("binary")
    add_files("src/async/async.cpp")

-- libhv
target("hv_udp_client")
    set_kind("binary")
    add_files("src/hv_udp/UdpClient_test.cpp")

target("hv_udp_server")
    set_kind("binary")
    add_files("src/hv_udp/UdpServer_test.cpp")

target("hv_tcp_client")
    set_kind("binary")
    add_files("src/hv_tcp/TcpClient_test.cpp")

target("hv_tcp_client_loop")
    set_kind("binary")
    add_files("src/hv_tcp/TcpClientEventLoop_test.cpp")

target("hv_tcp_server")
    set_kind("binary")
    add_files("src/hv_tcp/TcpServer_test.cpp")

target("hv_http_client")
    set_kind("binary")
    add_files("src/hv_http/http_client_test.cpp")

target("hv_http_server")
    set_kind("binary")
    add_files("src/hv_http/http_server_test.cpp")

target("hv_websocket_client")
    set_kind("binary")
    add_files("src/hv_websocket/websocket_client_test.cpp")

target("hv_websocket_server")
set_kind("binary")
add_files("src/hv_websocket/websocket_server_test.cpp")

-- xtensor / xtensor-blas 已在根作用域 add_packages，这里无需重复
target("xtensor")
    set_kind("binary")
    add_files("src/xtensor/xtensor.cpp")

-- ============================================================
-- ImPlot 系列：共用源码 / 头文件 / 依赖抽到 add_implot()，避免重复
--   add_implot(额外源码, 额外头文件目录)
-- ============================================================
local implot_files = {
    "src/plot/common/Fonts/*.cpp",
    "src/plot/implot/*.cpp",
    "src/plot/common/*.cpp",
}
local implot_incs = {
    "src/plot/common",
    "src/plot/implot",
    "src/plot/common/Fonts",
}
local implot_pkgs = {"imgui", "glad", "nativefiledialog-extended", "cxxopts"}

local function add_implot(extra_file, extra_inc)
    for _, f in ipairs(implot_files) do add_files(f) end
    for _, d in ipairs(implot_incs) do add_includedirs(d) end
    for _, p in ipairs(implot_pkgs) do add_packages(p) end
    if extra_file then add_files(extra_file) end
    if extra_inc then add_includedirs(extra_inc) end
end

-- 2d plot: static / dynamic / csv
target("implot_dynamic")
    set_kind("binary")
    add_files("src/plot/main_implot_dynamic.cpp")
    add_implot()

target("implot_static")
    set_kind("binary")
    add_files("src/plot/main_implot_static.cpp")
    add_implot()

target("implot_csv")
    set_kind("binary")
    add_files("src/plot/main_implot_csv.cpp")
    add_implot()

-- 3d plot
target("implot3d")
    set_kind("binary")
    add_files("src/plot/main_implot3d.cpp")
    add_implot("src/plot/implot3d/*.cpp", "src/plot/implot3d")

target("implot3d_csv")
    set_kind("binary")
    add_files("src/plot/main_implot3d_csv.cpp")
    add_implot("src/plot/implot3d/*.cpp", "src/plot/implot3d")

target("base64")
set_kind("binary")
add_files("src/base64/base64.cpp")
