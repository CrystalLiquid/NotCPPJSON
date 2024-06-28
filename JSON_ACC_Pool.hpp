#include "JSON_Base.h"
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
#include <variant>
namespace json_acc_variant_maybe_deprecate {
//variant擦除，难以序列化，可能会丢弃
	struct json_acc;
	enum search_t {
		directly = 1,
		dfs = 2,
		bfs = 3
	};
	struct json_pool_slice {
		int begin{-1};
		int end{-1};
	};
	class json_pool : public std::vector<json_acc> {
	  public:
		json_pool_slice get_slice(std::string v) {
			json_pool_slice tmp;
			std::string sk, sj;
			for (int i = 0; i < v.length(); i++) {
				if (v[i] == ':') {
					for (int k = i + 1; k < v.length() - 1; k++) {
						sk.push_back(v[k]);
					}
					for (int j = 1; j < i; j++) {
						sj.push_back(v[j]);
					}
				}
			}
			tmp.begin = std::stoi(sj);
			tmp.end = std::stoi(sk);
			return tmp;
		}
		void set_val_pool(std::string& key, int search_type = search_t::directly) {
			if (search_type == search_t::directly) {
				for (int i = 0; i < this->size(); i++) {

				}
			}
			if (search_type == search_t::dfs) {

			}
			if (search_type == search_t::bfs) {

			}
		}
	};
	using i64t = int64_t;
	using d64t = long double;
//uing JSON_POOL =  std::vector<JSON_ACC>;

	struct json_acc {

		using content_type = std::variant<i64t, double, std::string, bool, std::monostate>;
	  public:
		std::string title;//32
		content_type content;
		int type{notype};//4
		int Father_idx{0};
		std::vector<int> Child_idx;

	  public:
		using unsafe_ptr = void*;
		json_acc(json_pool& v) {}
		json_acc():
			type(notype),
			Father_idx(0) {};
		//std::variant<int, double, std::string, bool, JSON_POOL>
		std::variant<i64t, double, std::string, bool, std::monostate, json_pool_slice> get_val(json_pool* ppool = nullptr) {
			if (!std::get_if<i64t>(&content) || !std::get_if<std::string>(&content) || !std::get_if<bool>(&content) || !std::get_if<double>(&content)) {
				//TODO 非空判断，get_if如果类型不对会返回空指针
				if (type == str || type == dimension_void || type == pair_list_void || type == null) {
					return std::get<std::string>(content);
				}
				if (type == bool_t) {
					return std::get<bool>(content);
				}
				if (type == digit_int) {
					return std::get<i64t>(content);
				}
				if (type == digit_double) {
					return std::get<double>(content);
				}
				if (type == pair_list || type == dimension_list) {
					return json_pool_slice{Child_idx.front(), Child_idx.back()};
				}
				if (type == notype) {
					return std::monostate{};
				}
			}
			return std::monostate{};
		}
		void set_val(std::variant<i64t, double, std::string, bool, std::monostate, json_pool> v, int list_type = notype, json_pool* pool = nullptr) {

			auto check_type = [&v, &list_type]() {
				if (!std::get_if<i64t>(&v) ) {
					return digit_int;
				}
				if (!std::get_if<std::string>(&v)) {
					if ((std::get<std::string>(v))[0] == Quote) {
						return str;
					}
					if ((std::get<std::string>(v))[0] == 'n') {
						return null;
					}
					if ((std::get<std::string>(v))[0] == FieldS) {
						return dimension_void;
					}
					if ((std::get<std::string>(v))[0] == LayerS) {
						return pair_list_void;
					}
				}
				if (!std::get_if<bool>(&v)) {
					return bool_t;
				}
				if (!std::get_if<double>(&v)) {
					return digit_double;
				}
				if (!std::get_if<std::monostate>(&v)) {

					if (list_type == notype) {
						std::cerr << "[On Processing Get Val-Check_Type]Invalid List Type!\n";
						return invalid_opttype;
					}
					if (list_type == pair_list) {
						return pair_list;
					}
					if (list_type == dimension_list) {
						return dimension_list;
					}
				}
			};
			int tmp_type = check_type();
			switch (tmp_type) {
				case notype:
					return;
					break;
				case str:
					content = (std::get<std::string>(v));
					type = tmp_type;
					break;
				case digit_int:
					content = (std::get<i64t>(v));
					type = tmp_type;
					break;
				case digit_double:
					content = (std::get<double>(v));
					type = tmp_type;
					break;
				case dimension_list:
					content = std::monostate{};
					if (pool == nullptr) {
						std::cerr << "[On Processing Get Val-Switch]Invalid Poolptr!\n";
						return;
					}
					pool->insert(pool->end(),//need to search for proper key-val idx
					             std::make_move_iterator((*std::get_if<json_pool>(&v)).begin()),
					             std::make_move_iterator((*std::get_if<json_pool>(&v)).end())
					            );
					break;
				case pair_list:
					content = std::monostate{};
					if (pool == nullptr) {
						std::cerr << "[On Processing Get Val-Switch]Invalid Poolptr!\n";
						return;
					}
					pool->insert(pool->end(),
					             std::make_move_iterator((*std::get_if<json_pool>(&v)).begin()),
					             std::make_move_iterator((*std::get_if<json_pool>(&v)).end())
					            );
					break;
				case dimension_void:
					content = (std::get<std::string>(v));
					type = tmp_type;
					break;
				case pair_list_void:
					content = (std::get<std::string>(v));
					type = tmp_type;
					break;
				case bool_t:
					content = (std::get<bool>(v));
					type = tmp_type;
					break;
				case null:
					content = (std::get<std::string>(v));
					type = tmp_type;
					break;
				case invalid_opttype:
					return;
					break;
				default:
					//TODO
					break;
			}
		}
		void set_key(std::string & tle) {
			title = tle;
		}
		unsafe_ptr get_pval() noexcept {
			switch (this->type) {
				case notype:
					return nullptr;
					break;
				case str:
					return (unsafe_ptr)std::get_if<std::string>(&this->content);
					break;
				case digit_int:
					return (unsafe_ptr)std::get_if<i64t>(&this->content);
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
			content = std::monostate{};
			type = notype;
			Father_idx = 0;
			Child_idx.clear();
		}

	};


	int PairList_Expect_Pool(std::string& data, json_pool&map, int current_root_idx, int beginpos);
	int DimensionArray_Expect_Pool(std::string& data, json_pool& map, int current_root_idx, int beginpos);
	int JSON_Parse_Pool(json_pool & map, std::string& data);
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
					j = json_acc_variant::DimensionArray_Expect_Pool(data, map, map.size() - 1, i + 1);

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
					j = json_acc_variant::PairList_Expect_Pool(data, map, map.size() - 1, i + 1);

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
					j = json_acc_variant::DimensionArray_Expect_Pool(data, map, map.size() - 1, i + 1);

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
					j = json_acc_variant::PairList_Expect_Pool(data, map, map.size() - 1, i + 1);

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
					j = json_acc_variant::DimensionArray_Expect_Pool(data, map, map.size() - 1, i + 1);
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
					j = json_acc_variant::PairList_Expect_Pool(data, map, map.size() - 1, i + 1);

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
}
