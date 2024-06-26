#include "JSON_Base.h"
#include <thread>
#pragma once
#ifndef __ACC_POOL_CXX_
#define __ACC_POOL_CXX_
int PairList_Expect_Pool(std::string& data, json_pool&map, int current_root_idx, int beginpos = 0) {
	json_acc buf;
	int j = 0;
	int k = 0;
	int i = 0;
	int t = 0;
	int layer = 0;
	std::string fragment;
	if (data[beginpos] == LayerS) {
		//std::cout << "++\n";
		++layer;
	}
	for (i = beginpos + 1; layer != 0; i++) {
		//std::cout << data[i];
		if (data[i] == LayerS) {
			//std::cout << "++\n";
			++layer;
		}
		//std::cout << layer << "\n";
		if (data[i] == LayerE) {
			//std::cout << "--\n";
			--layer;
		}
		if (data[i] == ConS) {
			buf.Father_idx = current_root_idx;
			for (t = i; data[t] != ConE && data[t] != LayerS; t--);
			for (k = t; data[k] != ConS; k++) {
				if (data[k] != LayerS && data[k] != FieldS && data[k] != ConE) {
					buf.title.push_back(data[k]);
				}
			}
			if (data[i + 1] == 'n' && data[i + 2] == 'u' && data[i + 3] == 'l' && data[i + 4] == 'l') {
				buf.content = "null";
				buf.type = null;

				map.emplace_back(buf);
				map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

				buf.clear();
			}
			if (data[i + 1] == 't' && data[i + 2] == 'r' && data[i + 3] == 'u' && data[i + 4] == 'e') {//bool true
				buf.content = true;
				buf.type = bool_t;

				map.emplace_back(buf);
				map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

				buf.clear();
			}
			if (data[i + 1] == 'f' && data[i + 2] == 'a' && data[i + 3] == 'l' && data[i + 4] == 's' && data[i + 5] == 'e') { //bool false
				buf.content = false;
				buf.type = bool_t;

				map.emplace_back(buf);
				map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

				buf.clear();
			}
			if (data[i + 1] == Quote) {//str
				//std::cout << "STR" << "\n";
				buf.type = str;
				for (j = i + 1; data[j] != ConE && data[j] != LayerE; j++) {
					fragment.push_back(data[j]);
				}
				buf.content = fragment;

				map.emplace_back(buf);
				map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

				buf.clear();
				//std::cout << data[j] << "  ssssssss\n";
				i = j - 1;
				j = 0;
			}
			if (std::isdigit(data[i + 1]) || (data[i + 1] == '-' && std::isdigit(data[i + 2]))) {//digit_all
				//std::cout << "DIGIT" << "\n";
				buf.type = digit_int;
				for (j = i + 1; data[j] != ConE && data[j] != LayerE; j++) {
					if (data[j] == '.') {
						buf.type = digit_double;
					}
					fragment.push_back(data[j]);
				}
				if (buf.type == digit_double) {
					buf.content = std::stod(fragment);
				}
				if (buf.type == digit_int) {
					buf.content = std::stoll(fragment);
				}
				map.emplace_back(buf);
				map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

				buf.clear();
				i = j - 1;
				j = 0;
			}
			//////////////////////////////////////////////////////////////////
			if (data[i + 1] == FieldS && data[i + 2] == FieldE) {//void list
				buf.type = dimension_void;
				buf.content = "[]";
				map.emplace_back(buf);
				map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

				buf.clear();
				i = i + 2;
			}
			if (data[i + 1] == LayerS && data[i + 2] == LayerE) { //pair_list_VOID {}   Object
				buf.type = pair_list_void;

				buf.content = "{}";
				map.emplace_back(buf);
				map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

				buf.clear();
				i = i + 2;
			}
			///////////////////////
			if (data[i + 1] == FieldS) { //Dimension List
				buf.type = dimension_list;
#define Pair_Dimen
				buf.content = std::monostate {};
				map.emplace_back(buf);
				map[current_root_idx].Child_idx.emplace_back(map.size() - 1);
				j = DimensionArray_Expect_Pool(data, map, map.size() - 1, i + 1);

				buf.clear();
				i = j;
			}
			/////////////////////////////////////////////////////////////////
			/*
			if (data[i + 1] == FieldS && data[i + 2] == LayerS) {//field [] Array
			//std::cout << "FIELD" << "\n";
			//for (j = i; data[j] != FieldE; ++j) {
			//}
			buf.type = dimension_list;

			j = Field_Expect(data, buf, i + 1);
			currentm.Child.emplace_back(buf);
			buf.clear();
			i = j;
			j = 0;
			//i = field_endpos + 1;
			}
			*/

			if (data[i + 1] == LayerS) {//pair_list {}   Object
				buf.type = pair_list;
#define Pair_Pair
				buf.content = std::monostate {};
				map.emplace_back(buf);
				map[current_root_idx].Child_idx.emplace_back(map.size() - 1);
				j = PairList_Expect_Pool(data, map, map.size() - 1, i + 1);

				buf.clear();
				i = j;
				//std::cout << "RealPair:" << data[i] << "\n";
				j = 0;
			}
			//if (data[i + 1] == LayerS && data[i + 2] == LayerE) {//pair_list {}   Object
			///	buf.type = pair_list_void;
			//	currentm.Child.emplace_back(buf);
			//	buf.clear();
			//	i = i + 2;
			//}
			fragment.clear();
		}
	}
	return i;
}

