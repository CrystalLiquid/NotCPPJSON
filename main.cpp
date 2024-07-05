#include <iostream>
#include "JSON_ACC_Pool_Str.hpp"
#include "JSON_IO.hpp"
#include <chrono>
#pragma once
#ifndef __MAIN_
#define __MAIN_
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

int main() {
	//const char* paths[] = {"citm_catalog.json", "2.json", "3.json", "4.json"};
	//const char* path = "citm_catalog.json";
	const char* path = "3.json";
	//const char* path = "4.json";
	//const char* path = "twitter.json";
	//const char* path = "2.json";
	std::string data;
	HP_IO::JSON_IO_FILE FILE;
	json_acc_str_np::json_pool_str map;
	json_acc_str_np::json_acc_str root;
	map.emplace_back(root);
	int path_idx = 0;

	FILE.READ(path, data);
	//std::cout << data << "\n\n\n\n";
	high_resolution_clock::time_point Stime = high_resolution_clock::now();
	json_acc_str_np::JSON_Parse_Pool(map, std::move(data));
	high_resolution_clock::time_point Etime = high_resolution_clock::now();



	std::cout << "JSONParse Finish\n";
	milliseconds Parse_Interval = std::chrono::duration_cast<milliseconds>(Etime - Stime);


	//for (int i = (int)map.size() - 1; i > 0; i--) {
	//	std::cout << "Father:" << map.at(map.at(i).Father_idx).title << "|" << map.at(i).title << ":" << map.at(i).content << "\n";
	//}
	std::cout << "ParseTime:" << Parse_Interval.count() << "ms" << std::endl;


	std::cout << "DataSize:" << data.size() << "\n";
	std::cout << "MapSize:" << map.size() << "\n";
	std::string result;
	map.setval_by_name("site", "nomorejson");
	std::cout << map.getval_by_name("site").content << "\n";
	json_acc_str_np::JSON_Serialize_Pool(map, result, 0);
	//std::cout << result;
	LP_BASIC_IO::IO_Write(&result, "1.json");

	//std::cout << "ParseTime:" << Parse_Interval.count() << "ms" << "\n";
	return 0;
}
#endif
