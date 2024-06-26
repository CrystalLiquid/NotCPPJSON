#include <iostream>
#include "JSON_Base.h"
#include "JSON_ACC_Pool_Str.hpp"
#include "JSON_IO.h"
#include <chrono>
#pragma once
#ifndef __MAIN_
#define __MAIN_
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

int main() {
	//const char* path = "citm_catalog.json";
	//const char* path = "canada.json";
	//const char* path = "twitter.json";
	const char* path = "2.json";
	std::string data;
	HP_IO::JSON_IO_FILE FILE;
	json_acc_str_np::json_pool_str map;
	json_acc_str_np::json_acc_str root;
	map.emplace_back(root);
	/*
	map.back().title = "Hello World";
	JSON_ACC hh("CCC", "SSSZ", notype, nullptr);n
	std::cout << map.back().title << "\n";
	map.emplace_back(hh);
	std::cout << map.back().title << map.back().content << "\n";
	*/

	FILE.READ(path, data);
	//std::cout << data << "\n\n\n\n";
	high_resolution_clock::time_point Stime = high_resolution_clock::now();
	//JSON_Parse(map, data);
	JSON_Parse_Pool(map, data);
	//std::cout << "Result" << map.back().title;
	//try {
	//} catch (std::bad_alloc) {
	//	std::cout << "ERROR\n";
	//}
	high_resolution_clock::time_point Etime = high_resolution_clock::now();
	std::cout << "JSONParse Finish\n";
	milliseconds Parse_Interval = std::chrono::duration_cast<milliseconds>(Etime - Stime);


	for (int i = 0; i < (int)map.size(); i++) {
		std::cout << "Father:" << map.at(map.at(i).Father_idx).title << "|" << map.at(i).title << ":" << map.at(i).content << "\n";
	}
	std::cout << "ParseTime:" << Parse_Interval.count() << "ms" << std::endl;

	//SeeChild(rt);
	//IsValidMap_VectorP(rt, 0);
	/*
	  json_acc::JSON_MAP_POOL mapp;

	  high_resolution_clock::time_point PStime = high_resolution_clock::now();
	  //JSON_Parse(map, data);
	  json_acc::poolize::JSON_Parse(,mapp,data);
	  high_resolution_clock::time_point PEtime = high_resolution_clock::now();
	  milliseconds Parse_IntervalP = std::chrono::duration_cast<milliseconds>(PEtime - PStime);
	 */


	std::cout << "DataSize:" << data.size() << "\n";
	std::cout << "MapSize:" << map.size() << "\n";
	//std::cout << "ParseTime:" << Parse_Interval.count() << "ms" << "\n";
	return 0;
}
#endif