int DimensionArray_Expect_Pool(std::string& data, json_pool& map, int current_root_idx, int beginpos = 0) {
	//std::cout << "IN_FUNC\n";
	json_acc buf;
	int j = 0;
	int i = 0;
	std::string fragment;
	//if (data[beginpos] == FieldS) {
	//	field++;
	//}
	//想法1 嵌套 跳过那些多余的]
	//想法2 双vector，老老实实读
	for (i = beginpos; data[i] != FieldE; ++i) {
		if (data[i] == FieldS || data[i] == ConE) {
			buf.Father_idx = current_root_idx;
			if (data[i + 1] == 'n' && data[i + 2] == 'u' && data[i + 3] == 'l' && data[i + 4] == 'l') {
				buf.content = "null";
				buf.type = null;

				map.emplace_back(buf);
				map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

				buf.clear();
				i + 4;
			}
			if (data[i + 1] == 't' && data[i + 2] == 'r' && data[i + 3] == 'u' && data[i + 4] == 'e') {//bool true
				buf.content = true;
				buf.type = bool_t;

				map.emplace_back(buf);
				map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

				buf.clear();
				i + 4;
			}
			if (data[i + 1] == 'f' && data[i + 2] == 'a' && data[i + 3] == 'l' && data[i + 4] == 's' && data[i + 5] == 'e') { //bool false
				buf.content = false;
				buf.type = bool_t;

				map.emplace_back(buf);
				map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

				buf.clear();
				i + 5;
			}
			if (data[i + 1] == Quote) {//str
				//std::cout << "STR" << "\n";
				buf.type = str;
				//std::cout << data[i];
				for (j = i + 1; data[j] != ConE && data[j] != LayerE; j++) {
					fragment.push_back(data[j]);

				}

				buf.content = fragment;


				//std::cout << data[i];
				map.emplace_back(buf);
				map[current_root_idx].Child_idx.emplace_back(map.size() - 1);


				buf.clear();
				//std::cout << data[j] << "  ssssssss\n";
				i = j - 1;
				j = 0;
			}
			if (std::isdigit(data[i + 1]) || (data[i + 1] == '-' && std::isdigit(data[i + 2]))) {//digit_all
				//std::cout << "DIGIT" << "\n";
				/////////////////////////////////////////////////////////////
				buf.type = digit_int;
				for (j = i + 1; data[j] != ConE && data[j] != FieldE; j++) {
					if (data[j] == '.') {
						buf.type = digit_double;
					}
					fragment.push_back(data[j]);
				}
//#pragma test
				//std::cout << fragment << std::stoi(fragment) << "\n";

				if (buf.type == digit_double) {
					buf.content = std::stod(fragment);
				}
				if (buf.type == digit_int) {

					buf.content = std::stoll(fragment);
				}

				map.emplace_back(buf);
				map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

				buf.clear();
				i = j - 1;
				j = 0;
			}

			/////////////////////////////////////////////////////////////////////////////////
			if (data[i + 1] == FieldS && data[i + 2] == FieldE) {//void list
				buf.type = dimension_void;
				buf.content = "[]";
				map.emplace_back(buf);
				map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

				buf.clear();
				//i = i + 2;
				i = i + 2;
				//std::cout << data[i - 1] << "("  << data[i] << ")" << "   Void\n";
			}

			if (data[i + 1] == FieldS) { //Dimension List
				buf.type = dimension_list;
#define Dimension_Dimen
				buf.content = std::monostate {};
				map.emplace_back(buf);
				map[current_root_idx].Child_idx.emplace_back(map.size() - 1);
				j = DimensionArray_Expect_Pool(data, map, map.size() - 1, i + 1);

				buf.clear();
				//i = j;
				i = j;
				//std::cout << data[i - 1] << "("  << data[i] << ")" << "   NonVoid\n";
			}

			/////////////////////////////////////////////////////////////////////////////

			if (data[i + 1] == LayerS && data[i + 2] == LayerE) { //pair_list_VOID {}   Object
				buf.type = pair_list_void;

				buf.content = "{}";
				map.emplace_back(buf);
				map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

				buf.clear();
				i = i + 2;
			}

			if (data[i + 1] == LayerS) { //pair_list {}   Object///////////////////
				buf.type = pair_list;
#define Dimension_Pair
				buf.content = std::monostate {};
				map.emplace_back(buf);
				map[current_root_idx].Child_idx.emplace_back(map.size() - 1);
				j = PairList_Expect_Pool(data, map, map.size() - 1, i + 1);

				buf.clear();

				i = j - 1;
				//std::cout << "PAIR_LIST:" << data[i] << "\n";
				j = 0;
			}
			fragment.clear();
		}

	}

	//std::cout << "Outa!:" << i << "\n";
	if (!std::get_if<i64t>(&buf.content) || !std::get_if<std::string>(&buf.content) || !std::get_if<bool>(&buf.content) || !std::get_if<double>(&buf.content)) {
		map.emplace_back(buf);
		buf.clear();
	}//本来是应对']'的，但是会多出来

	//std::cout << field << "\n";
	return i;
}

