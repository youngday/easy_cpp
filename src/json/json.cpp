#include "main.hpp"
#include "yyjson.h"

using std::cout, std::endl, std::string;

int main(int argc, char** argv) {

  // 输出文件：可用 argv[1] 指定，默认 output.json
  const char* out_path = (argc > 1) ? argv[1] : "output.json";

  const char* json_text = R"(
  {
    "pi": "3.141",
    "happy": true
  }
)";

  // 解析 JSON（yyjson 为 C 库）
  yyjson_doc* doc = yyjson_read(json_text, strlen(json_text), 0);
  if (!doc) {
    cout << "yyjson parse error" << endl;
    return 1;
  }

  yyjson_val* root = yyjson_doc_get_root(doc);
  cout << "ex1_pi:" << yyjson_get_str(yyjson_obj_get(root, "pi")) << endl;

  // dump（序列化）
  char* dumped = yyjson_write(doc, YYJSON_WRITE_PRETTY, NULL);
  cout << dumped << endl;

  // 同时写入 json 文件
  yyjson_write_err werr;
  if (yyjson_write_file(out_path, doc, YYJSON_WRITE_PRETTY, NULL, &werr)) {
    cout << "written to " << out_path << endl;
  } else {
    cout << "write " << out_path << " error: " << werr.msg << endl;
  }

  // 再把 dump 出来的文本解析一次
  yyjson_doc* doc2 = yyjson_read(dumped, strlen(dumped), 0);
  if (doc2) {
    yyjson_val* root2 = yyjson_doc_get_root(doc2);
    cout << "happy:" << (yyjson_get_bool(yyjson_obj_get(root2, "happy")) ? "true" : "false") << endl;
    yyjson_doc_free(doc2);
  }
  free(dumped);

  yyjson_doc_free(doc);
  return 0;
}
