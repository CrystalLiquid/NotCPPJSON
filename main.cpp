#include <iostream>
#include "JSON_Base.h"
#include "JSON_IO.h"
#include <chrono>
#pragma once
#ifndef __MAIN_
#define __MAIN_
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;


void IsValidMap_VectorP(JSON_ACC& map, int Counter) {
	for (int i = 0; i != (int)map.Child.size(); i++) {
		for (int t = 0; t <= Counter; t++) {
			std::cout << "    ";
		}
		JSON_ACC& child = *(map.Child.at(i));
		//map.Child.at(i).type << " | " <<
		std::cout << child.title << " : " << child.content << "    " << Counter << "\n";
		//if (map.Child.at(i).title.empty()) {
		//	std::cerr << "ERROR 1 Size:";
		//	std::cerr << (int)map.Child.size() << "\n";
		//}
		IsValidMap_VectorP(child, Counter + 1);
	}

}

void IsValidMap_Vector(JSON& map, int Counter) {
	for (int i = 0; i != (int)map.Child.size(); i++) {
		for (int t = 0; t <= Counter; t++) {
			std::cout << "    ";
		}
		//map.Child.at(i).type << " | " <<
		std::cout << map.Child.at(i).title << " : " << map.Child.at(i).content << "    " << Counter << "\n";
		//if (map.Child.at(i).title.empty()) {
		//	std::cerr << "ERROR 1 Size:";
		//	std::cerr << (int)map.Child.size() << "\n";
		//}
		IsValidMap_Vector(map.Child.at(i), Counter + 1);
	}

}

int count_map(JSON& map, int&tmp) {
	tmp += map.Child.size();
	for (int i = 0; i < (int)map.Child.size(); i++) {
		tmp += count_map(map.Child.at(i), tmp);
	}
	return tmp;
}

/*
  void IsValidMap_List(JSON& map, int Counter) {
  for (std::list<JSON>::iterator i = map.Child.begin(); i != map.Child.end(); i++) {
  for (int t = 0; t <= Counter; t++) {
  std::cout << "    ";
  }
  std::cout << (*i).title << " : " << (*i).content << "    " << Counter << "\n";
  //if (map.Child.at(i).title.empty()) {
  //	std::cerr << "ERROR 1 Size:";
  //	std::cerr << (int)map.Child.size() << "\n";
  //}

  IsValidMap((*i), Counter + 1);

  }

  }
 */

int main() {
	//const char* path = "citm_catalog.json";
	//const char* path = "canada.json";
	//const char* path = "twitter.json";
	const char* path = "2.json";
	std::string data;
	HP_IO::JSON_IO_FILE FILE;
	JSON_POOL map;
	JSON_ACC buff;
	map.emplace_back(buff);
	FILE.READ(path, data);
	//std::cout << data << "\n\n\n\n";
	high_resolution_clock::time_point Stime = high_resolution_clock::now();
	//JSON_Parse(map, data);
	JSON_Parse_Pool(map, data);
	high_resolution_clock::time_point Etime = high_resolution_clock::now();
	std::cout << "JSONParse Finish\n";
	milliseconds Parse_Interval = std::chrono::duration_cast<milliseconds>(Etime - Stime);

	//for (int i = 0; i < (int)map.size(); i++) {
	//	std::cout << "title:" << map.at(i).title << "         content:" << map.at(i).content << "\n";
	//}
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
	std::cout << "ParseTime:" << Parse_Interval.count() << "ms" << "\n";
	return 0;
}
#endif
