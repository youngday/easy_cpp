# xmake-example

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

### ffmpeg example

sudo apt update
sudo apt install -y libavdevice-dev libavformat-dev libavcodec-dev libavutil-dev libswscale-dev libswresample-dev libavfilter-dev


forked from

[opencv_ffmpeg_streaming](https://github.com/andreanobile/opencv_ffmpeg_streaming)

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
|websocket|drogon/libhv(cross)|(uwebsocket not in xmake)|websocket|
|zero mq|cppzmq||message queue between other language|
|json|nlohmann_json|simdjson| simdjson just parser,do not have serializer,waiting for sonic-cpp having cross fun|
|base64|simdutf|cppcodec|simd depend|
|format|fmt|cout|print vector directly,with LOG_S(INFO)<< fmt::format("vector data:{}",data),#include "fmt/core.h",#include "fmt/ranges.h"|
|stream|ffmpeg||video streamer|
|database orm|sqlite_orm||database orm|


⏰🔑
