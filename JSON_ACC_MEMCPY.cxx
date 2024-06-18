#include "JSON_Base.h"
#include <thread>
#pragma once
#ifndef __ACC_MEMCPY_CXX_
#define __ACC_MEMCPY_CXX_
inline int Expect_Sign(std::string data, char sign, int startpos) {
	int i = 0;
	for (i = startpos; i < (int)data.length() && data[i] != sign; i++);
	return i;
}
using json_info = std::vector<std::pair<int, int>>;
//改为预先读取 再memcpy（现在是memmove）
int JSON_Parse_Memcpy_Pool(JSON_POOL & map, JSON_ACC&root, std::string& data) {
	JSON_ACC buf;
	json_info info;
	int j = 0;
	int k = 0;
	int t = 0;
	for (int i = 0; i < (int)(data.length() - 1); ++i) {
		//std::cout << data[i];
		if (data[i] == ConS) {
			///title

			//std::cout << t << "," << i << "\n";
			if (data[i + 1] == 'n' && data[i + 2] == 'u' && data[i + 3] == 'l' && data[i + 4] == 'l') {
				info.emplace_back(std::make_pair(i + 1, i + 4));
			}
			if (data[i + 1] == 't' && data[i + 2] == 'r' && data[i + 3] == 'u' && data[i + 4] == 'e') {//bool true
				info.emplace_back(std::make_pair(i + 1, i + 4));
			}
			if (data[i + 1] == 'f' && data[i + 2] == 'a' && data[i + 3] == 'l' && data[i + 4] == 's' && data[i + 5] == 'e') { //bool false
				info.emplace_back(std::make_pair(i + 1, i + 5));
			}
			if (data[i + 1] == Quote) {//str
				j = Expect_Sign(data, Quote, i + 2);
			}
			if (std::isdigit(data[i + 1]) || (data[i + 1] == '-' && std::isdigit(data[i + 2]))) {//digit_all

			}
			/////////////////////////////////////////////////////////////////////////////////
			if (data[i + 1] == FieldS && data[i + 2] == FieldE) {//void list

			}

			if (data[i + 1] == FieldS) { //Dimension List

			}

			/////////////////////////////////////////////////////////////////////////////
			if (data[i + 1] == LayerS) { //pair_list {}   Object
				j = Expect_Sign(data, LayerE, i + 2);

			}

		}
	}
	return 1;
}



#endif
