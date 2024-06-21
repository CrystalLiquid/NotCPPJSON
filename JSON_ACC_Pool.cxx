#include "JSON_Base.h"
#include <thread>
#pragma once
#ifndef __ACC_POOL_CXX_
#define __ACC_POOL_CXX_
/*
int PairList_Expect_Pool(std::string& data, JSON_POOL&map, int current_root_idx, int beginpos = 0) {

return i;
}

int DimensionArray_Expect_Pool(std::string& data, JSON_POOL& map, int current_root_idx, int beginpos = 0) {

return i;
}
*/


int JSON_Parse_Pool(JSON_POOL & map, std::string& data) {
	JSON_ACC buf;
	int j = 0;
	int k = 0;
	int t = 0;
	for (int i = 0; i < (int)(data.length() - 1); ++i) {
		//std::cout << data[i];
		if (data[i] == ConS) {
			///title
			for (t = i; data[t] != ConE && data[t] != LayerS; t--) {
			}
			for (k = t + 1; data[k] != ConS; k++) {
				buf.title.push_back(data[k]);
			}
			//std::cout << t << "," << i << "\n";
			if (data[i + 1] == 'n' && data[i + 2] == 'u' && data[i + 3] == 'l' && data[i + 4] == 'l') {
				buf.content = "null";
				buf.type = null;
				map.emplace_back(buf);
				buf.clear();
			}
			if (data[i + 1] == 't' && data[i + 2] == 'r' && data[i + 3] == 'u' && data[i + 4] == 'e') {//bool true
				buf.content = "true";
				buf.type = bool_t;
				map.emplace_back(buf);
				buf.clear();
			}
			if (data[i + 1] == 'f' && data[i + 2] == 'a' && data[i + 3] == 'l' && data[i + 4] == 's' && data[i + 5] == 'e') { //bool false
				buf.content = "false";
				buf.type = bool_t;
				map.emplace_back(buf);
				buf.clear();
			}
			if (data[i + 1] == Quote) {//str
				//std::cout << "STR" << "\n";
				buf.type = str;

				for (j = i + 1; data[j] != ConE && data[j] != LayerE; j++) {
					buf.content.push_back(data[j]);
				}

				map.emplace_back(buf);
				buf.clear();
				//std::cout << data[j] << "  ssssssss\n";
				i = j - 1;
			}
			if (std::isdigit(data[i + 1]) || (data[i + 1] == '-' && std::isdigit(data[i + 2]))) {//digit_all
				//std::cout << "DIGIT" << "\n";
				buf.type = digit_int;
				for (j = i + 1; data[j] != ConE && data[j] != LayerE; j++) {
					if (data[j] == '.') {
						buf.type = digit_double;
					}
					buf.content.push_back(data[j]);
				}

				map.emplace_back(buf);
				buf.clear();
				i = j - 1;
			}
			/////////////////////////////////////////////////////////////////////////////////
			if (data[i + 1] == FieldS && data[i + 2] == FieldE) {//void list
				buf.type = dimension_void;
				map.emplace_back(buf);
				buf.clear();
				i = i + 2;
			}

			if (data[i + 1] == FieldS) { //Dimension List
				buf.type = dimension_list;
#pragma Field
				map.emplace_back(buf);
				j = DimensionArray_Expect_Pool(data, map, map.size() - 1, i + 1);
				//std::cout << "Dimen:" << data[i + 1] << "\n";
				buf.clear();
				i = j;
			}

			/////////////////////////////////////////////////////////////////////////////
			if (data[i + 1] == LayerS) { //pair_list {}   Object
				buf.type = pair_list;
#pragma Pair
				map.emplace_back(buf);
				j = PairList_Expect_Pool(data, map, map.size() - 1, i + 1);
				buf.clear();
				i = j;
			}

		}
	}
	return 1;
}
#endif
