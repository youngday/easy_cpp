#include "main.hpp"

using namespace std;


int main(int argc, char *argv[]) {

  quill_init();
  auto time_str = mylocal_time();
  LOG_INFO(logger, "Starting at {}!\n", time_str);

  const auto config = toml::try_parse("config/config.toml");

    if (!config.is_ok()) // error
    {
      LOG_INFO(logger, "parse toml file error.\n");
    } else { // ok
    }

  vector<string> strTest;
  auto toml_size = config.unwrap().at("str_array").at("test").as_array().size();
  // auto test = config.unwrap().at("str_array").at("test").as_array().at(0).as_string();
  for (size_t i = 0; i < toml_size; i++) {
    // Pass the value directly to emplace_back()
    strTest.emplace_back(config.unwrap().at("str_array").at("test").as_array().at(i).as_string()); // array to vector
  }

    LOG_INFO(logger,"strTest:{0}.", strTest);

  // // use a visitor to iterate over heterogenous data
  // config.for_each([](auto &key, auto &value)
  //                 {
  // std::cout << value << "\n";
  // if constexpr (toml::is_string<decltype(value)>)
  //     do_something_with_string_values(value); });

  // // you can also iterate more 'traditionally' using a ranged-for
  // for (auto &&[k, v] : config)
  // {
  //     // ...
  // }

  // re-serialize as TOML
  // std::cout << config << "\n";

  // // re-serialize as JSON
  // std::cout << toml::json_formatter{config} << "\n";


  return 0;
}


