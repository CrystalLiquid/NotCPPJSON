#include <chrono>
#include <iostream>
#include <string>

#include "../src/json.hpp"
#include "../src/json_io.hpp"

using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
int main() {
  const char* path = "2.json";
  std::string data;
  HP_IO::json_file FILE;
  json_map map;
  json root;
  map.emplace_back(root);
  FILE.read(path, data);
  map.parse(data);

  high_resolution_clock::time_point Stime = high_resolution_clock::now();
  for (int i = 0; i < 250; ++i) {
    map["systembit"+std::to_string(i)][64];  // add
    map["key_new"+std::to_string(i)]["wtf"];
  }
  for (int i = 0; i <= 249; ++i) {
    map.find("systembit"+std::to_string(i))[0].delete_which();
    map.find("key_new"+std::to_string(i))[0].delete_which();
  }
  
  high_resolution_clock::time_point Etime = high_resolution_clock::now();
  milliseconds Op_Interval =
      std::chrono::duration_cast<milliseconds>(Etime - Stime);
  std::cout << "OpTime:" << Op_Interval.count() << "ms" << std::endl;
  LP_BASIC_IO::IO_Write(map.serialize(), "output.json");
}