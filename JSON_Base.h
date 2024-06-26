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
#include <deque>
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
#include <any>
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
	template<typename T>
	T get_val() {
		switch (this->type) {
			case notype:
				return std::string("");
				break;
			case str:
				return this->content;
				break;
			case digit_int:
				return std::stoi(this->content);
				break;
			case digit_double:
				return std::stod(this->content);
				break;
			case dimension_list:
				return this->Child;
				break;
			case pair_list:
				return this->Child;
				break;
			case dimension_void:
				return std::string("[]");
				break;
			case pair_list_void:
				return std::string("{}");
				break;
			case bool_t:
				if (content == "true") {
					return true;
				}
				if (content == "false") {
					return false;
				}
				break;
			case null:
				return std::string("null");
				break;
			default:
				//TODO
				break;
		}
	}
	std::vector<JSON> Child;//24
}*pJSON;
int PairList_Expect(std::string& data, JSON& currentm, int beginpos);
int DimensionArray_Expect(std::string& data, JSON& currentm, int beginpos);
int JSON_Parse(JSON & map, std::string& data);

////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <variant>
struct JSON_ACC;
using JSON_POOL =  std::vector<JSON_ACC>;
struct JSON_ACC {
  public:
	std::string title;//32
	std::variant<int, double, std::string, bool> content;
	//std::string content;//32
	int type{notype};//4
	int Father_idx{0};
	std::vector<int> Child_idx;
//	JSON_ACC(std::string &&tle, std::string &&con, int tp, int fa):
//		title(std::move(tle)),
//		content(std::move(con)),
//		type(tp),
//		Father_idx(fa) {};
	using unsafe_ptr = void*;
	JSON_ACC(JSON_POOL& v) {

	}
	JSON_ACC():
		type(notype),
		Father_idx(0) {};
	unsafe_ptr get_pval() {
		switch (this->type) {
			case notype:
				return nullptr;
				break;
			case str:
				return (unsafe_ptr)std::get_if<std::string>(&this->content);
				break;
			case digit_int:
				return (unsafe_ptr)std::get_if<int>(&this->content);
				break;
			case digit_double:
				return (unsafe_ptr)std::get_if<double>(&this->content);
				break;
			case dimension_list:
				return &this->Child_idx;
				break;
			case pair_list:
				return &this->Child_idx;
				break;
			case dimension_void:
				return (unsafe_ptr)std::get_if<std::string>(&this->content);
				break;
			case pair_list_void:
				return (unsafe_ptr)std::get_if<std::string>(&this->content);
				break;
			case bool_t:
				return (unsafe_ptr)std::get_if<bool>(&this->content);
				break;
			case null:
				return (unsafe_ptr)std::get_if<std::string>(&this->content);
				break;
			default:
				//TODO
				break;
		}
		return nullptr;
	}

	void clear() {
		title.clear();

		type = notype;
		Father_idx = 0;
		Child_idx.clear();
	}

};


int PairList_Expect_Pool(std::string& data, JSON_POOL&map, int current_root_idx, int beginpos);
int DimensionArray_Expect_Pool(std::string& data, JSON_POOL& map, int current_root_idx, int beginpos);
int JSON_Parse_Pool(JSON_POOL & map, std::string& data);

int JSON_Parse_Memcpy_Pool(JSON_POOL & map, JSON_ACC&root, std::string& data);

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




/*#include <memory>

struct JSON_POOL {
using  plJSON_ACC = std::unique_ptr<JSON_ACC[]>;
plJSON_ACC raw_ptr	;
int pool_size{0};
//	JSON_POOL() {
//		raw_ptr = std::make_unique<JSON_ACC[]>(1);
//		pool_size += 1;
//	}
explicit JSON_POOL(int initial_num) {
raw_ptr = std::make_unique<JSON_ACC[]>(initial_num);
pool_size += initial_num;
}
int size() {
return pool_size;
}
JSON_ACC& operator[](int idx) {
return raw_ptr[idx];
}
JSON_ACC& at(int idx) {
if (idx >= this->pool_size) {
return raw_ptr[idx];
}
return raw_ptr[0];
}
void clear_pool() {
raw_ptr.reset();
raw_ptr.release();
}
~JSON_POOL() {
raw_ptr.reset();
raw_ptr.release();
}
void emplace_back(JSON_ACC& to_emplace) {
raw_ptr = std::make_unique<JSON_ACC[]>(this->pool_size + 1);
++pool_size;
(raw_ptr[this->pool_size - 1]).title = to_emplace.title;
(raw_ptr[this->pool_size - 1]).content = to_emplace.content;
(raw_ptr[this->pool_size - 1]).type = to_emplace.type;
(raw_ptr[this->pool_size - 1]).Father = to_emplace.Father;
(raw_ptr[this->pool_size - 1]).Child.swap(to_emplace.Child);
}
JSON_ACC& back() {
return (raw_ptr[this->pool_size - 1]);
}
};

*/
/*
JSON_ACC() noexcept:
type(notype),
Father_idx(0) {};
JSON_ACC(std::string& tle, std::string& con, int tp, int fa) noexcept:
title(std::move(tle)),
content(std::move(con)),
type(tp),
Father_idx(fa) {};
*/


/*
JSON_ACC(const JSON_ACC& v):
title(v.title),
content(v.content),
type(v.type),
Father_idx(v.Father_idx),
Child_idx(v.Child_idx) {};
JSON_ACC(JSON_ACC && v) noexcept:
title(std::move(v.title)),
content(std::move(v.content)),
type(v.type),//虽然是移动构造，但是基本类型只能复制，STL才可能移动
Father_idx(v.Father_idx),//虽然是移动构造，但是基本类型只能复制，STL才可能移动
Child_idx(std::move(v.Child_idx)) {};
*/
