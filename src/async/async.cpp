#include "main.hpp"
#include <future>
using std::string, std::this_thread::sleep_for;
using namespace std::literals;
using std::chrono::steady_clock;
// a non-optimized way of checking for prime numbers:
bool is_prime(int x) {
  LOG_INFO(logger,"Calculating. Please, wait...");
  for (int i = 2; i < x; ++i)
    if (x % i == 0)
      return false;
  return true;
}

int main() {

  quill_init();
  auto time_str = mylocal_time();

  LOG_INFO(logger, "⏰Starting at {}!", time_str);

  auto start = steady_clock::now();
  // std::ifstream inputFile("test.rknn");
  // if (!inputFile.is_open()) {
  //   LOG_ERROR(logger, "Error opening the file!");
  //   return 1; // Exit the program with an error code
  // }
  // std::string fileContent;
  // std::string line;
  // while (std::getline(inputFile, line)) {
  //   fileContent += line + "\n"; // Append each line to the string
  // }
  // inputFile.close();
  auto dur = (steady_clock::now() - start) / 1ms;
  LOG_INFO(logger, "dur time:{} ms!", dur);
  // float fileSizeMb = fileContent.size() / 1000000.0;
  // LOG_INFO(logger, "File content size:{0} Mbytes,read speed:{1} M/s.",
  //          fileSizeMb, fileSizeMb / dur * 1000);


  while (true) {
    // call is_prime(313222313) asynchronously:
    std::future<bool> fut = std::async(is_prime, 313222313);

     LOG_INFO(logger, "Checking whether 313222313 is prime.");
    // ...
    bool ret = fut.get(); // waits for is_prime to return

    if (ret)
       LOG_INFO(logger,"It is prime!");
    else
      LOG_INFO(logger,"It is not prime.");
    sleep_for(3s);
  }

  return 0;
}
