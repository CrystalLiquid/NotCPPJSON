#include <iostream>
#include "json.hpp"
#include "json_io.hpp"
#include "bson.hpp"
#include <chrono>

#pragma once
#ifndef __MAIN_
#define __MAIN_
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
#define STR(x) case x: return #x; break;
std::string penum(int e) {
	switch (e) {
			STR(json_acc_layer_np::temp_digit)
			STR(json_acc_layer_np::str)
			STR(json_acc_layer_np::digit_int)
			STR(json_acc_layer_np::null)
			STR(json_acc_layer_np::digit_double)
			STR(json_acc_layer_np::pair_list)
			STR(json_acc_layer_np::pair_list_void)
			STR(json_acc_layer_np::bool_t)
			STR(json_acc_layer_np::dimension_void)
			STR(json_acc_layer_np::dimension_list)
			STR(json_acc_layer_np::error)
			STR(json_acc_layer_np::notype)
			STR(json_acc_layer_np::invalid_opttype)
		default:
			//TODO
			break;
	}
	return "------------------";
}
int main() {
	//const char* path = "5.json";
	const char* path = "citm_catalog.json";
	//const char* path = "3.json";
	//const char* path = "4.json";
	//const char* path = "canada.json";
	//const char* path = "twitter.json";
	//const char* path = "2.json";
	std::string data;
	HP_IO::JSON_IO_FILE FILE;
	json_map map;
	json root;
	map.emplace_back(root);
	int path_idx = 0;

	FILE.READ(path, data);
	//std::cout << data << "\n\n\n\n";
	high_resolution_clock::time_point Stime = high_resolution_clock::now();
	map.parse(data);
	high_resolution_clock::time_point Etime = high_resolution_clock::now();
	std::cout << "JSONParse Finish\n";
	milliseconds Parse_Interval = std::chrono::duration_cast<milliseconds>(Etime - Stime);

#ifdef PRINT_MAP
	for (int i = (int)map.size() - 1; i > 0; i--) {
		std::cout << map.at(map.at(i).Father_idx).title << "|" << map.at(i).title << ":" << map.at(i).content << "       " << map.at(i).layer << "\n";
	}
#endif
	std::cout << "ParseTime:" << Parse_Interval.count() << "ms" << std::endl;


	std::cout << "DataSize:" << data.size() << "\n";
	std::cout << "MapSize:" << map.size() << "\n";
	LP_BASIC_IO::IO_Write(map.serialize(), "1.json");

	return 0;
}
#endif
