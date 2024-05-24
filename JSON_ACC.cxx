#include "JSON_Base.h"
#include <thread>
#pragma once
#ifndef __ACC_CXX_
#define __ACC_CXX_

int PairList_Expect_Pool(std::string& data, JSON_POOL&map, JSON_ACC& current_root, int beginpos = 0) {
	JSON_ACC buf;
	int j = 0;
	int k = 0;
	int i = 0;
	int t = 0;
	int layer = 0;
	if (data[beginpos] == LayerS) {
		//std::cout << "++\n";
		layer++;
	}
	for (i = beginpos + 1; layer != 0; i++) {
		std::cout << "IP:" << data[i];
		if (data[i] == LayerS) {
			//std::cout << "++\n";
			layer++;
		}
		//std::cout << layer << "\n";
		if (data[i] == LayerE) {
			//std::cout << "--\n";
			layer--;
		}
		if (data[i] == ConS) {
			for (t = i; data[t] != ConE && data[t] != LayerS; t--);
			for (k = t; data[k] != ConS; k++) {
				if (data[k] != LayerS && data[k] != FieldS && data[k] != ConE) {
					buf.title.push_back(data[k]);
				}

			}
			if (data[i + 1] == 'n' && data[i + 2] == 'u' && data[i + 3] == 'l' && data[i + 4] == 'l') {
				buf.content = "null";
				buf.Father = &current_root;
				map.emplace_back(buf);
				current_root.Child.emplace_back(&(map.back()));
				buf.clear();
			}
			if (data[i + 1] == 't' && data[i + 2] == 'r' && data[i + 3] == 'u' && data[i + 4] == 'e') {//bool true
				buf.content = "true";
				buf.Father = &current_root;
				map.emplace_back(buf);
				current_root.Child.emplace_back(&(map.back()));
				buf.clear();
			}
			if (data[i + 1] == 'f' && data[i + 2] == 'a' && data[i + 3] == 'l' && data[i + 4] == 's' && data[i + 5] == 'e') { //bool false
				buf.content = "false";
				buf.Father = &current_root;
				map.emplace_back(buf);
				current_root.Child.emplace_back(&(map.back()));
				buf.clear();
			}
			if (data[i + 1] == Quote) {//str
				//std::cout << "STR" << "\n";
				buf.type = str;

				for (j = i + 1; data[j] != ConE && data[j] != LayerE; j++) {
					buf.content.push_back(data[j]);
				}
				buf.Father = &current_root;
				map.emplace_back(buf);
				current_root.Child.emplace_back(&(map.back()));
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
					buf.content.push_back(data[j]);
				}
				buf.Father = &current_root;
				map.emplace_back(buf);
				current_root.Child.emplace_back(&(map.back()));
				buf.clear();
				i = j - 1;
				j = 0;
			}
			//////////////////////////////////////////////////////////////////
			if (data[i + 1] == FieldS && data[i + 2] == FieldE) {//void list
				buf.Father = &current_root;
				map.emplace_back(buf);
				current_root.Child.emplace_back(&(map.back()));
				buf.clear();
				i = i + 2;
			}
			///////////////////////
			if (data[i + 1] == FieldS) { //Dimension List
				buf.type = dimension_list;
				buf.Father = &current_root;
				map.emplace_back(buf);
				j = DimensionArray_Expect_Pool(data, map, map.back(), i + 1);
				current_root.Child.emplace_back(&(map.back()));
				buf.clear();
				i = j;
			}
			/////////////////////////////////////////////////////////////////


			if (data[i + 1] == LayerS) {//pair_list {}   Object
				buf.type = pair_list;
				buf.Father = &current_root;
				map.emplace_back(buf);
				j = PairList_Expect_Pool(data, map, map.back(), i + 1);
				current_root.Child.emplace_back(&(map.back()));
				buf.clear();
				i = j;
				//std::cout << "RealPair:" << data[i] << "\n";
				j = 0;
			}
		}
	}
	return i;
}

