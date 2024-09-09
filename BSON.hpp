#include "json.hpp"
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
using d64 = long double;
using data_type = json_acc_layer_np::data_type;
using json_map = json_acc_layer_np::json_map;
using json = json_acc_layer_np::json;
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


void BSON_Serialize_JMap(json_map& map) {
	jsonmap_to_bsonmap(map);
	int sign = 0;

	for (json& x : map) {
		switch (x.type) {

			case bson_data_type::null:

				break;
			case bson_data_type::integer:

				break;
			case bson_data_type::d64_t:

				break;
			case bson_data_type::bool_t:

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
