# xmake-example

## local dependent packages

wei get from github and cmake or xmake ,and installed.
  xmake install --admin
  at /usr/local/

* cpplinuxserial
* gnuplot ,apt install .

## xmake package

* matplotplusplus
like matplotlib of python,data analyzer .
* rapidcsv
read datas from csv file,and plot them out.

## libhv

udp ,tcp,http,websocket.

### other examples

lock-free concurrentqueue, cppzmq, matplot,csv
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
|serial port|cpplinuxserial|asio,cserialport,libserial|ws|
|tcp/http|libhv|coost|tcp http with coroutine|
|websocket|libhv|(uwebsocket not in xmake)|websocket|
|zero mq|cppzmq||message queue between other language|
|2D/3D plot|matplotplusplus|cvplot|plot 2D/3D static ,depend on gnuplot|
|2D/3D implot/implot3d|implot/implot3d|cvplot|plot 2D/3D,dynamic depend imgui|
|json|nlohmann_json|simdjson| simdjson just parser,do not have serializer,waiting for sonic-cpp having cross fun|
|base64|simdutf|cppcodec|simd depend|
|format|fmt|cout|print vector directly,with LOG_S(INFO)<< fmt::format("vector data:{}",data),#include "fmt/core.h",#include "fmt/ranges.h"|
|database orm|sqlite_orm||database orm|


⏰🔑
