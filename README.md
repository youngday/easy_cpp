# xmake-example

## build

源码按 target group 组织，共享代码抽成静态库，便于分场景构建：

* `core` —— 无 GUI 依赖（json / queue / libhv / sqlite_orm / xtensor / 串口 …），CI 构建这一组
* `plot` —— implot / implot3d 可视化示例，依赖系统 GL/GLFW

```sh
xmake f -y -m release     # 配置（依赖按 target 声明）
xmake build -y -g core    # 只构建 core 组
xmake build -y -g plot    # 只构建 plot 组
xmake build -y            # 构建全部
```

plot 组开关：

* `--plot=y`（默认）—— 解析并构建 implot/implot3d，需要系统 GL/GLFW/X11
* `--plot=n` —— 完全跳过 GUI 依赖，CI 与无图形库的机器用这个：

```sh
xmake f -y -m release --plot=n   # 不会拉取 imgui/glfw/libx11/libxext 依赖树
xmake build -y -g core
```

共享静态库（避免重复编译）：

* `utils` —— src/utils（log / mytime），被需要日志/时间的 target 依赖
* `plot_common` —— implot / implot3d / common / Fonts 公共源码，5 个 plot 程序共用

## 网络优化
https://xmake.io/zh/guide/package-management/network-optimization.html
```txt
--proxy_hosts=PROXY_HOSTS    Only enable proxy for the given hosts list, it will enable all if be unset,
                             and we can pass match pattern to list:
                                 e.g.
                                 - xmake g --proxy_hosts='github.com,gitlab.*,*.xmake.io'
                                 
xmake g --proxy_hosts='github.com,gitlab.*,*.xmake.io'                                 
```

## local dependent packages

wei get from github and cmake or xmake ,and installed.
  xmake install --admin
  at /usr/local/

* cpplinuxserial
* apt install .

## xmake package

* rapidcsv
read datas from csv file

## libhv

udp ,tcp,http,websocket.

### other examples

lock-free concurrentqueue, cppzmq, csv
🐞

run zlmediakit:

```sh
sudo ./zlmediakit
```

check your v4l2 carmera dev/video?

```cpp
video_fname = std::string("0");
```

|function|lib|alternate|note|
|-|-|-|-|
|frame|asio/libhv|boost,libhv|lightweight frame work|
|arg cli|argparse|cli11|cmd line param parser|
|log|quill|fmtlog loguru |lightweight log|
|csv|rapidcsv|csv2 |csv parser|
|structor data file|npy|csv |share numpy files|
|queue|concurrentqueue|atomic_queue|lock-free concurrent queue|
|async_serial port|cpplinuxserial,with readerwriterqueue |asio2|async serial|
|tcp/http|libhv|coost|tcp http with coroutine|
|websocket|libhv|(uwebsocket not in xmake)|websocket|
|zero mq|cppzmq||message queue between other language|
|json|nlohmann_json|simdjson| simdjson just parser,do not have serializer,waiting for sonic-cpp having cross fun|
|base64|simdutf|cppcodec|simd depend|
|format|fmt|cout|print vector directly,with LOG_S(INFO)<< fmt::format("vector data:{}",data),#include "fmt/core.h",#include "fmt/ranges.h"|
|database orm|sqlite_orm||database orm|


⏰🔑
