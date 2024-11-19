#include <cmath>
#include <cstddef>
#include <cstdint>
// #include <stdint.h>
#include <tuple>
#include <limits>
#include "json.hpp"

enum class message_pack_type : int32_t
{
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
    str,
    binary8,
    binary16,
    binary32,
    array16,
    array32,
    map16,
    map32,
    map_void,
    array_void,
    invalid_t=-2000
};
using d64 = double;
using data_type = json_acc_layer_np::data_type;
using json_map = json_acc_layer_np::json_map;
using json = json_acc_layer_np::json;

struct messagepk : json_map
{
public:
    std::string serialize()
    {
    }

private:
    message_pack_type minimal_inttype(json_acc_layer_np::i64t v)
    {
        if (v <= std::numeric_limits<int64_t>::max() && v >= std::numeric_limits<int64_t>::lowest())
        {
            if (v <= std::numeric_limits<int32_t>::max() && v >= std::numeric_limits<int32_t>::lowest())
            {
                if (v <= std::numeric_limits<int16_t>::max() && v >= std::numeric_limits<int16_t>::lowest())
                {
                    if (v <= std::numeric_limits<int8_t>::max() && v >= std::numeric_limits<int8_t>::lowest())
                    {
                        return message_pack_type::int8;
                    }
                    return message_pack_type::int16;
                }
                return message_pack_type::int32;
            }
            return message_pack_type::int64;
        }
        throw nullptr;
    }
    void root_serialize()
    {
    }
    void plain_serialize(const int idx)
    {
        message_pack_type finalt = message_pack_type::invalid_t;
        switch (this->at(idx).type)
        {
          case json_acc_layer_np::data_type::digit_int:
            finalt = minimal_inttype(std::get<json_acc_layer_np::i64t>(this->at(idx).get_val()));
          break;
          case json_acc_layer_np::data_type::null:
            finalt = message_pack_type::nil_type;
          break;
          case json_acc_layer_np::data_type::digit_double:
            finalt = message_pack_type::float_type64;
          break;
          case json_acc_layer_np::data_type::str:
            finalt = message_pack_type::str;
          break;
        }
    }
    void object_serialize()
    {
    }
    void array_serialize()
    {
    }
};

/*
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
*/
