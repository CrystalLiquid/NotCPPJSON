#include <cctype>
#include <cmath>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

#pragma once
#define LayerS '{'
#define LayerE '}'
#define Quote '"'
#define ConE ','
#define FieldS '['
#define FieldE ']'
#define ConS ':'

namespace json_acc_layer_np {
// 用string来类型擦除，返回variant还原类型
struct json_map;
struct json;
#define function_define
int PairList_Expect_Pool(std::string& data, json_map& map, int current_root_idx,
                         int beginpos, int current_layer);
int DimensionArray_Expect_Pool(std::string& data, json_map& map,
                               int current_root_idx, int beginpos,
                               int current_layer);
int JSON_Parse_Pool(json_map& map, std::string& data);
int JSON_Serialize_Pool(json_map& map, std::string& result, int current_root);
#define over_defining
enum data_type {
  temp_digit = -2,
  str = 1,
  digit_int = 5,
  null = 6,
  digit_double = 7,
  object_list = 8,
  object_void = 12,
  bool_t = 9,
  array_void = 10,
  array_list = 11,
  error = -1,
  notype = 0,
  invalid_opttype = -124
};

enum search_t { directly = 1, dfs = 2, bfs = 3 };

using i64t = long long;
using child_slice = std::vector<int>;

struct json {
 public:
  std::string key;             // 32
  std::string value;           // 32
  int type{notype};            // 4
  int layer{-1};               // 4
  int Father_idx{0};           // 4
  std::vector<int> Child_idx;  // 24//total:64+8+24=96
  void clear() {
    key.clear();
    value.clear();
    type = notype;
    Father_idx = 0;
    Child_idx.clear();
  }

