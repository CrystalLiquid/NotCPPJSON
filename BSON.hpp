#include "JSON_ACC_Pool_Str.hpp"
enum bson_data_type {
	str,
	digit,
	bool_t,
	null,
	object,
	array,
	object_void,
	array_void
};
void jsonmap_to_bsonmap(json_acc_str_np::json_pool_str& map) {
	for (json_acc_str_np::json_acc_str x : map) {
		switch (x.type) {
			case json_acc_str_np::data_type::str:
				x.type = bson_data_type::str;
				break;
			case json_acc_str_np::data_type::digit_int:
				x.type = bson_data_type::digit;
				break;
			case json_acc_str_np::data_type::null:
				x.type = bson_data_type::null;
				break;
			case json_acc_str_np::data_type::digit_double:
				x.type = bson_data_type::digit;
				break;
			case json_acc_str_np::data_type::pair_list:
				x.type = bson_data_type::object;
				break;
			case json_acc_str_np::data_type::pair_list_void:
				x.type = bson_data_type::object_void;
				break;
			case json_acc_str_np::data_type::bool_t:
				x.type = bson_data_type::bool_t;
				break;
			case json_acc_str_np::data_type::dimension_void:
				x.type = bson_data_type::array_void;
				break;
			case json_acc_str_np::data_type::dimension_list:
				x.type = bson_data_type::array;
				break;
			default:
				std::cerr << "InValid Map!\n";
				break;
		}
	}
}
