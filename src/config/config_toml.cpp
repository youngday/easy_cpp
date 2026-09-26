#include "main.hpp"
#include "tomlc17.h"

using namespace std;


int main(int argc, char *argv[]) {

  quill_init();
  auto time_str = mylocal_time();
  LOG_INFO(logger, "Starting at {}!\n", time_str);

  // tomlc17: 解析文件失败时返回的 result.ok 为 false
  toml_result_t res = toml_parse_file_ex("config/config.toml");
  if (!res.ok) {
    LOG_INFO(logger, "parse toml file error: {}\n", res.errmsg);
    return 1;
  }

  // str_array.test 是一个字符串数组
  toml_datum_t str_array = toml_get(res.toptab, "str_array");
  toml_datum_t test = toml_get(str_array, "test");

  vector<string> strTest;
  if (test.type == TOML_ARRAY) {
    for (int32_t i = 0; i < test.u.arr.size; i++) {
      toml_datum_t elem = test.u.arr.elem[i];
      if (elem.type == TOML_STRING) {
        strTest.emplace_back(elem.u.str.ptr, elem.u.str.len);
      }
    }
  }

  LOG_INFO(logger, "strTest:{0}.", strTest);

  toml_free(res);

  return 0;
}
