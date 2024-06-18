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

struct json_info {
	std::vector<std::pair<int, int>> ppos;
	int type{notype};
	int pair_layer{0};
	int dimension_layer{0};
};

//改为预先读取 再memcpy（现在是memmove）
int JSON_PreRead_Memcpy_Pool( std::string& data, json_info& info_x) {
	JSON_ACC buf;
	json_info info;
	int j = 0;
	for (int i = 0; i < (int)(data.length() - 1); ++i) {
		//std::cout << data[i];
		if (data[i] == ConS) {
			///title

			//std::cout << t << "," << i << "\n";
			if (data[i + 1] == 'n' && data[i + 2] == 'u' && data[i + 3] == 'l' && data[i + 4] == 'l') {
				info.ppos.emplace_back(std::make_pair(i + 1, i + 4));
			}
			if (data[i + 1] == 't' && data[i + 2] == 'r' && data[i + 3] == 'u' && data[i + 4] == 'e') {//bool true
				info.ppos.emplace_back(std::make_pair(i + 1, i + 4));
			}
			if (data[i + 1] == 'f' && data[i + 2] == 'a' && data[i + 3] == 'l' && data[i + 4] == 's' && data[i + 5] == 'e') { //bool false
				info.ppos.emplace_back(std::make_pair(i + 1, i + 5));
			}
			if (data[i + 1] == Quote) {//str
				j = Expect_Sign(data, Quote, i + 2);
				info.ppos.emplace_back(std::make_pair(i + 1, j));
			}
			if (std::isdigit(data[i + 1]) || (data[i + 1] == '-' && std::isdigit(data[i + 2]))) {//digit_all
				j = Expect_Sign(data, ConE, i + 1);
				info.ppos.emplace_back(std::make_pair(i + 1, j));
			}
			/////////////////////////////////////////////////////////////////////////////////
			if (data[i + 1] == FieldS && data[i + 2] == FieldE) {//void list
				info.ppos.emplace_back(std::make_pair(i + 1, 2));
			}

			if (data[i + 1] == FieldS) { //Dimension List
				j = Expect_Sign(data, FieldE, i + 1);
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
