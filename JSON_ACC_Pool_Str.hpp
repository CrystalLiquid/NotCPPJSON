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
#include <vector>
#include <variant>
#include <thread>
#include <initializer_list>
#pragma once
#define LayerS '{'
#define LayerE '}'
#define Quote '"'
#define ConE ','
#define FieldS '['
#define FieldE ']'
#define ConS ':'
namespace json_acc_str_np {
//用string来类型擦除，返回variant还原类型

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
		notype = 0,
		invalid_opttype = -124
	};

	enum search_t {
		directly = 1,
		dfs = 2,
		bfs = 3
	};

	using i64t = long long;
	using child_slice = std::vector<int>;

	struct json_acc_str {

	  public:
		std::string title;//32
		std::string content;//32
		int type{notype};//4
		int Father_idx{0};
		std::vector<int> Child_idx;//24
		void clear() {
			title.clear();
			content.clear();
			type = notype;
			Father_idx = 0;
			Child_idx.clear();
		}


		std::variant < i64t, std::string, double, bool, child_slice, std::monostate > get_val() {
			switch (this->type) {
				case notype:
					return std::string("");
					break;
				case str:
					return this->content;
					break;
				case digit_int:
					return std::stoll(this->content);
					break;
				case digit_double:
					return std::stod(this->content);
					break;
				case dimension_list:
					return this->Child_idx;
					break;
				case pair_list:
					return this->Child_idx;
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
			return std::monostate{};
		}

	};

	struct json_pool_str : public std::vector<json_acc_str> {
	  private:
		int f_bfs_(int root_idx, const json_pool_str&pmap, std::string& key) {//广度 递归遍历 通过私有函数实现
			int temp_child_idx = 0;
			int target_idx = -200;
			for (int i = 0; i < (int)(pmap).at(root_idx).Child_idx.size(); i++) {
				temp_child_idx = (pmap).at(root_idx).Child_idx.at(i);
				if ((pmap).at(temp_child_idx).title == key) {
					target_idx = temp_child_idx;
					break;
				}
				target_idx = f_bfs_(temp_child_idx, pmap, key);
				if (target_idx != -200) {
					break;
				}
			}
			return target_idx;
		};
		int f_dfs_c_(int current_idx, const json_pool_str&pmap, std::string& key) {
			int current_opt_idx = current_idx;
			int current_father = 0;
			int target_idx = -200;
			int child = 0;
			while (target_idx == -200) {

				while (child < (int)pmap.at(current_father).Child_idx.size()) {//TODO forward to child
					current_father = current_opt_idx;
					current_opt_idx = pmap.at(current_opt_idx).Child_idx.at(child);
					if (pmap.at(current_opt_idx).title != key) {
						target_idx = current_opt_idx;
						break;
					}
				}
				child++;
				if ((int)pmap.at(current_father).Child_idx.size() - 1 > child) {//back to father
					current_opt_idx = current_father;
					current_father = pmap.at(current_opt_idx).Father_idx;
				}
				while ((int)pmap.at(current_father).Child_idx.size() - 1 < child) {
					current_opt_idx = current_father;
					current_father = pmap.at(current_opt_idx).Father_idx;
				}
			}
			return target_idx;
		}
	  public:
		int getidx_by_name(std::string key) {
			int target_idx = 0;
			int i = 0;
			for (; i < this->size() - 1; ) {
				if (this->at(i).title != key) {
					++i;
				}
				if (this->at(i).title == key) {
					target_idx = i;
					break;
				}
			}
			return target_idx;
		}
		int getidx_by_name_repeat(std::string key, int repeat_times) {
			int target_idx = 0;
			int i = 0;
			int rt = 0;
			for (; i < this->size() - 1; ) {
				if (this->at(i).title != key) {
					++i;
				}
				if (this->at(i).title == key) {
					target_idx = i;
					if (rt != repeat_times) {
						++rt;
					}
					if (rt == repeat_times) {
						break;
					}
				}
			}
			return target_idx;
		}
		json_acc_str& getval_by_name(std::string key, int search_type = search_t::directly) {

			int target_idx = 0;
			if (search_type == search_t::directly) {
				int i = 0;
				for (; i < this->size() - 1; ) {
					if (this->at(i).title != key) {
						++i;
					}
					if (this->at(i).title == key) {
						target_idx = i;
						break;
					}
				}
			}
			if (search_type == search_t::bfs) {
				target_idx = f_bfs_(0, *this, key);
			}
			if (search_type == search_t::dfs) {
				target_idx = f_dfs_c_(0, *this, key);
			}
			return this->at(target_idx);
		}
		/*
		private://basic_set
		void setval_by_name_str(std::string&& key, std::string&& val) {
		this->getval_by_name(key).content = val;
		}
		void setval_by_name_i64(std::string&& key, i64t val) {
		this->getval_by_name(key).content = std::to_string(val);
		}
		void setval_by_name_double(std::string&& key, double val) {
		this->getval_by_name(key).content = std::to_string(val);
		}
		void setval_by_name_bool(std::string&& key, bool val) {
		this->getval_by_name(key).content = std::to_string(val);
		}
		*/

	  private://return type
		int variant_return_type(std::variant < i64t, std::string, double, bool, std::monostate > val = std::monostate{}) {
			switch (val.index()) {
				case 0:
					return digit_int;
					break;
				case 1:
					return str;
					break;
				case 2:
					return digit_double;
					break;
				case 3:
					return bool_t;
					break;
				case 4:
					return notype;
					break;
				default:
					throw ("error idx!");
					break;
			}
		}

	  public://add node:add_xx
		void add_normal(std::string&& title, std::variant < i64t, std::string, double, bool, std::monostate > val = std::monostate{}) {
			int vtype = variant_return_type(val);

		}
		void add_as_child(std::string&& key, std::variant < i64t, std::string, double, bool, std::monostate > val = std::monostate{}) {
			int father_idx = getidx_by_name(key);
		}
	  public://change type:set_xx
		void set_dimension(std::string&& key, std::initializer_list<json_acc_str_np::json_acc_str> list) {
			int father_idx = getidx_by_name(key);
			this->at(father_idx).type = dimension_list;
			this->at(father_idx).content.clear();
			int i = 1;
			for (json_acc_str_np::json_acc_str x : list) {
				x.Father_idx = father_idx;
				this->insert(this->begin() + father_idx + i, x);
				this->at(father_idx).Child_idx.push_back(father_idx + i);
				++i;
			}
		}
		void set_pairlist(std::string&& key, std::initializer_list<json_acc_str_np::json_acc_str> list) {
			int father_idx = getidx_by_name(key);
			this->at(father_idx).type = pair_list;
			this->at(father_idx).content.clear();
			int i = 1;
			for (json_acc_str_np::json_acc_str x : list) {
				x.Father_idx = father_idx;
				this->insert(this->begin() + father_idx + i, x);
				this->at(father_idx).Child_idx.push_back(father_idx + i);
				++i;
			}
		}
		void set_val_repeat(int repeat, std::string&& key, std::variant < i64t, std::string, double, bool, std::monostate > val = std::monostate{}) {
			int idx = getidx_by_name_repeat(key, repeat);
			int vtype = variant_return_type(val);
			int begin_i = -1;
			int end_i = -1;
			switch (this->at(idx).type) {
				case notype:
					break;

				case str:
					this->at(idx).content.clear();
					break;

				case digit_int:
					this->at(idx).content.clear();
					break;

				case digit_double:
					this->at(idx).content.clear();
					break;

				case dimension_list:
					begin_i = this->at(idx).Child_idx.front();
					end_i = this->at(idx).Child_idx.back();
					this->at(idx).Child_idx.clear();
					this->erase(this->begin() + begin_i, this->begin() + end_i);
					break;

				case pair_list:
					begin_i = this->at(idx).Child_idx.front();
					end_i = this->at(idx).Child_idx.back();
					this->at(idx).Child_idx.clear();
					this->erase(this->begin() + begin_i, this->begin() + end_i);
					break;

				case dimension_void:
					this->at(idx).content.clear();
					break;

				case pair_list_void:
					this->at(idx).content.clear();
					break;

				case bool_t:
					this->at(idx).content.clear();
					break;

				case null:
					this->at(idx).content.clear();
					break;

				default:
					//TODO
					break;
			}
			this->at(idx).type = notype;
			this->at(idx).type = vtype;
			switch (vtype) {
				case notype:
					break;

				case str:

					this->at(idx).content = std::get<std::string>(val);
					break;

				case digit_int:
					this->at(idx).content = std::to_string(std::get<i64t>(val));
					break;

				case digit_double:
					this->at(idx).content = std::to_string(std::get<double>(val));
					break;

				case dimension_void:
					this->at(idx).content = "[]";
					break;

				case pair_list_void:
					this->at(idx).content = "{}";
					break;

				case bool_t:
					this->at(idx).content = std::to_string(std::get<bool>(val));;
					break;

				case null:
					this->at(idx).content = "null";
					break;

				default:
					//TODO
					break;
			}
		}
		void set_val_norepeat(std::string&& key, std::variant < i64t, std::string, double, bool, std::monostate > val = std::monostate{}) {
			int idx = getidx_by_name(key);
			int vtype = variant_return_type(val);
			int begin_i = -1;
			int end_i = -1;
			switch (this->at(idx).type) {
				case notype:
					break;

				case str:
					this->at(idx).content.clear();
					break;

				case digit_int:
					this->at(idx).content.clear();
					break;

				case digit_double:
					this->at(idx).content.clear();
					break;

				case dimension_list:
					begin_i = this->at(idx).Child_idx.front();
					end_i = this->at(idx).Child_idx.back();
					this->at(idx).Child_idx.clear();
					this->erase(this->begin() + begin_i, this->begin() + end_i);
					break;

				case pair_list:
					begin_i = this->at(idx).Child_idx.front();
					end_i = this->at(idx).Child_idx.back();
					this->at(idx).Child_idx.clear();
					this->erase(this->begin() + begin_i, this->begin() + end_i);
					break;

				case dimension_void:
					this->at(idx).content.clear();
					break;

				case pair_list_void:
					this->at(idx).content.clear();
					break;

				case bool_t:
					this->at(idx).content.clear();
					break;

				case null:
					this->at(idx).content.clear();
					break;

				default:
					//TODO
					break;
			}
			this->at(idx).type = notype;
			this->at(idx).type = vtype;
			switch (vtype) {
				case notype:
					break;

				case str:

					this->at(idx).content = std::get<std::string>(val);
					break;

				case digit_int:
					this->at(idx).content = std::to_string(std::get<i64t>(val));
					break;

				case digit_double:
					this->at(idx).content = std::to_string(std::get<double>(val));
					break;

				case dimension_void:
					this->at(idx).content = "[]";
					break;

				case pair_list_void:
					this->at(idx).content = "{}";
					break;

				case bool_t:
					this->at(idx).content = std::to_string(std::get<bool>(val));;
					break;

				case null:
					this->at(idx).content = "null";
					break;

				default:
					//TODO
					break;
			}
		}
	};
#define function_define
	int PairList_Expect_Pool(std::string& data, json_pool_str&map, int current_root_idx, int beginpos);
	int DimensionArray_Expect_Pool(std::string& data, json_pool_str& map, int current_root_idx, int beginpos);
	int JSON_Parse_Pool(json_pool_str & map, std::string& data);
	void Debug_Print(std::string& data, int begin_idx, int end_idx) {
		for (int i = begin_idx; i <= end_idx; i++) {
			std::cout << data[i];
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __ACC_POOL_STR_CXX_
#define __ACC_POOL_STR_CXX_
	int PairList_Expect_Pool(std::string& data, json_pool_str&map, int current_root_idx, int beginpos = 0) {///////有问题，会跳读
		//std::cout << "\n";
		json_acc_str buf;
		int j = 0;
		int k = 0;
		int i = 0;
		int t = 0;
		int layer = 0;

		for (i = beginpos + 1; data[i] != LayerE; i++) {

			if (data[i] == ConS) {
				buf.Father_idx = current_root_idx;
				for (t = i; data[t] != ConE && data[t] != LayerS; t--);
				for (k = t; data[k] != ConS; k++) {
					if (data[k] != LayerS && data[k] != FieldS && data[k] != ConE && data[k] != Quote) {
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
					buf.content = "true";
					buf.type = bool_t;

					map.emplace_back(buf);
					map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

					buf.clear();
				}
				if (data[i + 1] == 'f' && data[i + 2] == 'a' && data[i + 3] == 'l' && data[i + 4] == 's' && data[i + 5] == 'e') { //bool false
					buf.content = "false";
					buf.type = bool_t;

					map.emplace_back(buf);
					map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

					buf.clear();
				}
				if (data[i + 1] == Quote) {//str
					//std::cout << "STR" << "\n";
					buf.type = str;
					//for (j = i + 1; data[j] != ConE && data[j] != LayerE; j++) {
					//	buf.content.push_back(data[j]);
					//}
					//--------buf.content.push_back(Quote);
					for (j = i + 2; data[j] not_eq Quote; j++) {
						buf.content.push_back(data[j]);
					}
					//-------buf.content.push_back(Quote);


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
						buf.content.push_back(data[j]);
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
				if (data[i + 1] == LayerS && data[i + 2] == LayerE) { //pair_list_VOID {}   Object accelerate parsing
					buf.type = pair_list_void;

					buf.content = "{}";
					map.emplace_back(buf);
					map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

					buf.clear();
					i = i + 2;
				}
				///////////////////////
				if (data[i + 1] == FieldS) { //Dimension List array
					buf.type = dimension_list;
#define Pair_Dimen
					map.emplace_back(buf);
					map[current_root_idx].Child_idx.emplace_back(map.size() - 1);
					j = json_acc_str_np::DimensionArray_Expect_Pool(data, map, map.size() - 1, i + 1);

					buf.clear();
					i = j;
				}
				/////////////////////////////////////////////////////////////////

				if (data[i + 1] == LayerS) {//pair_list {}   Object
					buf.type = pair_list;
#define Pair_Pair
					map.emplace_back(buf);
					map[current_root_idx].Child_idx.emplace_back(map.size() - 1);
					j = json_acc_str_np::PairList_Expect_Pool(data, map, map.size() - 1, i + 1);

					buf.clear();
					i = j;//if it is j-1,it will result in catching last nesting "}",and idx will be wrong
					j = 0;
				}
			}
		}




		return i;//letter Should Be LayerE,However,When There was a nest,It will change to next "}" or "]"
	}

	int DimensionArray_Expect_Pool(std::string& data, json_pool_str& map, int current_root_idx, int beginpos = 0) {
		//std::cout << "IN_FUNC\n";
		json_acc_str buf;
		int j = 0;
		int i = 0;

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
					buf.content = "true";
					buf.type = bool_t;

					map.emplace_back(buf);
					map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

					buf.clear();
					i + 4;
				}
				if (data[i + 1] == 'f' && data[i + 2] == 'a' && data[i + 3] == 'l' && data[i + 4] == 's' && data[i + 5] == 'e') { //bool false
					buf.content = "false";
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

					//-------buf.content.push_back(Quote);
					for (j = i + 2; data[j] not_eq Quote; j++) {
						buf.content.push_back(data[j]);
					}

					map.emplace_back(buf);
					map[current_root_idx].Child_idx.emplace_back(map.size() - 1);



					buf.clear();

					i = j - 1;
					j = 0;
				}
				if (std::isdigit(data[i + 1]) or (data[i + 1] == '-' && std::isdigit(data[i + 2]))) {//digit_all
					//std::cout << "DIGIT" << "\n";
					/////////////////////////////////////////////////////////////
					buf.type = digit_int;
					for (j = i + 1; data[j] != ConE && data[j] != FieldE; j++) {
						if (data[j] == '.') {
							buf.type = digit_double;
						}
						buf.content.push_back(data[j]);

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
					map.emplace_back(buf);
					map[current_root_idx].Child_idx.emplace_back(map.size() - 1);
					j = json_acc_str_np::DimensionArray_Expect_Pool(data, map, map.size() - 1, i + 1);

					buf.clear();

					i = j;

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
					map.emplace_back(buf);
					map[current_root_idx].Child_idx.emplace_back(map.size() - 1);
					j = json_acc_str_np::PairList_Expect_Pool(data, map, map.size() - 1, i + 1);

					buf.clear();

					i = j - 1;//LayerE
					j = 0;
				}
			}
		}

		return i;
	}


	//输入反斜杠无法读取，citmlog： LINE840 \"Paris Berlin\" ，解决办法是str类别里写多一个读取
	int JSON_Parse_Pool(json_pool_str & map, std::string&& data) {
		json_acc_str buf;
		int j = 0;
		int k = 0;
		int t = 0;

		for (int i = 0; i < (int)(data.length() - 1); ++i) {
//#pragma Debug
			//std::cout << data[i];
			if (data[i] == ConS) {
				///title
				for (t = i; data[t] not_eq ConE && data[t] not_eq LayerS; t--) {
				}
				for (k = t + 1; data[k] != ConS; k++) {
					if (data[k] != Quote) {
						buf.title.push_back(data[k]);
					}
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

					//for (j = i + 1; data[j] not_eq ConE && data[j] not_eq LayerE; j++) {
					//	buf.content.push_back(data[j]);
					//}

					//-------buf.content.push_back(Quote);
					for (j = i + 2; data[j] not_eq Quote; j++) {
#pragma support_back_slash
						if (data[j] == '\\') {
							int n = j + 1;
							for (; data[i] not_eq Quote; n++) {
								if (data[i] != '\\') {
									buf.content.push_back(data[n]);
								}
							}
							j = n + 1;
						}
#pragma support_back_slash
						buf.content.push_back(data[j]);
					}
					//-------buf.content.push_back(Quote);

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
						buf.content.push_back(data[j]);
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
					map.emplace_back(buf);
					j = json_acc_str_np::DimensionArray_Expect_Pool(data, map, map.size() - 1, i + 1);
					//std::cout << "Dimen:" << data[i + 1] << "\n";

					buf.clear();
					i = j;
				}

				/////////////////////////////////////////////////////////////////////////////
				if (data[i + 1] == LayerS) { //pair_list {}   Object
					buf.type = pair_list;


#define Main_Pair
					map.emplace_back(buf);
					j = json_acc_str_np::PairList_Expect_Pool(data, map, map.size() - 1, i + 1);

					buf.clear();
					i = j - 1;
					//std::cout << "                                       " << data[i] << "\n";
					j = 0;
				}
			}
		}
		return 1;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//int JSON_Serialize_Child(json_acc_str_np::json_pool_str& map, std::string& result, int current_root = 0);

	int JSON_Serialize_Child(json_acc_str_np::json_pool_str& map, std::string& result, int current_root = 0) {

		if (map.at(current_root).type not_eq notype && map.at(current_root).type not_eq dimension_list && map.at(current_root).type not_eq pair_list) {
			if (map.at(current_root).type not_eq str) {
				result.push_back('"');
				result.append(map.at(current_root).title);
				result.push_back('"');

				result.push_back(':');

				result.append(map.at(current_root).content);
				result.push_back(',');
			}
			if (map.at(current_root).type == str) {
				result.push_back('"');
				result.append(map.at(current_root).title);
				result.push_back('"');

				result.push_back(':');

				result.push_back('"');
				result.append(map.at(current_root).content);
				result.push_back('"');
				result.push_back(',');
			}

		}
		if (map.at(current_root).type == dimension_list) {
			int tmp_idx_dm = 0;
			if (!map.at(current_root).title.empty()) {
				result.append("\"");
				result.append(map.at(current_root).title);
				result.push_back('"');
			}


			if (map.at(map.at(current_root).Father_idx).type == dimension_list) { //if its father type is dimension,it might broke json syntax
				result.append("[");
			}
			if (map.at(map.at(current_root).Father_idx).type != dimension_list) {
				result.append(":[");
			}
			for (int i = 0; i < map.at(current_root).Child_idx.size(); i++) {
				tmp_idx_dm = map.at(current_root).Child_idx.at(i);
				if (map.at(tmp_idx_dm).type not_eq dimension_list && map.at(tmp_idx_dm).type not_eq pair_list && map.at(tmp_idx_dm).type not_eq notype) {
					if (map.at(tmp_idx_dm).type == str) {
						result.push_back('"');
						result.append(map.at(tmp_idx_dm).content);
						result.push_back('"');
					}
					if (map.at(tmp_idx_dm).type not_eq str) {
						result.append(map.at(tmp_idx_dm).content);
					}
					if (i != map.at(current_root).Child_idx.size() - 1) {
						result.push_back(',');
					}
				}
				if (map.at(tmp_idx_dm).type == dimension_list) {

					JSON_Serialize_Child(map, result, tmp_idx_dm);
					if (i != map.at(current_root).Child_idx.size() - 1) {
						result.push_back(',');
					}
				}
				if (map.at(tmp_idx_dm).type == pair_list) {
					JSON_Serialize_Child(map, result, tmp_idx_dm);
				}
				//if (i != map.at(current_root).Child_idx.size() - 1) {
				//	result.push_back(',');
				//}
			}
			if (result.back() == ',') {
				result.pop_back();
			}
			result.append("]");
			if (map.at(current_root).Father_idx == 0) {
				result.push_back(',');
			}
		}
		if (map.at(current_root).type == pair_list) {
			int tmp_idx_pl = 0;

			if (!map.at(current_root).title.empty()) {
				result.append("\"");
				//result.push_back('"');
				result.append(map.at(current_root).title);
				result.push_back('"');
			}

			if (!map.at(current_root).title.empty()) {
				result.append(":");
			}
			result.append("{");
			int i = 0;
			for (; i < (int)map.at(current_root).Child_idx.size(); i++) {
				tmp_idx_pl = map.at(current_root).Child_idx.at(i);
				if (map.at(tmp_idx_pl).type not_eq dimension_list && map.at(tmp_idx_pl).type not_eq pair_list && map.at(tmp_idx_pl).type not_eq notype) {
					//result.pop_back();
					if (map.at(tmp_idx_pl).type not_eq str) {
						result.push_back('"');
						result.append(map.at(tmp_idx_pl).title);
						result.push_back('"');

						result.push_back(':');

						result.append(map.at(tmp_idx_pl).content);
						result.push_back(',');
					}
					if (map.at(tmp_idx_pl).type == str) {
						result.push_back('"');
						result.append(map.at(tmp_idx_pl).title);
						result.push_back('"');

						result.push_back(':');

						result.push_back('"');
						result.append(map.at(tmp_idx_pl).content);
						result.push_back('"');
						result.push_back(',');
					}
				}
				if (map.at(tmp_idx_pl).type == pair_list) {
					JSON_Serialize_Child(map, result, tmp_idx_pl);
					//result.append("PairList");
				}
				if (map.at(tmp_idx_pl).type == dimension_list) {
					JSON_Serialize_Child(map, result, tmp_idx_pl);
					if (tmp_idx_pl != map.at(current_root).Child_idx.size() - 1) {
						result.push_back(',');
					}
				}
			}
			if (result.back() == ',') {
				result.pop_back();
			}
			result.push_back('}');
			result.push_back(',');
		}



		//result.pop_back();

		return 1;
	}
	int JSON_Serialize_Pool(json_acc_str_np::json_pool_str& map, std::string& result, int current_root = 0) {
		result.push_back('{');
		int target_idx = 0;
		for (int i = 0; i < map.size(); i++) {
			if (map.at(i).Father_idx == 0) {
				target_idx = i;
				// << i << "\n";
				json_acc_str_np::JSON_Serialize_Child(map, result, i);
			}
		}
		if (result.back() == ',') {
			result.pop_back();
		}
		result.push_back('}');
		return 1;
	}


#endif
}
