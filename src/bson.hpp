#include "json.hpp"
#include <cstddef>
#include <cstdint>
#include <tuple>
/*@
Attention! This Lib is Only Compatible With JSON!!!
Extra Specification Might Not Be Implemented!!!
@*/
enum bson_data_type {
	str,
	integer,
	d64_t,
	bool_t,
	null,
	object,
	array,
	object_void,
	array_void
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
	cstring=11,
	javascript_code=13,
	int32 = 16,
	uint64_timestamp=17,
	int64_int = 18,
	decimal_128=19,
	min_key=-1,
	max_key=128,
};
using d64 = long double;
using data_type = json_acc_layer_np::data_type;
using json_map = json_acc_layer_np::json_map;
using json = json_acc_layer_np::json;
template <typename ...Args>

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
void BSON_Serialize_BMap() {

}

#include <utility>
void BSON_Serialize_JMap(json_map& map, std::string&& buf) {
	jsonmap_to_bsonmap(map);
	int8_t signed_bytes = 0;
	uint8_t unsigned_bytes = 0;
	int32_t final_document_len = 0;
	int32_t head_size = 0;
	for (json& x : map) {
		switch (x.type) {

			case bson_data_type::null:
				signed_bytes = 10;
				buf.push_back(static_cast<char>(signed_bytes));
				break;
			case bson_data_type::integer:
				
				break;
			case bson_data_type::d64_t:
				
				signed_bytes = 2;
				head_size = sizeof(d64_t)+sizeof(signed_bytes)+;
				buf.push_back(head_size);
				
				buf.append(x.title);
				buf.push_back(static_cast<char>(signed_bytes));
				

				signed_bytes = 0;
				head_size = 0;
				break;
			case bson_data_type::bool_t:
				if (x.content == "true") {
					unsigned_bytes = 1;
				}
				else if (x.content == "false") {
					unsigned_bytes = 0;
				}else {
					std::cerr<<"Error Type\n";
					return;
				}
				buf.push_back(static_cast<char>(unsigned_bytes));
				break;
			case bson_data_type::str:

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
				//TODO
				break;
		}
	}
}