int JSON_Parse_Pool(json_pool & map, std::string& data) {
	json_acc buf;
	int j = 0;
	int k = 0;
	int t = 0;
	std::string fragment;
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
			buf.Father_idx = 0;
			if (data[i + 1] == 'n' && data[i + 2] == 'u' && data[i + 3] == 'l' && data[i + 4] == 'l') {
				buf.content = "null";
				buf.type = null;

				map.emplace_back(buf);
				buf.clear();
			}
			if (data[i + 1] == 't' && data[i + 2] == 'r' && data[i + 3] == 'u' && data[i + 4] == 'e') {//bool true
				buf.content = true;
				buf.type = bool_t;
				map.emplace_back(buf);
				buf.clear();
			}
			if (data[i + 1] == 'f' && data[i + 2] == 'a' && data[i + 3] == 'l' && data[i + 4] == 's' && data[i + 5] == 'e') { //bool false
				buf.content = false;
				buf.type = bool_t;
				map.emplace_back(buf);
				buf.clear();
			}
			if (data[i + 1] == Quote) {//str
				//std::cout << "STR" << "\n";
				buf.type = str;

				for (j = i + 1; data[j] != ConE && data[j] != LayerE; j++) {
					fragment.push_back(data[j]);
				}


				buf.content = fragment;
				map.emplace_back(buf);
				buf.clear();
				//std::cout << data[j] << "  ssssssss\n";
				i = j - 1;
				j = 0;
			}
			if (std::isdigit(data[i + 1]) || (data[i + 1] == '-' && std::isdigit(data[i + 2]))) {//digit_all
				//std::cout << "DIGIT" << "\n";
				buf.type = digit_int;
				//std::cout << data[i] << data[i + 1] << data[i + 2] << "\n";
				for (j = i + 1; data[j] != ConE && data[j] != LayerE; j++) {
					if (data[j] == '.') {
						buf.type = digit_double;
					}
					fragment.push_back(data[j]);
				}


				if (buf.type == digit_double) {
					buf.content = std::stod(fragment);
				}
				if (buf.type == digit_int) {
					buf.content = std::stoll(fragment);
				}



				map.emplace_back(buf);
				buf.clear();
				i = j - 1;
				j = 0;
			}
			/////////////////////////////////////////////////////////////////////////////////
			if (data[i + 1] == FieldS && data[i + 2] == FieldE) {//void list
				buf.type = dimension_void;
				buf.content = "[]";

				map.emplace_back(buf);
				buf.clear();
				i = i + 2;
			}
			if (data[i + 1] == LayerS && data[i + 2] == LayerE) {//void list
				buf.type = pair_list_void;
				buf.content = "{}";

				map.emplace_back(buf);
				buf.clear();
				i = i + 2;
			}

			if (data[i + 1] == FieldS) { //Dimension List
				buf.type = dimension_list;
#define Main_Dimension
				buf.content = std::monostate {};
				map.emplace_back(buf);
				j = DimensionArray_Expect_Pool(data, map, map.size() - 1, i + 1);
				//std::cout << "Dimen:" << data[i + 1] << "\n";

				buf.clear();
				i = j;
			}

			/////////////////////////////////////////////////////////////////////////////
			if (data[i + 1] == LayerS) { //pair_list {}   Object
				buf.type = pair_list;


#define Main_Pair
				buf.content = std::monostate {};
				map.emplace_back(buf);
				j = PairList_Expect_Pool(data, map, map.size() - 1, i + 1);

				buf.clear();
				i = j;
				j = 0;
			}
			fragment.clear();
		}
	}
	return 1;
}

#endif
