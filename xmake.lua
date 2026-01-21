set_xmakever("3.0.0")
set_project("xmake-example")
set_languages("c++20")
-- set_policy("build.warning", true)--warn
-- set_warnings("all", "extra")--warn
add_rules("mode.debug", "mode.release")
-- https://xmake.io/mirror/zh-cn/plugin/more_plugins.html
-- add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})
add_rules("plugin.compile_commands.autoupdate")

add_requires("opencv", { system = true })
add_requires("nlohmann_json 3.12.0", "simdutf 8.0.0", "toml11 4.4.0", "quill 11.0.2", "fmt 12.1.0", "libnpy 1.0.1",
    "argparse 3.2")
add_requires("concurrentqueue 1.0.4", "libhv 1.3.3", "cppzmq 4.11.0")
add_requires("asio2 2.9.0")
add_packages("nlohmann_json", "simdutf", "toml11", "quill", "fmt", "libnpy")
add_packages("asio2")
add_packages("cppzmq", "concurrentqueue", "libhv")
add_requires("ffmpeg 7.1")
add_packages("ffmpeg")
add_requires("sqlite_orm 1.9 ")
add_packages("sqlite_orm")

-- xtensor dependencies - manually configured to use system packages
-- sudo apt install -y libxtensor-dev libxtensor-blas-dev xtl-dev
add_includedirs("/usr/include")
add_includedirs("/usr/include/xtensor")
add_includedirs("/usr/include/xtl")

add_includedirs("src/utils")
add_files("src/utils/*.cpp")

add_includedirs("src/")
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

target("serial")
set_kind("binary")
add_linkdirs("lib")
add_links("CppLinuxSerial")
add_files("src/serial/FlowControl.cpp")
add_includedirs("src/CppLinuxSerial")

-- asio2
-- sudo chmod 666 /dev/ttyS0
target("asio2_serial")
set_kind("binary")
add_files("src/asio2/serial.cpp")

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

target("xtensor")
set_kind("binary")
add_files("src/xtensor/xtensor.cpp")

target("ffmpeg")
set_kind("binary")
add_files("src/streamer/*.cpp")
add_includedirs("src/streamer")
-- add_packages("ffmpeg", {public = true})
add_packages("opencv")
add_links("avfilter", "avdevice", "avformat", "avcodec", "swscale", "swresample", "avutil", "postproc")

target("base64")
set_kind("binary")
add_files("src/base64/base64.cpp")
