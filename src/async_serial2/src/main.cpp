#include <chrono>
#include <iostream>
#include <future>
#include <iomanip>
#include <vector>
#include <thread>

#include "SerialPort.hpp"
#include "readerwriterqueue.h"
#include "atomicops.h"

using namespace std::chrono_literals;
using namespace mn::CppLinuxSerial;
using namespace moodycamel;

using std::cout;
using std::endl;
using std::chrono::milliseconds;

//

BlockingReaderWriterQueue<int> queue_resp;
// 读取线程的函数
void readThreadFunction(SerialPort& serialPort) {
    for (int x = 0; x < 10; x++) {
        std::cout << "Reading" << std::endl;

        std::vector<uint8_t> readData;
        serialPort.ReadBinary(readData);
        cout << "readData size: " << readData.size() << std::endl;
        for (auto d : readData) {
            cout << std::hex << std::setw(2) << d << " ";
        }
        cout << endl;
        queue_resp.enqueue(x);
    }
}

// 写入线程的函数
void writeThreadFunction(SerialPort& serialPort) {
    std::this_thread::sleep_for(100ms);
    int item;
    for (int x = 0; x < 10; x++) {
        std::this_thread::sleep_for(100ms);
        std::cout << "Writing " << std::endl;
        std::vector<uint8_t> writeData = {'H', 'e', 'l', 'l', 'o'};
        serialPort.WriteBinary(writeData);
        cout << "writeData size: " << writeData.size() << std::endl;
        for (auto d : writeData) {
            cout << std::hex << std::setw(2) << d << " ";
        }
        cout << endl;
        if (queue_resp.wait_dequeue_timed(item, milliseconds(500))) {
            cout << "Received response: " << item << endl;
        } else {
            cout << "Timeout waiting for response" << endl;
        }

    }
}

int main() {
    // This example relies on a serial device which echos serial data at 9600
    // baud, 8n1.
    // std::cout << __FILE__ << "::" << __func__ << " called." << std::endl;
    // SerialPort serialPort("/dev/ttyACM0", BaudRate::B_9600, NumDataBits::EIGHT,
    //                       Parity::NONE, NumStopBits::ONE, HardwareFlowControl::OFF,
    //                       SoftwareFlowControl::OFF);
    SerialPort serialPort("/dev/ttyUSB0", BaudRate::B_115200, NumDataBits::EIGHT,
                          Parity::NONE, NumStopBits::ONE, HardwareFlowControl::OFF,
                          SoftwareFlowControl::OFF);
    serialPort.SetTimeout(1000); // Block when reading for 1000ms

    serialPort.Open();

    std::this_thread::sleep_for(100ms);

    // 使用std::async启动异步任务
    // std::launch::async 确保任务在单独的线程中执行
    auto readFuture = std::async(std::launch::async, readThreadFunction, std::ref(serialPort));
    auto writeFuture = std::async(std::launch::async, writeThreadFunction, std::ref(serialPort));

    // 等待两个异步任务完成
    // get()会阻塞直到任务完成，并可以获取返回值（如果有的话）
    readFuture.get();
    writeFuture.get();

    serialPort.Close();
    return 0;
}
