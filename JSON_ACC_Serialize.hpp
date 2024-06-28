#include "JSON_Base.h"
#include "JSON_ACC_Pool_Str.hpp"
#include <thread>
#pragma once
#ifndef __ACC_SER_CXX_
#define __ACC_SER_CXX_

namespace json_acc_str_np {

	//int JSON_Serialize_Child(json_acc_str_np::json_pool_str& map, std::string& result, int current_root = 0);

	int JSON_Serialize_Child(json_acc_str_np::json_pool_str& map, std::string& result, int current_root = 0) {
		if (map.at(current_root).type != notype && map.at(current_root).type != dimension_list && map.at(current_root).type != pair_list) {
			result.append(map.at(current_root).title);
			result.push_back(':');
			result.append(map.at(current_root).content);
			result.push_back(',');
		}
		if (map.at(current_root).type == dimension_list) {
			int tmp_idx_dm = 0;
			result.push_back('[');
			for (int i = 0; i < map.at(current_root).Child_idx.size(); i++) {
				tmp_idx_dm = map.at(current_root).Child_idx.at(i);
				if (map.at(tmp_idx_dm).type != dimension_list && map.at(tmp_idx_dm).type != pair_list && map.at(tmp_idx_dm).type != notype) {
					result.append(map.at(tmp_idx_dm).content);
					if (i != map.at(current_root).Child_idx.size() - 1) {
						result.push_back(',');
					}
				}
				if (map.at(tmp_idx_dm).type == dimension_list) {
					JSON_Serialize_Child(map, result, tmp_idx_dm);
				}
				if (map.at(tmp_idx_dm).type == pair_list) {
					result.append("{");
					JSON_Serialize_Child(map, result, tmp_idx_dm);
					result.append("},");
				}
			}
			result.append("],");
		}
		if (map.at(current_root).type == pair_list) {
			int tmp_idx_pl = 0;
			result.append(":{");
			if (map.at(current_root).type != notype && map.at(current_root).type != dimension_list && map.at(current_root).type != pair_list) {

				if (map.at(tmp_idx_pl).type != dimension_list && map.at(tmp_idx_pl).type != pair_list && map.at(tmp_idx_pl).type != notype) {
					for (int i = 0; i < map.at(current_root).Child_idx.size(); i++) {
						tmp_idx_pl = map.at(current_root).Child_idx.at(i);
						JSON_Serialize_Child(map, result, tmp_idx_pl);
					}
				}

			}
			result.append("},");

		}

		return 1;
	}
	int JSON_Serialize_Pool(json_acc_str_np::json_pool_str& map, std::string& result, int current_root = 0) {
		result.push_back('{');

	}
}
#endif