  std::variant<i64t, std::string, double, bool, child_slice, std::monostate>
  get_val() {
    switch (this->type) {
      case notype:
        return std::string("");
        break;
      case str:
        return this->value;
        break;
      case digit_int:
        return std::stoll(this->value);
        break;
      case digit_double:
        return std::stod(this->value);
        break;
      case array_list:
        return this->Child_idx;
        break;
      case object_list:
        return this->Child_idx;
        break;
      case array_void:
        return std::string("[]");
        break;
      case object_void:
        return std::string("{}");
        break;
      case bool_t:
        if (this->value == "true") {
          return true;
        }
        if (this->value == "false") {
          return false;
        }
        break;
      case null:
        return std::string("null");
        break;
      default:
        // TODO
        break;
    }
    return std::monostate{};
  }
};

struct json_map : public std::vector<json> {
 private:  ///////////////////////////////////////////searching
           /// algorithm/////////////////////////////////////
  int f_bfs_(int root_idx, const json_map& pmap, std::string& key,
             int layer) {  // 广度 递归遍历 通过私有函数实现
    int temp_child_idx = 0;
    int target_idx = -200;
    for (int i = 0; i < (int)(pmap).at(root_idx).Child_idx.size(); i++) {
      temp_child_idx = (pmap).at(root_idx).Child_idx.at(i);
      if ((pmap).at(temp_child_idx).key == key &&
          (pmap).at(temp_child_idx).layer == layer) {
        target_idx = temp_child_idx;
        break;
      }
      target_idx = f_bfs_(temp_child_idx, pmap, key, layer);
      if (target_idx != -200) {
        break;
      }
    }
    return target_idx;
  };
  int f_dfs_c_(int current_idx, const json_map& pmap, std::string& key,
               int layer) {
    int current_opt_idx = current_idx;
    int current_father = 0;
    int target_idx = -200;
    int child = 0;
    while (target_idx == -200) {
      while (child < (int)pmap.at(current_father)
                         .Child_idx.size()) {  // TODO forward to child
        current_father = current_opt_idx;
        current_opt_idx = pmap.at(current_opt_idx).Child_idx.at(child);
        if (pmap.at(current_opt_idx).key != key) {
          target_idx = current_opt_idx;
          break;
        }
      }
      child++;
      if ((int)pmap.at(current_father).Child_idx.size() - 1 >
          child) {  // back to father
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

 public:  ///////////////////////////////////////////get
          /// value////////////////////////////////////
  int getidx_by_name_layer(std::string key, int layer) {
    int target_idx = 0;
    int i = 0;
    int rt = 0;
    for (; i < this->size() - 1;) {
      if (this->at(i).key != key) {
        ++i;
      }
      if (this->at(i).key == key && this->at(i).layer == layer) {
        target_idx = i;
        break;
      }
    }
    return target_idx;
  }
  json& getval_by_name(std::string key, int search_type = search_t::directly,
                       int layer = -1) {
    int target_idx = 0;
    if (search_type == search_t::directly) {
      int i = 0;
      for (; i < this->size() - 1;) {
        if (this->at(i).key != key) {
          ++i;
        }
        if (this->at(i).key == key && this->at(i).layer == layer) {
          target_idx = i;
          break;
        }
      }
    }
    if (search_type == search_t::bfs) {
      target_idx = f_bfs_(0, *this, key, layer);
    }
    if (search_type == search_t::dfs) {
      target_idx = f_dfs_c_(0, *this, key, layer);
    }
    return this->at(target_idx);
  }

 private:  ///////////////////////////////////////////return
           /// type/////////////////////////////////////
  int variant_return_type(
      std::variant<i64t, std::string, double, bool, std::monostate> val =
          std::monostate{}) {
    switch (val.index()) {
      case 0:
        return digit_int;
        break;
      case 1:
        if (std::get<std::string>(val) != "[]" &&
            std::get<std::string>(val) != "{}") {
          return str;
        }
        if (std::get<std::string>(val) == "[]") {
          return array_void;
        }
        if (std::get<std::string>(val) == "{}") {
          return object_void;
        }
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
        throw("error idx!");
        break;
    }
  }

 private:  ////////////////////////////////////////delete
           /// private////////////////////////////////////
  void delete_child(int current_idx) {
    for (int i = 0;
         i < (int)this->at(this->at(current_idx).Father_idx).Child_idx.size();
         ++i) {
      if (this->at(this->at(current_idx).Father_idx).Child_idx.at(i) ==
          current_idx) {
        this->at(this->at(current_idx).Father_idx)
            .Child_idx.erase(
                this->at(this->at(current_idx).Father_idx).Child_idx.begin() +
                i);
      }
    }
    if (!this->at(current_idx).Child_idx.empty()) {
      for (int i; i < this->at(current_idx).Child_idx.size(); ++i) {
        delete_child(this->at(current_idx).Child_idx.at(i));
      }
      this->erase(this->begin() + this->at(current_idx).Child_idx.front(),
                  this->begin() + this->at(current_idx).Child_idx.back());
      this->erase(this->begin() + current_idx);
    }
    if (this->at(current_idx).Child_idx.empty()) {
      this->erase(this->begin() + current_idx);
    }
  };

 public:  // delete func//
  void delete_idx(int idx) { delete_child(idx); }
  void delete_name(std::string key, int layer = -1) {
    int idx = getidx_by_name_layer(key, layer);
    delete_child(idx);
  }

 public:  /////////////////////////////////////add
          /// node:add_xx///////////////////////////////////////
  void add_val_atback(
      std::string&& key,
      std::variant<i64t, std::string, double, bool, std::monostate> val =
          std::monostate{}) {
    int vtype = variant_return_type(val);
    json buf = {key, "", vtype, 0, 0, {}};
    switch (vtype) {
      case notype:

        break;
      case str:
        buf.value = std::get<std::string>(val);
        break;

      case digit_int:
        buf.value = std::to_string(std::get<i64t>(val));
        break;

      case digit_double:
        buf.value = std::to_string(std::get<double>(val));
        break;

      case array_void:
        buf.value = "[]";
        break;

      case object_void:
        buf.value = "{}";
        break;

      case bool_t:
        buf.value = std::to_string(std::get<bool>(val));
        ;
        break;

      case null:
        buf.value = "null";
        break;

      default:
        std::cerr << "Fatal Type To Change!\n";
        return;
        break;
    }
    this->emplace_back(buf);
  }
  void add_val_aschild(
      std::string&& father_key, std::string&& key,
      std::variant<i64t, std::string, double, bool, std::monostate> val =
          std::monostate{},
      int layer = -1) {
    int father_idx = getidx_by_name_layer(father_key, layer - 1);
    int vtype = variant_return_type(val);
    json buf = {key, "", vtype, layer, father_idx, {}};
    switch (vtype) {
      case notype:

        break;
      case str:
        buf.value = std::get<std::string>(val);
        break;

      case digit_int:
        buf.value = std::to_string(std::get<i64t>(val));
        break;

      case digit_double:
        buf.value = std::to_string(std::get<double>(val));
        break;

      case array_void:
        buf.value = "[]";
        break;

      case object_void:
        buf.value = "{}";
        break;

      case bool_t:
        buf.value = std::to_string(std::get<bool>(val));
        ;
        break;

      case null:
        buf.value = "null";
        break;

      default:
        std::cerr << "Fatal Type To Change!\n";
        return;
        break;
    }
    this->emplace_back(buf);
  }
  void add_dimension_aschild(std::string&& key,
                             std::initializer_list<json> list, int layer = -1) {
    int grandfather_idx = getidx_by_name_layer(key, layer - 1);
    json buf = {key, "", array_list, layer, grandfather_idx, {}};
    this->emplace_back(buf);
    int father_idx = this->size() - 1;
    int tmp_child_idx = 0;
    for (json x : list) {
      this->emplace_back(x);
      this->back().Father_idx = father_idx;
      tmp_child_idx = (int)this->size() - 1;
      this->at(father_idx).Child_idx.emplace_back(tmp_child_idx);
    }
  }
  void add_pairlist_aschild(std::string&& key, std::initializer_list<json> list,
                            int layer = -1) {
    int grandfather_idx = getidx_by_name_layer(key, layer - 1);
    json buf = {key, "", object_list, layer, grandfather_idx, {}};
    this->emplace_back(buf);
    int father_idx = this->size() - 1;
    int tmp_child_idx = 0;
    for (json x : list) {
      this->emplace_back(x);
      this->back().Father_idx = father_idx;
      tmp_child_idx = (int)this->size() - 1;
      this->at(father_idx).Child_idx.emplace_back(tmp_child_idx);
    }
  }
  void add_dimension_atback(std::string&& key,
                            std::initializer_list<json> list) {
    json buf = {key, "", array_list, 0, 0, {}};
    this->emplace_back(buf);
    int father_idx = this->size() - 1;
    int tmp_child_idx = 0;
    for (json x : list) {
      this->emplace_back(x);
      this->back().Father_idx = father_idx;
      tmp_child_idx = (int)this->size() - 1;
      this->at(father_idx).Child_idx.emplace_back(tmp_child_idx);
    }
  }
  void add_pairlist_atback(std::string&& key,
                           std::initializer_list<json> list) {
    json buf = {key, "", object_list, 0, 0, {}};
    this->emplace_back(buf);
    int father_idx = this->size() - 1;
    int tmp_child_idx = 0;
    for (json x : list) {
      this->emplace_back(x);
      this->back().Father_idx = father_idx;
      tmp_child_idx = (int)this->size() - 1;
      this->at(father_idx).Child_idx.emplace_back(tmp_child_idx);
    }
  }

 public:  /////////////////////////////////////change
          /// type:set_xx//////////////////////////////////
  void set_dimension(std::string&& key, std::initializer_list<json> list,
                     int layer = -1) {
    int father_idx = getidx_by_name_layer(key, layer);
    this->at(father_idx).type = array_list;
    this->at(father_idx).value.clear();
    int i = 1;
    for (json x : list) {
      x.Father_idx = father_idx;
      this->insert(this->begin() + father_idx + i, x);
      this->at(father_idx).Child_idx.push_back(father_idx + i);
      ++i;
    }
  }
  void set_pairlist(std::string&& key, std::initializer_list<json> list,
                    int layer = -1) {
    int father_idx = getidx_by_name_layer(key, layer);
    this->at(father_idx).type = object_list;
    this->at(father_idx).value.clear();
    int i = 1;
    for (json x : list) {
      x.Father_idx = father_idx;
      this->insert(this->begin() + father_idx + i, x);
      this->at(father_idx).Child_idx.push_back(father_idx + i);
      ++i;
    }
  }
  void set_val(std::string&& key,
               std::variant<i64t, std::string, double, bool, std::monostate>
                   val = std::monostate{},
               int layer = -1) {
    int idx = getidx_by_name_layer(key, layer);
    int vtype = variant_return_type(val);
    int begin_i = -1;
    int end_i = -1;
    switch (this->at(idx).type) {
      case notype:
        break;

      case str:
        this->at(idx).value.clear();
        break;

      case digit_int:
        this->at(idx).value.clear();
        break;

      case digit_double:
        this->at(idx).value.clear();
        break;

      case array_list:
        begin_i = this->at(idx).Child_idx.front();
        end_i = this->at(idx).Child_idx.back();
        this->at(idx).Child_idx.clear();
        this->erase(this->begin() + begin_i, this->begin() + end_i);
        break;

      case object_list:
        begin_i = this->at(idx).Child_idx.front();
        end_i = this->at(idx).Child_idx.back();
        this->at(idx).Child_idx.clear();
        this->erase(this->begin() + begin_i, this->begin() + end_i);
        break;

      case array_void:
        this->at(idx).value.clear();
        break;

      case object_void:
        this->at(idx).value.clear();
        break;

      case bool_t:
        this->at(idx).value.clear();
        break;

      case null:
        this->at(idx).value.clear();
        break;

      default:
        std::cerr << "Fatal Type To Clean!\n";
        return;
        break;
    }
    this->at(idx).type = notype;
    this->at(idx).type = vtype;
    switch (vtype) {
      case notype:
        break;

      case str:

        this->at(idx).value = std::get<std::string>(val);
        break;

      case digit_int:
        this->at(idx).value = std::to_string(std::get<i64t>(val));
        break;

      case digit_double:
        this->at(idx).value = std::to_string(std::get<double>(val));
        break;

      case array_void:
        this->at(idx).value = "[]";
        break;

      case object_void:
        this->at(idx).value = "{}";
        break;

      case bool_t:
        this->at(idx).value = std::to_string(std::get<bool>(val));
        ;
        break;

      case null:
        this->at(idx).value = "null";
        break;

      default:
        std::cerr << "Fatal Type To Change!\n";
        return;
        break;
    }
  }

 public:
  int PairList_Expect_Pool(std::string& data, json_map& map,
                           int current_root_idx, int beginpos = 0,
                           int current_layer = -1) {  ///////有问题，会跳读
    // std::cout << "\n";
    json buf;
    int j = 0;
    int k = 0;
    int i = 0;
    int t = 0;
    int layer = 0;

    for (i = beginpos + 1; data[i] != LayerE; i++) {
      if (data[i] == ConS) {
        buf.Father_idx = current_root_idx;
        buf.layer = current_layer;
        for (t = i; data[t] != ConE && data[t] != LayerS; t--)
          ;
        for (k = t; data[k] != ConS; k++) {
          if (data[k] != LayerS && data[k] != FieldS && data[k] != ConE &&
              data[k] != Quote) {
            buf.key.push_back(data[k]);
          }
        }
        if (data[i + 1] == 'n' && data[i + 2] == 'u' && data[i + 3] == 'l' &&
            data[i + 4] == 'l') {
          buf.value = "null";
          buf.type = null;

          map.emplace_back(buf);
          map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

          buf.clear();
        }
        if (data[i + 1] == 't' && data[i + 2] == 'r' && data[i + 3] == 'u' &&
            data[i + 4] == 'e') {  // bool true
          buf.value = "true";
          buf.type = bool_t;

          map.emplace_back(buf);
          map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

          buf.clear();
        }
        if (data[i + 1] == 'f' && data[i + 2] == 'a' && data[i + 3] == 'l' &&
            data[i + 4] == 's' && data[i + 5] == 'e') {  // bool false
          buf.value = "false";
          buf.type = bool_t;

          map.emplace_back(buf);
          map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

          buf.clear();
        }
        if (data[i + 1] == Quote) {  // str
          buf.type = str;

          for (j = i + 2; (data[j + 1] != ConE || data[j] != Quote) &&
                          (data[j + 1] != LayerE || data[j] != Quote);
               ++j) {
            buf.value.push_back(data[j]);
          }
          //-------buf.value.push_back(Quote);

          map.emplace_back(buf);
          map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

          buf.clear();
          // std::cout << data[j] << "  ssssssss\n";
          i = j - 1;
          j = 0;
        }
        if (std::isdigit(data[i + 1]) ||
            (data[i + 1] == '-' && std::isdigit(data[i + 2]))) {  // digit_all
          // std::cout << "DIGIT" << "\n";
          buf.type = digit_int;
          for (j = i + 1; data[j] != ConE && data[j] != LayerE; j++) {
            if (data[j] == '.') {
              buf.type = digit_double;
            }
            buf.value.push_back(data[j]);
          }

          map.emplace_back(buf);
          map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

          buf.clear();
          i = j - 1;
          j = 0;
        }
        //////////////////////////////////////////////////////////////////
        if (data[i + 1] == FieldS && data[i + 2] == FieldE) {  // void list
          buf.type = array_void;
          buf.value = "[]";
          map.emplace_back(buf);
          map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

          buf.clear();
          i = i + 2;
        }
        if (data[i + 1] == LayerS &&
            data[i + 2] ==
                LayerE) {  // object_void {}   Object accelerate parsing
          buf.type = object_void;

          buf.value = "{}";
          map.emplace_back(buf);
          map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

          buf.clear();
          i = i + 2;
        }
        ///////////////////////
        if (data[i + 1] == FieldS) {  // Dimension List array
          buf.type = array_list;
          map.emplace_back(buf);
          map[current_root_idx].Child_idx.emplace_back(map.size() - 1);
          j = DimensionArray_Expect_Pool(data, map, map.size() - 1, i + 1,
                                         current_layer + 1);

          buf.clear();
          i = j;
        }
        /////////////////////////////////////////////////////////////////

        if (data[i + 1] == LayerS) {  // object_list {}   Object
          buf.type = object_list;
          map.emplace_back(buf);
          map[current_root_idx].Child_idx.emplace_back(map.size() - 1);
          j = PairList_Expect_Pool(data, map, map.size() - 1, i + 1,
                                   current_layer + 1);

          buf.clear();
          i = j;  // if it is j-1,it will result in catching last nesting
                  // "}",and idx will be wrong
          j = 0;
        }
      }
    }

    return i;  // letter Should Be LayerE,However,When There was a nest,It will
               // change to next "}" or "]"
  }

  int DimensionArray_Expect_Pool(std::string& data, json_map& map,
                                 int current_root_idx, int beginpos = 0,
                                 int current_layer = -1) {
    // std::cout << "IN_FUNC\n";
    json buf;
    int j = 0;
    int i = 0;

    for (i = beginpos; data[i] != FieldE; ++i) {
      if (data[i] == FieldS || data[i] == ConE) {
        buf.Father_idx = current_root_idx;
        buf.layer = current_layer;
        if (data[i + 1] == 'n' && data[i + 2] == 'u' && data[i + 3] == 'l' &&
            data[i + 4] == 'l') {
          buf.value = "null";
          buf.type = null;

          map.emplace_back(buf);
          map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

          buf.clear();
          i += 4;
        }
        if (data[i + 1] == 't' && data[i + 2] == 'r' && data[i + 3] == 'u' &&
            data[i + 4] == 'e') {  // bool true
          buf.value = "true";
          buf.type = bool_t;

          map.emplace_back(buf);
          map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

          buf.clear();
          i += 4;
        }
        if (data[i + 1] == 'f' && data[i + 2] == 'a' && data[i + 3] == 'l' &&
            data[i + 4] == 's' && data[i + 5] == 'e') {  // bool false
          buf.value = "false";
          buf.type = bool_t;

          map.emplace_back(buf);
          map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

          buf.clear();
          i += 5;
        }
        if (data[i + 1] == Quote) {  // str
          buf.type = str;
          for (j = i + 2; (data[j + 1] != ConE || data[j] != Quote) &&
                          (data[j + 1] != FieldE || data[j] != Quote);
               ++j) {
            buf.value.push_back(data[j]);
          }

          map.emplace_back(buf);
          map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

          buf.clear();

          i = j - 1;
          j = 0;
        }
        if (std::isdigit(data[i + 1]) or
            (data[i + 1] == '-' && std::isdigit(data[i + 2]))) {  // digit_all
          /////////////////////////////////////////////////////////////
          buf.type = digit_int;
          for (j = i + 1; data[j] != ConE && data[j] != FieldE; j++) {
            if (data[j] == '.') {
              buf.type = digit_double;
            }
            buf.value.push_back(data[j]);
          }

          map.emplace_back(buf);
          map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

          buf.clear();
          i = j - 1;
          j = 0;
        }

        /////////////////////////////////////////////////////////////////////////////////
        if (data[i + 1] == FieldS && data[i + 2] == FieldE) {  // void list
          buf.type = array_void;
          buf.value = "[]";
          map.emplace_back(buf);
          map[current_root_idx].Child_idx.emplace_back(map.size() - 1);
          buf.clear();
          i = i + 2;
        }

        if (data[i + 1] == FieldS) {  // Dimension List
          buf.type = array_list;

          map.emplace_back(buf);
          map[current_root_idx].Child_idx.emplace_back(map.size() - 1);
          j = DimensionArray_Expect_Pool(data, map, map.size() - 1, i + 1,
                                         current_layer + 1);

          buf.clear();

          i = j;
        }

        /////////////////////////////////////////////////////////////////////////////

        if (data[i + 1] == LayerS &&
            data[i + 2] == LayerE) {  // object_void {}   Object
          buf.type = object_void;

          buf.value = "{}";
          map.emplace_back(buf);
          map[current_root_idx].Child_idx.emplace_back(map.size() - 1);

          buf.clear();
          i = i + 2;
        }

        if (data[i + 1] == LayerS) {  // object_list {} Object///////////////////
          buf.type = object_list;

          map.emplace_back(buf);
          map[current_root_idx].Child_idx.emplace_back(map.size() - 1);
          j = PairList_Expect_Pool(data, map, map.size() - 1, i + 1,
                                   current_layer + 1);

          buf.clear();

          i = j - 1;  // LayerE
          j = 0;
        }
      }
    }

    return i;
  }

  int JSON_Parse_Pool(json_map& map, std::string&& data) {
    json buf;
    int j = 0;
    int k = 0;
    int t = 0;

    for (int i = 0; i < (int)(data.length() - 1); ++i) {
      if (data[i] == ConS) {
        /// title
        for (t = i; data[t] != ConE && data[t] != LayerS; t--) {
        }
        for (k = t + 1; data[k] != ConS; k++) {
          if (data[k] != Quote) {
            buf.key.push_back(data[k]);
          }
        }
        buf.Father_idx = 0;
        buf.layer = 1;
        if (data[i + 1] == 'n' && data[i + 2] == 'u' && data[i + 3] == 'l' &&
            data[i + 4] == 'l') {
          buf.value = "null";
          buf.type = null;

          map.emplace_back(buf);
          buf.clear();
        }
        if (data[i + 1] == 't' && data[i + 2] == 'r' && data[i + 3] == 'u' &&
            data[i + 4] == 'e') {  // bool true
          buf.value = "true";
          buf.type = bool_t;
          map.emplace_back(buf);
          buf.clear();
        }
        if (data[i + 1] == 'f' && data[i + 2] == 'a' && data[i + 3] == 'l' &&
            data[i + 4] == 's' && data[i + 5] == 'e') {  // bool false
          buf.value = "false";
          buf.type = bool_t;
          map.emplace_back(buf);
          buf.clear();
        }
        if (data[i + 1] == Quote) {  // str
          buf.type = str;
          for (j = i + 2; (data[j + 1] != ConE || data[j] != Quote) &&
                          (data[j + 1] != LayerE || data[j] != Quote);
               j++) {
            buf.value.push_back(data[j]);
          }
          map.emplace_back(buf);
          buf.clear();
          i = j - 1;
          j = 0;
        }
        if (std::isdigit(data[i + 1]) ||
            (data[i + 1] == '-' && std::isdigit(data[i + 2]))) {  // digit_all
          buf.type = digit_int;
          for (j = i + 1; data[j] != ConE && data[j] != LayerE; j++) {
            if (data[j] == '.') {
              buf.type = digit_double;
            }
            buf.value.push_back(data[j]);
          }
          map.emplace_back(buf);
          buf.clear();
          i = j - 1;
          j = 0;
        }
        /////////////////////////////////////////////////////////////////////////////////
        if (data[i + 1] == FieldS && data[i + 2] == FieldE) {  // void list
          buf.type = array_void;
          buf.value = "[]";

          map.emplace_back(buf);
          buf.clear();
          i = i + 2;
        }
        if (data[i + 1] == LayerS && data[i + 2] == LayerE) {  // void list
          buf.type = object_void;
          buf.value = "{}";

          map.emplace_back(buf);
          buf.clear();
          i = i + 2;
        }

        if (data[i + 1] == FieldS) {  // Dimension List
          buf.type = array_list;
#define Main_Dimension
          map.emplace_back(buf);
          j = DimensionArray_Expect_Pool(data, map, map.size() - 1, i + 1, 1);
          // std::cout << "Dimen:" << data[i + 1] << "\n";

          buf.clear();
          i = j;
        }

        /////////////////////////////////////////////////////////////////////////////
        if (data[i + 1] == LayerS) {  // object_list {}   Object
          buf.type = object_list;

#define Main_Pair
          map.emplace_back(buf);
          j = PairList_Expect_Pool(data, map, map.size() - 1, i + 1, 1);

          buf.clear();
          i = j - 1;
          // std::cout << "                                       " << data[i]
          // << "\n";
          j = 0;
        }
      }
    }
    return 1;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  int JSON_Serialize_Child(json_map& map, std::string& result,
                           int current_root = 0) {
    if (map.at(current_root).type != notype &&
        map.at(current_root).type != array_list &&
        map.at(current_root).type != object_list) {
      if (map.at(current_root).type != str) {
        result.push_back('"');
        result.append(map.at(current_root).key);
        result.push_back('"');

        result.push_back(':');

        result.append(map.at(current_root).value);
        result.push_back(',');
      }
      if (map.at(current_root).type == str) {
        result.push_back('"');
        result.append(map.at(current_root).key);
        result.push_back('"');

        result.push_back(':');

        result.push_back('"');
        result.append(map.at(current_root).value);
        result.push_back('"');
        result.push_back(',');
      }
    }
    if (map.at(current_root).type == array_list) {
      int tmp_idx_dm = 0;
      if (!map.at(current_root).key.empty()) {
        result.append("\"");
        result.append(map.at(current_root).key);
        result.push_back('"');
      }

      if (map.at(map.at(current_root).Father_idx).type ==
          array_list) {  // if its father type is dimension,it might broke
                             // json syntax
        result.append("[");
      }
      if (map.at(map.at(current_root).Father_idx).type != array_list) {
        result.append(":[");
      }
      for (int i = 0; i < map.at(current_root).Child_idx.size(); i++) {
        tmp_idx_dm = map.at(current_root).Child_idx.at(i);
        if (map.at(tmp_idx_dm).type != array_list &&
            map.at(tmp_idx_dm).type != object_list &&
            map.at(tmp_idx_dm).type != notype) {
          if (map.at(tmp_idx_dm).type == str) {
            result.push_back('"');
            result.append(map.at(tmp_idx_dm).value);
            result.push_back('"');
          }
          if (map.at(tmp_idx_dm).type != str) {
            result.append(map.at(tmp_idx_dm).value);
          }
          if (i != map.at(current_root).Child_idx.size() - 1) {
            result.push_back(',');
          }
        }
        if (map.at(tmp_idx_dm).type == array_list) {
          JSON_Serialize_Child(map, result, tmp_idx_dm);
          if (i != map.at(current_root).Child_idx.size() - 1) {
            result.push_back(',');
          }
        }
        if (map.at(tmp_idx_dm).type == object_list) {
          JSON_Serialize_Child(map, result, tmp_idx_dm);
        }
      }
      if (result.back() == ',') {
        result.pop_back();
      }
      result.append("]");
      if (map.at(current_root).Father_idx == 0) {
        result.push_back(',');
      }
    }
    if (map.at(current_root).type == object_list) {
      int tmp_idx_pl = 0;

      if (!map.at(current_root).key.empty()) {
        result.append("\"");
        // result.push_back('"');
        result.append(map.at(current_root).key);
        result.push_back('"');
      }

      if (!map.at(current_root).key.empty()) {
        result.append(":");
      }
      result.append("{");
      int i = 0;
      for (; i < (int)map.at(current_root).Child_idx.size(); i++) {
        tmp_idx_pl = map.at(current_root).Child_idx.at(i);
        if (map.at(tmp_idx_pl).type != array_list &&
            map.at(tmp_idx_pl).type != object_list &&
            map.at(tmp_idx_pl).type != notype) {
          // result.pop_back();
          if (map.at(tmp_idx_pl).type != str) {
            result.push_back('"');
            result.append(map.at(tmp_idx_pl).key);
            result.push_back('"');

            result.push_back(':');

            result.append(map.at(tmp_idx_pl).value);
            result.push_back(',');
          }
          if (map.at(tmp_idx_pl).type == str) {
            result.push_back('"');
            result.append(map.at(tmp_idx_pl).key);
            result.push_back('"');

            result.push_back(':');

            result.push_back('"');
            result.append(map.at(tmp_idx_pl).value);
            result.push_back('"');
            result.push_back(',');
          }
        }
        if (map.at(tmp_idx_pl).type == object_list) {
          JSON_Serialize_Child(map, result, tmp_idx_pl);
          // result.append("PairList");
        }
        if (map.at(tmp_idx_pl).type == array_list) {
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

    // result.pop_back();

    return 1;
  }
  int JSON_Serialize_Pool(json_map& map, std::string& result,
                          int current_root = 0) {
    result.push_back('{');
    int target_idx = 0;
    for (int i = 0; i < map.size(); i++) {
      if (map.at(i).Father_idx == 0) {
        target_idx = i;
        // << i << "\n";
        JSON_Serialize_Child(map, result, i);
      }
    }
    if (result.back() == ',') {
      result.pop_back();
    }
    result.push_back('}');
    return 1;
  }

 public:
  void parse(std::string& data) { JSON_Parse_Pool(*this, std::move(data)); }
  std::string serialize() {
    std::string buf;
    JSON_Serialize_Pool(*this, buf, 0);
    return buf;
  }

 private:
  struct iter_info {
    struct key {
      int begin{0};
      int end{0};
    };
    struct val {
      int begin{0};
      int end{0};
    };
    int layer_through{0};
    int field_through{0};
  };

 public:
  void iter_parse(std::string& data);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Debug_Print(std::string& data, int begin_idx, int end_idx) {
  for (int i = begin_idx; i <= end_idx; i++) {
    std::cout << data[i];
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void json_map::iter_parse(std::string& data) {
  json buf;
  json_map& map = *this;
  int layer = 0;
  int object = 0;
  std::vector<json_map::iter_info> infos;
  for (int i = 0; i < (int)(data.length() - 1); ++i) {
  }
}

}  // namespace json_acc_layer_np
using json_map = json_acc_layer_np::json_map;
using json = json_acc_layer_np::json;