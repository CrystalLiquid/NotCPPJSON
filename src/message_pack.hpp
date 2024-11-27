#include <cmath>
#include <cstddef>
#include <cstdint>
// #include <stdint.h>
#include <string>
#include <tuple>
#include <limits>

#include "json.hpp"

enum class message_pack_type : int64_t
{
    int8=0xd0,
    int16=0xd1,
    int32=0xd2,
    int64=0xd3,
    /*not supporting json :(
    uint8=0xcc,
    uint16=0xcd,
    uint32=0xce,
    uint64=0xcf,
    */
    nil_type=0xc0,
    boolean_true=0xc3,
    boolean_false=0xc2,
    /*not supporting json :(
    float_type32=0xca,
    */
    float_type64=0xcb,
    str8=0xd9,
    str16=0xda,
    str32=0xdb,
    /*not supporting json X(
    binary8=0xc4,
    binary16=0xc5,
    binary32=0xc6,
    */
    array16=0xdc,
    array32=0xdd,
    map16=0xde,
    map32=0xdf,
    fix_str,
    invalid_t=0xc1
};
using d64 = double;
using data_type = json_acc_layer_np::data_type;
using json_map = json_acc_layer_np::json_map;
using json = json_acc_layer_np::json;
using byte = int8_t;
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
        throw error<Err_Code::err_mp_int>(__FUNCTION__, __LINE__, "Not a valid IntVal!");
    }
    void root_serialize()
    {
    }
    void plain_serialize(std::string& result,const int idx)
    {
        message_pack_type finalt = message_pack_type::invalid_t;
        switch (this->at(idx).type)
        {
            case json_acc_layer_np::data_type::digit_int:
                finalt = minimal_inttype(std::get<json_acc_layer_np::i64t>(this->at(idx).get_val()));

                switch (finalt)
                {
                    case message_pack_type::int8: {
                        int8_t int_val;
                        int_val = std::stoi(this->at(idx).value);
                        result.append(reinterpret_cast<char*>(&int_val));
                    }

                    break;
                    case message_pack_type::int16: {
                        int16_t int_val;
                        result.push_back(static_cast<char>(message_pack_type::int16));
                        int_val = std::stoi(this->at(idx).value);
                        result.append(reinterpret_cast<char*>(&int_val));
                    }

                    break;
                    case message_pack_type::int32: {
                        int32_t int_val;
                        int_val = std::stoi(this->at(idx).value);
                        result.append(reinterpret_cast<char*>(&int_val));
                    }
                    break;
                    case message_pack_type::int64: {
                        int64_t int_val;
                        int_val = std::stoi(this->at(idx).value);
                        result.append(reinterpret_cast<char*>(&int_val));
                    }
                    break;
                    default:
                        throw error<Err_Code::err_mp_serial_switch>(__FUNCTION__, __LINE__, "Not a valid Int!");
                        break;
                }
                break;
            case json_acc_layer_np::data_type::null:
                finalt = message_pack_type::nil_type;

                byte nil_val;
                nil_val = 0xc0;
                result.append(reinterpret_cast<char*>(&nil_val));

                break;
            case json_acc_layer_np::data_type::digit_double:
                finalt = message_pack_type::float_type64;
                float fl_val;
                fl_val = std::stof(this->at(idx).value);
                result.append(reinterpret_cast<char*>(&fl_val));
                break;
            case json_acc_layer_np::data_type::str:
                finalt = message_pack_type::fix_str;
                break;
            case json_acc_layer_np::data_type::bool_t:
                if (this->at(idx).value == "true")
                {
                    finalt = message_pack_type::boolean_true;
                }
                if (this->at(idx).value == "false")
                {
                    finalt = message_pack_type::boolean_false;
                }
                break;
            default:
                throw error<Err_Code::err_mp_serial_switch>(__FUNCTION__, __LINE__, "Not a valid type!");
                break;
        }
    }
    void object_serialize(const int idx)
    {
        message_pack_type finalt = message_pack_type::invalid_t;
        switch (this->at(idx).type)
        {
            case json_acc_layer_np::data_type::object_list: break;
            case json_acc_layer_np::data_type::object_void: break;
        }
    }
    void array_serialize(const int idx)
    {
        message_pack_type finalt = message_pack_type::invalid_t;
        switch (this->at(idx).type)
        {
            case json_acc_layer_np::data_type::array_list: break;
            case json_acc_layer_np::data_type::array_void: break;
        }
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
