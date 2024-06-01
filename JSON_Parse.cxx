#include "JSON_Base.h"
#include <thread>
#include <cmath>
#include <cstdlib>
#pragma once
#ifndef __DE_CXX_
#define __DE_CXX_


int PairList_Expect(std::string& data, JSON& currentm, int beginpos = 0) {
	JSON buf;
	int j = 0;
	int k = 0;
	int i = 0;
	int t = 0;
	int layer = 0;
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
			for (t = i; data[t] != ConE && data[t] != LayerS; t--);
			for (k = t; data[k] != ConS; k++) {
				if (data[k] != LayerS && data[k] != FieldS && data[k] != ConE) {
					buf.title.push_back(data[k]);
				}

			}
			if (data[i + 1] == 'n' && data[i + 2] == 'u' && data[i + 3] == 'l' && data[i + 4] == 'l') {
				buf.content = "null";
				currentm.Child.emplace_back(buf);
				buf.clear();
			}
			if (data[i + 1] == 't' && data[i + 2] == 'r' && data[i + 3] == 'u' && data[i + 4] == 'e') {//bool true
				buf.content = "true";
				currentm.Child.emplace_back(buf);
				buf.clear();
			}
			if (data[i + 1] == 'f' && data[i + 2] == 'a' && data[i + 3] == 'l' && data[i + 4] == 's' && data[i + 5] == 'e') { //bool false
				buf.content = "false";
				currentm.Child.emplace_back(buf);
				buf.clear();
			}
			if (data[i + 1] == Quote) {//str
				//std::cout << "STR" << "\n";
				buf.type = str;

				for (j = i + 1; data[j] != ConE && data[j] != LayerE; j++) {
					buf.content.push_back(data[j]);
				}

				currentm.Child.emplace_back(buf);
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
				currentm.Child.emplace_back(buf);
				buf.clear();
				i = j - 1;
				j = 0;
			}
			//////////////////////////////////////////////////////////////////
			if (data[i + 1] == FieldS && data[i + 2] == FieldE) {//void list
				currentm.Child.emplace_back(buf);
				buf.clear();
				i = i + 2;
			}
			///////////////////////
			if (data[i + 1] == FieldS) { //Dimension List
				buf.type = dimension_list;
				j = DimensionArray_Expect(data, buf, i + 1);
				currentm.Child.emplace_back(buf);
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
				j = PairList_Expect(data, buf, i + 1);
				currentm.Child.emplace_back(buf);
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
		}
	}
	return i;
}

int DimensionArray_Expect(std::string& data, JSON& currentm, int beginpos = 0) {
	//std::cout << "IN_FUNC\n";
	JSON buf;
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
				currentm.Child.emplace_back(buf);
				buf.clear();
			}
			if (data[i + 1] == 't' && data[i + 2] == 'r' && data[i + 3] == 'u' && data[i + 4] == 'e') {//bool true
				buf.content = "true";
				currentm.Child.emplace_back(buf);
				buf.clear();
			}
			if (data[i + 1] == 'f' && data[i + 2] == 'a' && data[i + 3] == 'l' && data[i + 4] == 's' && data[i + 5] == 'e') { //bool false
				buf.content = "false";
				currentm.Child.emplace_back(buf);
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
				currentm.Child.emplace_back(buf);
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

				currentm.Child.emplace_back(buf);
				buf.clear();
				i = j - 1;
				j = 0;
			}

			/////////////////////////////////////////////////////////////////////////////////
			if (data[i + 1] == FieldS && data[i + 2] == FieldE) {//void list
				buf.type = dimension_void;
				currentm.Child.emplace_back(buf);
				buf.clear();
				//i = i + 2;
				i = i + 2;
				//std::cout << data[i - 1] << "("  << data[i] << ")" << "   Void\n";
			}

			if (data[i + 1] == FieldS) { //Dimension List
				buf.type = dimension_list;
				j = DimensionArray_Expect(data, buf, i + 1);
				currentm.Child.emplace_back(buf);
				buf.clear();
				//i = j;
				i = j;
				//std::cout << data[i - 1] << "("  << data[i] << ")" << "   NonVoid\n";
			}

			/////////////////////////////////////////////////////////////////////////////

			if (data[i + 1] == LayerS && data[i + 2] == LayerE) { //pair_list_VOID {}   Object
				buf.type = pair_list_void;
				currentm.Child.emplace_back(buf);
				buf.clear();
				i = i + 2;
			}

			if (data[i + 1] == LayerS) { //pair_list {}   Object///////////////////
				buf.type = pair_list;

				j = PairList_Expect(data, buf, i + 1);
				currentm.Child.emplace_back(buf);
				buf.clear();

				i = j - 1;
				//std::cout << "PAIR_LIST:" << data[i] << "\n";
				j = 0;
			}

		}

	}
	//std::cout << "Outa!:" << i << "\n";
	if (!buf.content.empty()) {
		currentm.Child.emplace_back(buf);
		buf.clear();
	}//本来是应对']'的，但是会多出来

	//std::cout << field << "\n";
	return i;
}

int JSON_Parse(JSON & map, std::string& data) {
	JSON buf;
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
				map.Child.emplace_back(buf);
				buf.clear();
			}
			if (data[i + 1] == 't' && data[i + 2] == 'r' && data[i + 3] == 'u' && data[i + 4] == 'e') {//bool true
				buf.content = "true";
				map.Child.emplace_back(buf);
				buf.clear();
			}
			if (data[i + 1] == 'f' && data[i + 2] == 'a' && data[i + 3] == 'l' && data[i + 4] == 's' && data[i + 5] == 'e') { //bool false
				buf.content = "false";
				map.Child.emplace_back(buf);
				buf.clear();
			}
			if (data[i + 1] == Quote) {//str
				//std::cout << "STR" << "\n";
				buf.type = str;

				for (j = i + 1; data[j] != ConE && data[j] != LayerE; j++) {
					buf.content.push_back(data[j]);
				}

				map.Child.emplace_back(buf);
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

				map.Child.emplace_back(buf);
				buf.clear();
				i = j - 1;
				j = 0;
			}
			/////////////////////////////////////////////////////////////////////////////////
			if (data[i + 1] == FieldS && data[i + 2] == FieldE) {//void list
				buf.type = dimension_void;
				map.Child.emplace_back(buf);
				buf.clear();
				i = i + 2;
			}

			if (data[i + 1] == FieldS) { //Dimension List
				buf.type = dimension_list;
				j = DimensionArray_Expect(data, buf, i + 1);
				//std::cout << "Dimen:" << data[i + 1] << "\n";
				map.Child.emplace_back(buf);
				buf.clear();
				i = j;
			}

			/////////////////////////////////////////////////////////////////////////////
			if (data[i + 1] == LayerS) { //pair_list {}   Object
				buf.type = pair_list;

				j = PairList_Expect(data, buf, i + 1);
				map.Child.emplace_back(buf);
				buf.clear();
				i = j;
				j = 0;
			}

		}
	}
	return 1;
}


#endif
