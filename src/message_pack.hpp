#include <cmath>
#include <cstddef>
#include <cstdint>
#include <stdint.h>
#include <tuple>
#include"json.hpp"
enum message_pack_type{
    int8,
    int16,
    int32,
    int64,
    uint8,
    uint16,
    uint32,
    uint64,
    nil_type,
    boolean_true,
    boolean_false,
    float_type32,
    float_type64,
    str8,
    str16,
    str32,
    binary8,
    binary16,
    binary32,
    array16,
    array32,
    map16,
    map32,
    map_void,
    array_void
};
using d64 = double;
using data_type = json_acc_layer_np::data_type;
using json_map = json_acc_layer_np::json_map;
using json = json_acc_layer_np::json;
void jsonmap_to_bsonmap(json_map& map) {
  for (json x : map) {
    switch (x.type) {
      case data_type::str:
        x.type = message_pack_type::str32;
        break;
      case data_type::digit_int:
        x.type = message_pack_type::int64;
        break;
      case data_type::null:
        x.type = message_pack_type::nil_type;
        break;
      case data_type::digit_double:
        x.type = message_pack_type::float_type64;
        break;
      case data_type::object_list:
        x.type = message_pack_type::map32;
        break;
      case data_type::object_void:
        x.type = message_pack_type::map_void;
        break;
      case data_type::bool_t:
        if (x.value == "true") {
          x.type = message_pack_type::boolean_true;
        }
        if (x.value == "false") {
          x.type = message_pack_type::boolean_false;
        }
        break;
      case data_type::array_void:
        x.type = message_pack_type::array_void;
        break;
      case data_type::array_list:
        x.type = message_pack_type::array32;
        break;
      default:
        std::cerr << "InValid Map!\n";
        break;
    }
  }
}