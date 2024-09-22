#include <cmath>
#include <cstddef>
#include <cstdint>
#include <tuple>

#include "json.hpp"

/*@
Attention! This Lib is Only Compatible With JSON!!!
Extra Specification Might Not Be Implemented!!!
@*/
enum bson_data_type {
  str = 11,
  integer = 18,
  d64_t = 1,
  bool_t = 8,
  null = 10,
  object = 540,
  array = 541,
  object_void = 542,
  array_void = 543
};

enum bson_ext_full_type {
  f64b = 1,
  u8str = 2,
  emb_doc = 3,
  array_bson = 4,
  bin_data = 5,
  object_id = 7,
  boolean = 8,
  utc_datetime = 9,
  null_val = 10,
  cstring = 11,
  javascript_code = 13,
  int32 = 16,
  uint64_timestamp = 17,
  int64_int = 18,
  decimal_128 = 19,
  min_key = -1,
  max_key = 128,
};
using d64 = double;
using data_type = json_acc_layer_np::data_type;
using json_map = json_acc_layer_np::json_map;
using json = json_acc_layer_np::json;
template <typename... Args>

void jsonmap_to_bsonmap(json_map& map) {
  for (json x : map) {
    switch (x.type) {
      case data_type::str:
        x.type = bson_data_type::str;
        break;
      case data_type::digit_int:
        x.type = bson_data_type::integer;
        break;
      case data_type::null:
        x.type = bson_data_type::null;
        break;
      case data_type::digit_double:
        x.type = bson_data_type::d64_t;
        break;
      case data_type::pair_list:
        x.type = bson_data_type::object;
        break;
      case data_type::pair_list_void:
        x.type = bson_data_type::object_void;
        break;
      case data_type::bool_t:
        x.type = bson_data_type::bool_t;
        break;
      case data_type::dimension_void:
        x.type = bson_data_type::array_void;
        break;
      case data_type::dimension_list:
        x.type = bson_data_type::array;
        break;
      default:
        std::cerr << "InValid Map!\n";
        break;
    }
  }
}
void BSON_Serialize_BMap() {}
namespace bson_basic {
void null_serial(std::string& final_data, json& node) {
  int8_t signed_bytes = bson_data_type::null;
  int32_t head_size = 0;
  head_size = sizeof(signed_bytes) + node.key.size();
  final_data.push_back(head_size);
  final_data.push_back(static_cast<char>(signed_bytes));
  final_data.append(node.key);
}
void double_serial(std::string& final_data, json& node) {
  int8_t signed_bytes = bson_data_type::d64_t;
  int32_t head_size = 0;
  head_size = sizeof(signed_bytes) + sizeof(double) + node.key.size();
  final_data.push_back(head_size);
  final_data.push_back(static_cast<char>(signed_bytes));
  final_data.append(node.key);
  final_data.push_back(static_cast<char>(std::get<double>(node.get_val())));
}
void int64_serial(std::string& final_data, json& node) {
  int8_t signed_bytes = bson_data_type::integer;
  int32_t head_size = 0;
  head_size = sizeof(signed_bytes) + sizeof(int64_t) + node.key.size();
  final_data.push_back(head_size);
  final_data.push_back(static_cast<char>(signed_bytes));
  final_data.append(node.key);
  final_data.push_back(static_cast<char>(std::get<int64_t>(node.get_val())));
}
void bool_serial(std::string& final_data, json& node) {
  int8_t signed_bytes = bson_data_type::bool_t;
  uint8_t unsigned_bytes = 10;
  if (node.value == "true") {
    unsigned_bytes = 1;
  }
  if (node.value == "false") {
    unsigned_bytes = 0;
  }
  if (unsigned_bytes == 10) {
    throw node;
  }
  int32_t head_size = 0;
  head_size = sizeof(signed_bytes) + sizeof(bool) + node.key.size();
  final_data.push_back(head_size);
  final_data.push_back(static_cast<char>(signed_bytes));
  final_data.append(node.key);
  final_data.push_back(static_cast<char>(unsigned_bytes));
}
void str_serial(std::string& final_data, json& node) {
  int8_t signed_bytes = bson_data_type::str;
  int32_t head_size = 0;
  head_size = sizeof(signed_bytes) + node.value.size() + node.key.size();
  final_data.push_back(head_size);
  final_data.push_back(static_cast<char>(signed_bytes));
  final_data.append(node.key);
  final_data.append(node.value);
}
}  // namespace bson_basic
namespace bson_advance {
void array(std::string& final_data, json& node, json_map& map) {
  int8_t signed_bytes = bson_data_type::str;
  int32_t head_size = 0;
  head_size = sizeof(signed_bytes) + node.value.size();
  for (int i : node.Child_idx) {
	head_size += map[i].key.size();
  }
}
void object(std::string& final_data, json& node, json_map& map) {}
}  // namespace bson_advance
#include <utility>
void BSON_Serialize_JMap(json_map& map, std::string&& buf) {
  jsonmap_to_bsonmap(map);
  int32_t final_document_len = 0;
  int32_t head_size = 0;
  for (json& x : map) {
    switch (x.type) {
      case bson_data_type::null:
        bson_basic::null_serial(buf, x);
        break;
      case bson_data_type::integer:
        bson_basic::int64_serial(buf, x);
        break;
      case bson_data_type::d64_t:
        bson_basic::double_serial(buf, x);

        break;
      case bson_data_type::bool_t:
        bson_basic::bool_serial(buf, x);
        break;
      case bson_data_type::str:
        bson_basic::str_serial(buf, x);
        break;
      case bson_data_type::array:

        break;
      case bson_data_type::object:

        break;
      case bson_data_type::array_void:

        break;
      case bson_data_type::object_void:

        break;
      default:
        // TODO
        break;
    }
  }
}
