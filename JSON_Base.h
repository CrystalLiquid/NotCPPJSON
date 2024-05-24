#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <variant>
#include <any>
#include <list>
#include <thread>
#include <future>
#include <thread>
#include <cmath>
#include <cstdlib>
#pragma once
enum data_type {
	temp_digit = -2,
	str = 1,
	digit_int = 5,
	null = 6,
	digit_double = 7,
	pair_list = 8,
	pair_list_void = 12,
	bool_t = 9,
	dimension_void = 10,
	dimension_list = 11,
	error = -1,
	notype = 0
};
#ifndef __BASE_H_
#define __BASE_H_

typedef struct JSON {
  public:
	std::string title;//32
	std::string content;//32
	int type{notype};//4
	void clear() {
		title.clear();
		content.clear();
		type = notype;
		Child.clear();
	}
	std::vector<JSON> Child;//24
}*pJSON;
int PairList_Expect(std::string& data, JSON& currentm, int beginpos);
int DimensionArray_Expect(std::string& data, JSON& currentm, int beginpos);
int JSON_Parse(JSON & map, std::string& data);





struct JSON_ACC {
  public:
	std::string title;//32
	std::string content;//32
	int type{notype};//4
	void clear() {
		title.clear();
		content.clear();
		type = notype;
		Father = nullptr;
		Child.clear();
	}
	JSON_ACC* Father;
	std::vector<JSON_ACC*> Child;//24
};
using JSON_POOL = std::vector<JSON_ACC>;

int PairList_Expect_Pool(std::string& data, JSON_POOL&map, JSON_ACC& current_root, int beginpos);
int DimensionArray_Expect_Pool(std::string& data, JSON_POOL& map, JSON_ACC& current_root, int beginpos);
int JSON_Parse_Pool(JSON_POOL & map, std::string& data);
int initialize(std::vector<JSON_ACC> &map, std::string&data);


#endif
#define PR_SUCCESSFUL_RESULT 1
#define PR_ERROR_VOID_MAP -1
#define PR_ERROR_INVALID_MAP -2
#define PR_ERROR_NULL_DATA 81
#define LayerS '{'
#define LayerE '}'
#define Quote '"'
#define ConE ','
#define FieldS '['
#define FieldE ']'
#define ConS ':'