int DimensionArray_Expect_Pool(std::string& data, JSON_POOL&map, JSON_ACC& current_root, int beginpos = 0) {
	//std::cout << "IN_FUNC\n";
	JSON_ACC buf;
	int j = 0;
	int i = 0;
	//if (data[beginpos] == FieldS) {
	//	field++;
	//}
	//想法1 嵌套 跳过那些多余的]
	//想法2 双vector，老老实实读
	for (i = beginpos; data[i] != FieldE; i++) {
		if (data[i] == FieldS || data[i] == ConE) {

			if (data[i + 1] == 'n' && data[i + 2] == 'u' && data[i + 3] == 'l' && data[i + 4] == 'l') {
				buf.content = "null";
				buf.Father = &current_root;
				map.emplace_back(buf);
				current_root.Child.emplace_back(&(map.back()));
				buf.clear();
			}
			if (data[i + 1] == 't' && data[i + 2] == 'r' && data[i + 3] == 'u' && data[i + 4] == 'e') {//bool true
				buf.content = "true";
				buf.Father = &current_root;
				map.emplace_back(buf);
				current_root.Child.emplace_back(&(map.back()));
				buf.clear();
			}
			if (data[i + 1] == 'f' && data[i + 2] == 'a' && data[i + 3] == 'l' && data[i + 4] == 's' && data[i + 5] == 'e') { //bool false
				buf.content = "false";
				buf.Father = &current_root;
				map.emplace_back(buf);
				current_root.Child.emplace_back(&(map.back()));
				buf.clear();
			}
			if (data[i + 1] == Quote) {//str
				//std::cout << "STR" << "\n";
				buf.type = str;
				//std::cout << data[i];
				for (j = i + 1; data[j] != ConE && data[j] != LayerE; j++) {
					buf.content.push_back(data[j]);

				}
				//std::cout << data[i];
				buf.Father = &current_root;
				map.emplace_back(buf);
				current_root.Child.emplace_back(&(map.back()));
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
					buf.content.push_back(data[j]);
				}
				buf.Father = &current_root;
				map.emplace_back(buf);
				current_root.Child.emplace_back(&(map.back()));
				buf.clear();
				i = j - 1;
				j = 0;
			}

			/////////////////////////////////////////////////////////////////////////////////
			if (data[i + 1] == FieldS && data[i + 2] == FieldE) {//void list
				buf.type = dimension_void;
				buf.Father = &current_root;

				map.emplace_back(buf);
				current_root.Child.emplace_back(&(map.back()));
				buf.clear();
				//i = i + 2;
				i = i + 2;
				//std::cout << data[i - 1] << "("  << data[i] << ")" << "   Void\n";
			}

			if (data[i + 1] == FieldS) { //Dimension List
				buf.type = dimension_list;
				buf.Father = &current_root;
				map.emplace_back(buf);
				j = DimensionArray_Expect_Pool(data, map, map.back(), i + 1);
				current_root.Child.emplace_back(&(map.back()));
				//i = j;
				i = j;
				//std::cout << data[i - 1] << "("  << data[i] << ")" << "   NonVoid\n";
			}

			/////////////////////////////////////////////////////////////////////////////

			if (data[i + 1] == LayerS && data[i + 2] == LayerE) { //pair_list_VOID {}   Object
				buf.type = pair_list_void;
				buf.Father = &current_root;
				map.emplace_back(buf);
				current_root.Child.emplace_back(&(map.back()));
				buf.clear();
				i = i + 2;
			}

			if (data[i + 1] == LayerS) { //pair_list {}   Object///////////////////
				buf.type = pair_list;
				buf.Father = &current_root;
				map.emplace_back(buf);
				j = PairList_Expect_Pool(data, map, map.back(), i + 1);
				current_root.Child.emplace_back(&(map.back()));

				buf.clear();

				i = j - 1;
				//std::cout << "PAIR_LIST:" << data[i] << "\n";
				j = 0;
			}

		}

	}
	//std::cout << "Outa!:" << i << "\n";
	if (!buf.content.empty()) {
		buf.Father = &current_root;
		map.emplace_back(buf);
		current_root.Child.emplace_back(&(map.back()));
		buf.clear();
	}//本来是应对']'的，但是会多出来

	//std::cout << field << "\n";
	return i;
}
#include <vector>
int JSON_Parse_Pool(JSON_POOL& map, std::string& data) {
	JSON_ACC& root = map.back();
	JSON_ACC buf;
	int j = 0;
	int k = 0;
	int t = 0;
	//map.emplace_back(root);
	for (int i = 0; i < (int)(data.length() - 1); ++i) {
		std::cout << data[i];
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
				buf.Father = &root;
				map.emplace_back(buf);
				root.Child.emplace_back(&(map.back()));
				buf.clear();
			}
			if (data[i + 1] == 't' && data[i + 2] == 'r' && data[i + 3] == 'u' && data[i + 4] == 'e') {//bool true
				buf.content = "true";
				buf.Father = &root;
				map.emplace_back(buf);
				root.Child.emplace_back(&(map.back()));
				buf.clear();
			}
			if (data[i + 1] == 'f' && data[i + 2] == 'a' && data[i + 3] == 'l' && data[i + 4] == 's' && data[i + 5] == 'e') { //bool false
				buf.content = "false";
				buf.Father = &root;
				map.emplace_back(buf);
				root.Child.emplace_back(&(map.back()));
				buf.clear();
			}
			if (data[i + 1] == Quote) {//str
				//std::cout << "STR" << "\n";
				buf.type = str;

				for (j = i + 1; data[j] != ConE && data[j] != LayerE; j++) {
					buf.content.push_back(data[j]);
				}

				buf.Father = &root;
				map.emplace_back(buf);
				root.Child.emplace_back(&(map.back()));
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
					buf.content.push_back(data[j]);
				}

				buf.Father = &root;
				map.emplace_back(buf);
				root.Child.emplace_back(&(map.back()));
				buf.clear();
				i = j - 1;
				j = 0;
			}
			/////////////////////////////////////////////////////////////////////////////////
			if (data[i + 1] == FieldS && data[i + 2] == FieldE) {//void list
				buf.type = dimension_void;
				buf.Father = &root;
				map.emplace_back(buf);
				root.Child.emplace_back(&(map.back()));
				buf.clear();
				i = i + 2;
			}

			if (data[i + 1] == FieldS) { //Dimension List
				buf.type = dimension_list;
				buf.Father = &root;
				map.emplace_back(buf);
				j = DimensionArray_Expect_Pool(data, map, map.back(), i + 1);
				std::cout << "Dimen:" << data[i + 1] << "\n";
				root.Child.emplace_back(&(map.back()));

				buf.clear();
				i = j;
			}

			/////////////////////////////////////////////////////////////////////////////
			if (data[i + 1] == LayerS) { //pair_list {}   Object
				buf.type = pair_list;
				buf.Father = &root;
				map.emplace_back(buf);
				j = PairList_Expect_Pool(data, map, map.back(), i + 1);
				std::cout << "Pair:" << data[i] << "\n";
				root.Child.emplace_back(&(map.back()));

				buf.clear();
				i = j;
				j = 0;
			}

		}
	}
	return 1;
}

#endif
