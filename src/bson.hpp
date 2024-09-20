#include "json.hpp"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <tuple>
/*@
Attention! This Lib is Only Compatible With JSON!!!
Extra Specification Might Not Be Implemented!!!
@*/
enum bson_data_type {
	str=11,
	integer=18,
	d64_t=1,
	bool_t=8,
	null=10,
	object=540,
	array=541,
	object_void=542,
	array_void=543
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
using d64 = double;
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
template <typename val_type,typename bytes_type>
int32_t calc_headsize(json& node,val_type v,bytes_type b){
	return sizeof(v)+sizeof(b)+node.title.size();
}
void null_serial(std::string&final_data,json& node){
	int8_t signed_bytes = bson_data_type::null;
	int32_t head_size = 0;
	head_size = sizeof(signed_bytes)+node.title.size();
	final_data.push_back(head_size);
	final_data.push_back(static_cast<char>(signed_bytes));
	final_data.append(node.title);
	
}
void double_serial(std::string&final_data,json& node){
	int8_t signed_bytes = bson_data_type::d64_t;
	int32_t head_size = 0;
	head_size = sizeof(signed_bytes)+sizeof(double)+node.title.size();
	final_data.push_back(head_size);
	final_data.push_back(static_cast<char>(signed_bytes));
	final_data.append(node.title);
	final_data.push_back(static_cast<char>(std::get<double>(node.get_val())));
}
void int64_serial(std::string&final_data,json& node){
	int8_t signed_bytes = bson_data_type::integer;
	int32_t head_size = 0;
	head_size = sizeof(signed_bytes)+sizeof(int64_t)+node.title.size();
	final_data.push_back(head_size);
	final_data.push_back(static_cast<char>(signed_bytes));
	final_data.append(node.title);
	final_data.push_back(static_cast<char>(std::get<int64_t>(node.get_val())));
}
void bool_serial(std::string&final_data,json& node){
	int8_t signed_bytes = bson_data_type::bool_t;
	int8_t unsigned_bytes = 10;
	if (node.content == "true") {
		unsigned_bytes = 1;
	}
	if (node.content == "false") {
		unsigned_bytes = 0;
	}
	if (unsigned_bytes==10) {
		throw node;
	}
	int32_t head_size = 0;
	head_size = sizeof(signed_bytes)+sizeof(bool)+node.title.size();
	final_data.push_back(head_size);
	final_data.push_back(static_cast<char>(signed_bytes));
	final_data.append(node.title);
	final_data.push_back(static_cast<char>(unsigned_bytes));
}
void str_serial(std::string&final_data,json& node){
	int8_t signed_bytes = bson_data_type::str;
	int32_t head_size = 0;
	head_size = sizeof(signed_bytes)+node.content.size()+node.title.size();
	final_data.push_back(head_size);
	final_data.push_back(static_cast<char>(signed_bytes));
	final_data.append(node.title);
	final_data.append(node.content);
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
				null_serial(buf,x);
				break;
			case bson_data_type::integer:
				int64_serial(buf,x);
				break;
			case bson_data_type::d64_t:
				double_serial(buf,x);
				
				break;
			case bson_data_type::bool_t:
				bool_serial(buf,x);
				break;
			case bson_data_type::str:
				str_serial(buf, x);
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
