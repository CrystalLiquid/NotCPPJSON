#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <execution>
#include <initializer_list>
#include <iostream>
#include <numeric>
#include <string>
#include <system_error>
#include <unordered_set>
#include <utility>
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
enum class data_type : int32_t
{
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

using i64t = long long;
using child_slice = std::vector<int>;

struct json
{
    using input_val =
        std::variant<long long, std::string, double, bool, std::monostate>;

public:
    std::string key;                   // 32
    std::string value;                 // 32
    data_type type{data_type::notype}; // 4
    int layer{-1};                     // 4
    int Father_idx{0};                 // 4
    std::vector<int> Child_idx;        // 24//total:64+8+24=96
    void clear()
    {
        key.clear();
        value.clear();
        type = data_type::notype;
        Father_idx = 0;
        Child_idx.clear();
    }

    json operator()(std::string key, input_val val)
    {
        json buf;
        buf.key = key;
        switch (val.index())
        {
            case 0:
                buf.type = json_acc_layer_np::data_type::digit_int;
                buf.value = std::to_string(std::get<long long>(val));
                break;
            case 1:
                if (std::get<std::string>(val) != "[]" &&
                    std::get<std::string>(val) != "{}")
                {
                    buf.type = json_acc_layer_np::data_type::str;
                }
                if (std::get<std::string>(val) == "[]")
                {
                    buf.type = json_acc_layer_np::data_type::array_void;
                }
                if (std::get<std::string>(val) == "{}")
                {
                    buf.type = json_acc_layer_np::data_type::object_void;
                }
                buf.value = std::get<std::string>(val);
                break;
            case 2:
                buf.type = json_acc_layer_np::data_type::digit_double;
                buf.value = std::to_string(std::get<double>(val));
                break;
            case 3:
                buf.type = json_acc_layer_np::data_type::bool_t;
                buf.value = std::to_string(std::get<bool>(val));
                break;
            case 4: buf.type = json_acc_layer_np::data_type::notype; break;
            default: throw("error idx!"); break;
        }
        buf.layer = 0;
        return std::move(buf);
    }
    std::variant<i64t, std::string, double, bool, child_slice, std::monostate>
    get_val()
    {
        switch (this->type)
        {
            case data_type::notype: return std::string(""); break;
            case data_type::str: return this->value; break;
            case data_type::digit_int: return std::stoll(this->value); break;
            case data_type::digit_double: return std::stod(this->value); break;
            case data_type::array_list: return this->Child_idx; break;
            case data_type::object_list: return this->Child_idx; break;
            case data_type::array_void: return std::string("[]"); break;
            case data_type::object_void: return std::string("{}"); break;
            case data_type::bool_t:
                if (this->value == "true")
                {
                    return true;
                }
                if (this->value == "false")
                {
                    return false;
                }
                break;
            case data_type::null: return std::string("null"); break;
            default:
                // TODO
                break;
        }
        return std::monostate{};
    }
};

struct json_map : public std::vector<json>
{
    using input_val =
        std::variant<long long, std::string, double, bool, std::monostate>;
    struct slice : public std::vector<int>
    {
        void print_size_of()
        {
            std::cout << this->size() << "\n";
        }

        struct comp_result
        {
            json_map* map;
            slice* sl;
            int which_from_op{-1};
            slice list_of()
            {
                slice buf;
                buf.sresult.map = map;
                for (auto x : map->at(sl->at(which_from_op)).Child_idx)
                {
                    buf.emplace_back(x);
                }
                return buf;
            }
            json& self()
            {
                return map->at(sl->at(which_from_op));
            }
            void set_val(std::variant<i64t, std::string, double, bool, std::monostate> val = std::monostate{})
            {
                int idx = which_from_op;
                data_type vtype = map->variant_return_type(val);
                int begin_i = -1;
                int end_i = -1;
                switch (map->at(idx).type)
                {
                    case data_type::notype: break;

                    case data_type::str: map->at(idx).value.clear(); break;

                    case data_type::digit_int:
                        map->at(idx).value.clear();
                        break;

                    case data_type::digit_double:
                        map->at(idx).value.clear();
                        break;

                    case data_type::array_list:
                        begin_i = map->at(idx).Child_idx.front();
                        end_i = map->at(idx).Child_idx.back();
                        map->at(idx).Child_idx.clear();
                        map->erase(map->begin() + begin_i,
                                   map->begin() + end_i);
                        break;

                    case data_type::object_list:
                        begin_i = map->at(idx).Child_idx.front();
                        end_i = map->at(idx).Child_idx.back();
                        map->at(idx).Child_idx.clear();
                        map->erase(map->begin() + begin_i,
                                   map->begin() + end_i);
                        break;

                    case data_type::array_void:
                        map->at(idx).value.clear();
                        break;

                    case data_type::object_void:
                        map->at(idx).value.clear();
                        break;

                    case data_type::bool_t: map->at(idx).value.clear(); break;

                    case data_type::null: map->at(idx).value.clear(); break;

                    default:
                        std::cerr << "Fatal Type To Clean!\n";
                        return;
                        break;
                }
                map->at(idx).type = data_type::notype;
                map->at(idx).type = vtype;
                switch (vtype)
                {
                    case data_type::notype: break;

                    case data_type::str:

                        map->at(idx).value = std::get<std::string>(val);
                        break;

                    case data_type::digit_int:
                        map->at(idx).value =
                            std::to_string(std::get<i64t>(val));
                        break;

                    case data_type::digit_double:
                        map->at(idx).value =
                            std::to_string(std::get<double>(val));
                        break;

                    case data_type::array_void:
                        map->at(idx).value = "[]";
                        break;

                    case data_type::object_void:
                        map->at(idx).value = "{}";
                        break;

                    case data_type::bool_t:
                        map->at(idx).value =
                            std::to_string(std::get<bool>(val));
                        ;
                        break;

                    case data_type::null: map->at(idx).value = "null"; break;

                    default:
                        std::cerr << "Fatal Type To Change!\n";
                        return;
                        break;
                }
            }
            data_type typeof_which()
            {
                return map->at(sl->at(which_from_op)).type;
            }

            void print_typeof()
            {
                switch (map->at(sl->at(which_from_op)).type)
                {
                    case data_type::notype: std::cout << "no_type"; break;
                    case data_type::str: std::cout << "str"; break;
                    case data_type::digit_int: std::cout << "digit_int"; break;
                    case data_type::digit_double:
                        std::cout << "digit_double";
                        break;
                    case data_type::array_list: std::cout << "array"; break;
                    case data_type::object_list: std::cout << "object"; break;
                    case data_type::array_void:
                        std::cout << "array_void";
                        break;
                    case data_type::object_void:
                        std::cout << "object_void";
                        break;
                    case data_type::bool_t: std::cout << "bool"; break;
                    case data_type::null: std::cout << "null"; break;
                    default:
                        // TODO
                        break;
                }
                std::cout << "\n";
            }
            void delete_which()
            {
                switch (map->at(sl->at(which_from_op)).type)
                {
                    case data_type::array_list:
                        map->specific_delete_for_this(sl->at(which_from_op));
                        return;
                        break;
                    case data_type::object_list:
                        map->specific_delete_for_this(sl->at(which_from_op));
                        return;
                        break;
                    default:
                        map->specific_delete_for_this(sl->at(which_from_op));
                        return;
                        break;
                }
            }
        } sresult;

        comp_result& operator[](const int which)
        {
            this->sresult.sl = this;
            this->sresult.which_from_op = which;
            return this->sresult;
        }
    };

public:
    slice find(std::string key_name)
    {
        slice buf;
        buf.sresult.map = this;

        int i = 0;
        if (!key_name.empty())
        {
            for (json& x : *this)
            {
                if (x.key == key_name)
                {
                    buf.emplace_back(i);
                }
                ++i;
            }
        }
        return buf;
    }
    ///////////////////////////////////////check_repeat/////////////////////////////////////////////////
private:
    /*
    bool is_repeat(std::string& key_name, int layerv) {
       if (!key_name.empty()) {
         int i = 0;
         for (json& x : *this) {
           if (x.key == key_name && x.layer == layerv) {
             return true;
           }
           ++i;
         }
       }
       return false;
     }
    */

    void check_repeat_atback(std::string& key_name)
    {
        if (!key_name.empty())
        {
            for (json& x : *this)
            {
                if (x.key == key_name && x.layer == 0)
                {
                    std::cerr << "Repeated Key!\n";
                    throw x;
                }
            }
        }
    }
    void check_repeat(std::string& key_name, int layerv)
    {
        if (layerv == -1)
        {
            throw this;
        }
        std::vector<json*> list;
        if (!key_name.empty())
        {
            for (json& x : *this)
            {
                if (x.key == key_name)
                {
                    list.emplace_back(&x);
                }
                if (x.key == key_name && x.layer == layerv)
                {
                    std::cerr << "Repeated Key To Add!\n";
                }
            }

            if (list.size() > 2)
            {
                for (auto x : list)
                {
                    if (x->layer == layerv)
                    {
                        std::cerr << "Repeated Key To Add!\n";
                        throw list;
                    }
                }
            }
            if (list.size() == 2)
            {
                if (list[0]->layer == list[1]->layer)
                {
                    std::cerr << "Repeated Key!\n";

                    throw list;
                }
                else
                {
                    for (auto x : list)
                    {
                        if (x->layer == layerv)
                        {
                            std::cerr << "Repeated Key To Add!\n";
                            throw list;
                        }
                    }
                }
            }
            if (list.size() == 1)
            {
                if (layerv == list[0]->layer)
                {
                    std::cerr << "Repeated Key To Add!\n";
                    throw list;
                }
            }
        }
    }

public:
    int get_idxl1(std::string keyv)
    {
        if (keyv.empty())
        {
            std::cerr << "Shall Not Be Empty! | get_idxl1\n";
            throw this;
        }
        int target_idx = -1;
        for (int i = 0; i < this->size() - 1;)
        {
            // std::cout<<this->at(i).key<<" | "<<this->at(i).value<<"  ^
            // "<<i<<"\n";
            if (this->at(i).key != keyv || this->at(i).layer != 0)
            {
                ++i;
            }
            if (this->at(i).key == keyv && this->at(i).layer == 1)
            {
                target_idx = i;
                if (target_idx >= this->size() - 1)
                {
                    std::cerr << "Error Search Index | get_idxl1\n";
                    throw this;
                }
                break;
            }
        }
        return target_idx;
    }
    int get_idxl(std::string keyv, int layerv)
    {
        if (keyv.empty())
        {
            std::cerr << "Shall Not Be Empty! | get_idxl\n";
            throw this;
        }
        int target_idx = 0;
        for (int i = 0; i < this->size() - 1;)
        {
            // std::cout<<this->at(i).key<<" | "<<this->at(i).value<<"  ^
            // "<<i<<"\n";
            if (this->at(i).key != keyv || this->at(i).layer != layerv)
            {
                ++i;
            }
            if (this->at(i).key == keyv && this->at(i).layer == layerv)
            {
                target_idx = i;
                if (target_idx >= this->size() - 1)
                {
                    std::cerr << "Error Search Index | get_idxl\n";
                    throw this;
                }
                break;
            }
        }
        return target_idx;
    }

private: ///////////////////////////////////////////get
         /// value////////////////////////////////////
private: ///////////////////////////////////////////return
         /// type/////////////////////////////////////
    data_type variant_return_type(std::variant<i64t, std::string, double, bool, std::monostate> val = std::monostate{})
    {
        switch (val.index())
        {
            case 0: return data_type::digit_int; break;
            case 1:
                if (std::get<std::string>(val) != "[]" &&
                    std::get<std::string>(val) != "{}")
                {
                    return data_type::str;
                }
                if (std::get<std::string>(val) == "[]")
                {
                    return data_type::array_void;
                }
                if (std::get<std::string>(val) == "{}")
                {
                    return data_type::object_void;
                }
                break;
            case 2: return data_type::digit_double; break;
            case 3: return data_type::bool_t; break;
            case 4: return data_type::notype; break;
            default: throw("error idx!"); break;
        }
        throw "error reachable!";
    }

private:
    bool is_between(int begin, int end, int target)
    {
        return (target > begin && target < end) ? true : false;
    }
    ////////////////////////////////////////delete
    /// private////////////////////////////////////
private: // delete func//
    void specific_delete_for_this(int idx)
    { // must ensure the min one in front and max one in back
    //std::cout<<"Del:"<<idx<<"\n";
        auto fix_after_index = [this](int idx, int offset_size) {
            for (int k = idx; k < this->size(); ++k)
            {
                if (this->at(k).Father_idx > idx)
                {
                    this->at(k).Father_idx -= offset_size;
                }
                for (int i = 0; i < this->at(k).Child_idx.size(); ++i)
                {
                    if (this->at(k).Child_idx.at(i) > idx)
                    {
                        this->at(k).Child_idx.at(i) -= offset_size;
                    }
                }
            }
        };

        int std_idx = idx;
        while (!this->at(std_idx).Child_idx.empty())
        {
            std_idx = this->at(std_idx).Child_idx.back();
        }
        int to_del_size = std_idx - idx + 1;
        if (to_del_size == 1)
        {
            this->erase(this->begin() + idx);
        }
        if (to_del_size > 1)
        {
            this->erase(this->begin() + idx, this->begin() + std_idx);
        }

        fix_after_index(idx, to_del_size);
    }
    void __nofix_delete_noChild(int idx)
    {
        if (this->at(idx).Child_idx.empty())
        {
            // std::cout<<this->size()<<"\n";
            // std::cout<<idx<<"\n";
            for (int i = 0;
                 i < this->at(this->at(idx).Father_idx).Child_idx.size(); ++i)
            {
                if (this->at(this->at(idx).Father_idx).Child_idx.at(i) == idx)
                {
                    this->at(this->at(idx).Father_idx)
                        .Child_idx.erase(
                            (this->at(this->at(idx).Father_idx).Child_idx)
                                .begin() +
                            i);
                }
            }

            this->erase(this->begin() + idx);

            return;
        }
        else
        {
            std::cerr << "Incorrect Type | delete_noChild"
                      << "\n";
            throw this;
        }
    }
    void fix_delete_hasChild(int idx)
    {
        if (this->at(idx).Child_idx.empty())
        {
            std::cerr << "Incorrect Type | delete_hasChild\n";
            throw this;
        }
        ///////////////////////////////////////////////////
        int size = this->at(idx).Child_idx.size();
        int child_begin = this->at(idx).Child_idx.front();
        int child_end = this->at(idx).Child_idx.back();
        //////////////////////////////////////////////////
        auto fix_after_index = [this](int idx, int offset_size) {
            for (int k = idx; k < this->size(); ++k)
            {
                if (this->at(k).Father_idx > idx)
                {
                    this->at(k).Father_idx -= offset_size;
                }
                for (int i = 0; i < this->at(idx).Child_idx.size(); ++i)
                {
                    if (this->at(k).Child_idx.at(i) > idx)
                    {
                        this->at(k).Child_idx.at(i) -= offset_size;
                    }
                }
            }
        };
        auto erase_info_in_father = [this](int idx) {
            for (int i = 0; i < this->at(this->at(idx).Father_idx).Child_idx.size(); ++i)
            {
                if (this->at(this->at(idx).Father_idx).Child_idx.at(i) == idx)
                {
                    this->at(this->at(idx).Father_idx).Child_idx.erase((this->at(this->at(idx).Father_idx).Child_idx).begin() + i);
                    return;
                }
            }
        };
        /////////////////////////////////////////////////
        if (!this->at(idx).Child_idx.empty())
        {
            std::vector<int> x;
            /////////////////////////////////////del-child////////////////
            for (int i = child_begin; i <= child_end;)
            {
                std::cout << "InLoop:" << i << "\n";
                if (this->at(i).Child_idx.empty())
                {
                    erase_info_in_father(i);
                    this->erase(this->begin() + i);
                    // fix_after_index(i, 1);
                    --child_end;
                }
                if (!this->at(i).Child_idx.empty())
                {
                    for (auto x : this->at(i).Child_idx)
                    {
                    }
                }
            }
            ////////////////////////////////////////del-info-in-fa and
            /// del-self/////////////
            erase_info_in_father(idx);
            this->erase(this->begin() + idx);
            fix_after_index(idx, size + 1);
        }
    }
    void __nofix_delete_hasChild(int idx)
    {
        if (this->at(idx).Child_idx.empty())
        {
            std::cerr << "Incorrect Type | delete_hasChild\n";
            throw this;
        }
        int size = this->at(idx).Child_idx.size();
        int child_begin = this->at(idx).Child_idx.front();
        int child_end = this->at(idx).Child_idx.back();

        if (!this->at(idx).Child_idx.empty())
        {
            /////////////////////////////////////del-child////////////////
            for (int i = child_begin; i <= child_end;)
            {
                if (this->at(i).Child_idx.empty())
                {
                    __nofix_delete_noChild(i);

                    --child_end;
                }
                if (!this->at(i).Child_idx.empty())
                {
                    __nofix_delete_hasChild(i);
                }
            }
            ////////////////////////////////////////del-info-in-fa and
            /// del-self/////////////
            for (int i = 0;
                 i < this->at(this->at(idx).Father_idx).Child_idx.size(); ++i)
            {
                if (this->at(this->at(idx).Father_idx).Child_idx.at(i) == idx)
                {
                    this->at(this->at(idx).Father_idx)
                        .Child_idx.erase(this->at(this->at(idx).Father_idx)
                                             .Child_idx.begin() +
                                         i);
                }
            }
            this->erase(this->begin() + idx);
        }
    }

public:
    /*
      void operator+=(json&& buf) {
       check_repeat(buf.key);
       this->emplace_back(std::move(buf));
     }
     void operator+=(std::initializer_list<json> l) {
       for (auto x : l) {
         check_repeat(x.key);
         this->emplace_back(std::move(x));
       }
     }
    */
    json& operator()(std::string key) { return this->at(get_idxl1(key)); }
    struct comp
    {
        slice result;
        std::string key;
        int layer;
        bool is_set{false};
        json_map* map;
        ///////////at_back//////////////////////////////////////////
        void operator[](std::initializer_list<json> list)
        { // side to op[key],pass nodes to form a childtree
            if (this->is_set)
            {
                this->map->__nofix_delete_hasChild(this->map->get_idxl1(this->key));
            }
            else
            {
                if (list.size() == 0)
                {
                    json buf = {this->key, "{}", data_type::object_void, 1, 0, {}};
                    this->map->emplace_back(buf);
                }
                else
                {
                    bool is_array = true;
                    for (json x : list)
                    {
                        is_array = is_array && x.key.empty();
                    }
                    if (is_array)
                    {
                        std::cerr << "Not Valid Object | at back op[] list\n";
                        throw this;
                    }
                    json buf = {this->key, "", data_type::object_list, 1, 0, {}};
                    this->map->emplace_back(buf);
                    int father_idx = this->map->size() - 1;
                    int tmp_child_idx = 0;
                    for (json x : list)
                    {
                        this->map->emplace_back(x);
                        this->map->back().Father_idx = father_idx;
                        tmp_child_idx = (int)(this->map->size() - 1);
                        this->map->at(father_idx)
                            .Child_idx.emplace_back(tmp_child_idx);
                    }
                }
            }
        }
        void operator()(std::initializer_list<json> list)
        {
            if (this->is_set)
            {
                this->map->__nofix_delete_hasChild(this->map->get_idxl1(this->key));
            }
            else
            {
                if (list.size() == 0)
                {
                    json buf = {this->key, "[]", data_type::array_void, 1, 0, {}};
                    this->map->emplace_back(buf);
                }
                else
                {
                    bool is_array = true;
                    for (json x : list)
                    {
                        is_array = is_array && x.key.empty();
                    }
                    if (is_array)
                    {
                        json buf = {this->key, "", data_type::array_list, 1, 0, {}};
                        this->map->emplace_back(buf);
                        int father_idx = this->map->size() - 1;
                        int tmp_child_idx = 0;
                        for (json x : list)
                        {
                            this->map->emplace_back(x);
                            this->map->back().Father_idx = father_idx;
                            tmp_child_idx = (int)(this->map->size() - 1);
                            this->map->at(father_idx)
                                .Child_idx.emplace_back(tmp_child_idx);
                        }
                    }
                    else
                    {
                        std::cerr << "Not Valid Array | at back op() list\n";
                        throw this;
                    }
                }
            }
        }
        void operator[](input_val v)
        { // side to op[key],to pass value to add a node to layer1

            if (this->is_set)
            {
                int idx = this->map->get_idxl1(this->key);
                json& buf = this->map->at(idx);
                switch (v.index())
                {
                    case 0:
                        buf.type = json_acc_layer_np::data_type::digit_int;
                        buf.value = std::to_string(std::get<long long>(v));
                        break;
                    case 1:
                        if (std::get<std::string>(v) != "[]" &&
                            std::get<std::string>(v) != "{}")
                        {
                            buf.type = json_acc_layer_np::data_type::str;
                        }
                        if (std::get<std::string>(v) == "[]")
                        {
                            buf.type = json_acc_layer_np::data_type::array_void;
                        }
                        if (std::get<std::string>(v) == "{}")
                        {
                            buf.type =
                                json_acc_layer_np::data_type::object_void;
                        }
                        buf.value = std::get<std::string>(v);
                        break;
                    case 2:
                        buf.type = json_acc_layer_np::data_type::digit_double;
                        buf.value = std::to_string(std::get<double>(v));
                        break;
                    case 3:
                        buf.type = json_acc_layer_np::data_type::bool_t;
                        buf.value = std::to_string(std::get<bool>(v));
                        break;
                    case 4:
                        buf.type = json_acc_layer_np::data_type::notype;
                        break;
                    default:
                        std::cerr << "Error Type!\n";
                        throw("error idx!");
                        break;
                }
            }
            else
            {
                json buf;
                buf.key = this->key;
                switch (v.index())
                {
                    case 0:
                        buf.type = json_acc_layer_np::data_type::digit_int;
                        buf.value = std::to_string(std::get<long long>(v));
                        break;
                    case 1:
                        if (std::get<std::string>(v) != "[]" &&
                            std::get<std::string>(v) != "{}")
                        {
                            buf.type = json_acc_layer_np::data_type::str;
                        }
                        if (std::get<std::string>(v) == "[]")
                        {
                            buf.type = json_acc_layer_np::data_type::array_void;
                        }
                        if (std::get<std::string>(v) == "{}")
                        {
                            buf.type =
                                json_acc_layer_np::data_type::object_void;
                        }
                        buf.value = std::get<std::string>(v);
                        break;
                    case 2:
                        buf.type = json_acc_layer_np::data_type::digit_double;
                        buf.value = std::to_string(std::get<double>(v));
                        break;
                    case 3:
                        buf.type = json_acc_layer_np::data_type::bool_t;
                        buf.value = std::to_string(std::get<bool>(v));
                        break;
                    case 4:
                        buf.type = json_acc_layer_np::data_type::notype;
                        break;
                    default:
                        std::cerr << "Error Type!\n";
                        throw("error idx!");
                        break;
                }
                buf.layer = 1;
                map->emplace_back(buf);
            }
        }
        ///////////////////////at_back///////////////////////////
    };
    struct comp_as
    {
        json_map* map;
        int father_idx;
        //////////////////////as_child////////////////////////////
        void operator()(data_type type, std::initializer_list<json> list)
        {
            switch (type)
            {
                case data_type::array_list:
                    for (auto& x : list)
                    {
                        if (!x.key.empty())
                        {
                            std::cerr
                                << "Array Child Shouldn't Have Its Own Key\n";
                            throw this;
                        }
                    }
                    break;
                case data_type::object_list:
                    for (auto x : list)
                    {
                        if (x.key.empty())
                        {
                            std::cerr << "Object Child Shouldn't Be Empty\n";
                            throw this;
                        }
                        this->map->check_repeat(
                            x.key, this->map->at(this->father_idx).layer + 1);
                    }
                    break;
                default:
                    std::cerr << "Error Type of Op() asChild lists\n";
                    throw this;
                    break;
            }
        }
        void operator()(std::string k, input_val val)
        {
            int father_idx = this->father_idx;
            switch (this->map->at(father_idx).type)
            {
                case data_type::array_list:
                    if (!k.empty())
                    {
                        std::cerr << "Array Child Shouldn't Have Its Own Key\n";
                        throw this;
                    }
                    break;
                case data_type::object_list:
                    if (k.empty())
                    {
                        std::cerr << "Key Void!\n";
                        throw this;
                    }
                    this->map->check_repeat(
                        k, this->map->at(father_idx).layer + 1);
                    break;
                default:
                    std::cerr << "Error Type of Op() asChild kv\n";
                    throw this;
                    break;
            }
            data_type vtype = this->map->variant_return_type(val);
            json buf = {k, "", vtype, this->map->at(father_idx).layer + 1, father_idx, {}};
            switch (vtype)
            {
                case data_type::notype: break;
                case data_type::str:
                    buf.value = std::get<std::string>(val);
                    break;

                case data_type::digit_int:
                    buf.value = std::to_string(std::get<i64t>(val));
                    break;

                case data_type::digit_double:
                    buf.value = std::to_string(std::get<double>(val));
                    break;

                case data_type::array_void: buf.value = "[]"; break;

                case data_type::object_void: buf.value = "{}"; break;

                case data_type::bool_t:
                    buf.value = std::to_string(std::get<bool>(val));

                    break;

                case data_type::null: buf.value = "null"; break;

                default:
                    std::cerr << "Error Type\n";
                    throw this;
                    break;
            }
            this->map->emplace_back(buf);
        }
        //////////////////////as_child////////////////////////////
    };
    // helpers to extend functionality of op[],because before C++23 no one is
    // able to make passing params decent using only one op[]
    comp_as operator[](int father_idx_v) // as child
    {
        comp_as buf;
        buf.map = this;
        buf.father_idx = father_idx_v;                                                // passing information of course
        this->check_repeat(this->at(father_idx_v).key, this->at(father_idx_v).layer); // since this is a as child function,we
                                                                                      // shall check the fathers' key
        // stdmove make it an NVRO,remember?
        return std::move(buf);
    }
    comp operator[](std::string k)
    {
        comp buf;
        buf.key = k;    // passing infos
        buf.map = this; // nested struct,so u need to pass a ptr
        buf.result = find(std::move(k));
        if (buf.result.size() == 0)
        { // push at back,so we just make a check on the key u pass
            buf.is_set = false;
            return std::move(buf);
        }
        else
        {
            for (auto x : buf.result)
            {
                if (this->at(x).layer == 1)
                {
                    buf.is_set = true;
                    break;
                }
            }
            return std::move(buf);
        }
        return std::move(buf);
    }
    /*
     struct comp_delete {
        int layer{-1};
        json_map* map;

        void operator^(std::string key) {
          int idx = this->map->get_idxl(key, this->layer);
          switch (this->map->at(idx).type) {
            case data_type::array_list:
              this->map->delete_hasChild(idx);
              return;
              break;
            case data_type::object_list:
              this->map->delete_hasChild(idx);
              return;
              break;
            default:
              this->map->delete_noChild(idx);
              return;
              break;
          }
        }
      };
      comp_delete operator-(int layerv) {
        comp_delete buf;
        buf.layer = layerv;
        buf.map = this;
        return std::move(buf);
      }

      void operator^(
          std::string key_toDel) {  // function to delete,only check layer1
     because
                                    // it doesn't accept a second param
        int idx = this->get_idxl1(key_toDel);
        switch (this->at(idx).type) {
          case data_type::array_list:
            delete_hasChild(idx);
            return;
            break;
          case data_type::object_list:
            delete_hasChild(idx);
            return;
            break;
          default:
            delete_noChild(idx);
            return;
            break;
        }
      }


    */

public:
    constexpr json& operator[](size_t idx)
    {
        return this->at(idx);
    } // constexpr is the correct way to do it
      // and of course if u delete it,u will get a overload error

private:
    int PairList_Expect_Pool(std::string& data, json_map& map,
                             size_t current_root_idx, int beginpos = 0,
                             int current_layer = -1)
    { ///////有问题，会跳读
        // std::cout << "\n";
        json buf;
        int j = 0;
        int k = 0;
        int i = 0;
        int t = 0;
        int layer = 0;

        for (i = beginpos + 1; data[i] != LayerE; i++)
        {
            if (data[i] == ConS)
            {
                buf.Father_idx = current_root_idx;
                buf.layer = current_layer;
                for (t = i; data[t] != ConE && data[t] != LayerS; t--)
                    ;
                for (k = t; data[k] != ConS; k++)
                {
                    if (data[k] != LayerS && data[k] != FieldS &&
                        data[k] != ConE && data[k] != Quote)
                    {
                        buf.key.push_back(data[k]);
                    }
                }
                if (data[i + 1] == 'n' && data[i + 2] == 'u' &&
                    data[i + 3] == 'l' && data[i + 4] == 'l')
                {
                    buf.value = "null";
                    buf.type = data_type::null;

                    map.emplace_back(buf);
                    map[current_root_idx].Child_idx.emplace_back(map.size() -
                                                                 1);

                    buf.clear();
                }
                if (data[i + 1] == 't' && data[i + 2] == 'r' &&
                    data[i + 3] == 'u' && data[i + 4] == 'e')
                { // bool true
                    buf.value = "true";
                    buf.type = data_type::bool_t;

                    map.emplace_back(buf);
                    map[current_root_idx].Child_idx.emplace_back(map.size() -
                                                                 1);

                    buf.clear();
                }
                if (data[i + 1] == 'f' && data[i + 2] == 'a' &&
                    data[i + 3] == 'l' && data[i + 4] == 's' &&
                    data[i + 5] == 'e')
                { // bool false
                    buf.value = "false";
                    buf.type = data_type::bool_t;

                    map.emplace_back(buf);
                    map[current_root_idx].Child_idx.emplace_back(map.size() -
                                                                 1);

                    buf.clear();
                }
                if (data[i + 1] == Quote)
                { // str
                    buf.type = data_type::str;

                    for (j = i + 2; (data[j + 1] != ConE || data[j] != Quote) &&
                                    (data[j + 1] != LayerE || data[j] != Quote);
                         ++j)
                    {
                        buf.value.push_back(data[j]);
                    }
                    //-------buf.value.push_back(Quote);

                    map.emplace_back(buf);
                    map[current_root_idx].Child_idx.emplace_back(map.size() -
                                                                 1);

                    buf.clear();
                    // std::cout << data[j] << "  ssssssss\n";
                    i = j - 1;
                    j = 0;
                }
                if (std::isdigit(data[i + 1]) ||
                    (data[i + 1] == '-' && std::isdigit(data[i + 2])))
                { // digit_all
                    // std::cout << "DIGIT" << "\n";
                    buf.type = data_type::digit_int;
                    for (j = i + 1; data[j] != ConE && data[j] != LayerE; j++)
                    {
                        if (data[j] == '.')
                        {
                            buf.type = data_type::digit_double;
                        }
                        buf.value.push_back(data[j]);
                    }

                    map.emplace_back(buf);
                    map[current_root_idx].Child_idx.emplace_back(map.size() -
                                                                 1);

                    buf.clear();
                    i = j - 1;
                    j = 0;
                }
                //////////////////////////////////////////////////////////////////
                if (data[i + 1] == FieldS && data[i + 2] == FieldE)
                { // void list
                    buf.type = data_type::array_void;
                    buf.value = "[]";
                    map.emplace_back(buf);
                    map[current_root_idx].Child_idx.emplace_back(map.size() -
                                                                 1);

                    buf.clear();
                    i = i + 2;
                }
                if (data[i + 1] == LayerS && data[i + 2] == LayerE)
                { // object_void {}   Object accelerate parsing
                    buf.type = data_type::object_void;

                    buf.value = "{}";
                    map.emplace_back(buf);
                    map[current_root_idx].Child_idx.emplace_back(map.size() -
                                                                 1);

                    buf.clear();
                    i = i + 2;
                }
                ///////////////////////
                if (data[i + 1] == FieldS)
                { // Dimension List array
                    buf.type = data_type::array_list;
                    map.emplace_back(buf);
                    map[current_root_idx].Child_idx.emplace_back(map.size() -
                                                                 1);
                    j = DimensionArray_Expect_Pool(data, map, map.size() - 1,
                                                   i + 1, current_layer + 1);

                    buf.clear();
                    i = j;
                }
                /////////////////////////////////////////////////////////////////

                if (data[i + 1] == LayerS)
                { // object_list {}   Object
                    buf.type = data_type::object_list;
                    map.emplace_back(buf);
                    map[current_root_idx].Child_idx.emplace_back(map.size() -
                                                                 1);
                    j = PairList_Expect_Pool(data, map, map.size() - 1, i + 1,
                                             current_layer + 1);

                    buf.clear();
                    i = j; // if it is j-1,it will result in catching last
                           // nesting
                           // "}",and idx will be wrong
                    j = 0;
                }
            }
        }

        return i; // letter Should Be LayerE,However,When There was a nest,It
                  // will change to next "}" or "]"
    }

    int DimensionArray_Expect_Pool(std::string& data, json_map& map,
                                   size_t current_root_idx, int beginpos = 0,
                                   int current_layer = -1)
    {
        // std::cout << "IN_FUNC\n";
        json buf;
        int j = 0;
        int i = 0;

        for (i = beginpos; data[i] != FieldE; ++i)
        {
            if (data[i] == FieldS || data[i] == ConE)
            {
                buf.Father_idx = current_root_idx;
                buf.layer = current_layer;
                if (data[i + 1] == 'n' && data[i + 2] == 'u' &&
                    data[i + 3] == 'l' && data[i + 4] == 'l')
                {
                    buf.value = "null";
                    buf.type = data_type::null;

                    map.emplace_back(buf);
                    map[current_root_idx].Child_idx.emplace_back(map.size() -
                                                                 1);

                    buf.clear();
                    i += 4;
                }
                if (data[i + 1] == 't' && data[i + 2] == 'r' &&
                    data[i + 3] == 'u' && data[i + 4] == 'e')
                { // bool true
                    buf.value = "true";
                    buf.type = data_type::bool_t;

                    map.emplace_back(buf);
                    map[current_root_idx].Child_idx.emplace_back(map.size() -
                                                                 1);

                    buf.clear();
                    i += 4;
                }
                if (data[i + 1] == 'f' && data[i + 2] == 'a' &&
                    data[i + 3] == 'l' && data[i + 4] == 's' &&
                    data[i + 5] == 'e')
                { // bool false
                    buf.value = "false";
                    buf.type = data_type::bool_t;

                    map.emplace_back(buf);
                    map[current_root_idx].Child_idx.emplace_back(map.size() -
                                                                 1);

                    buf.clear();
                    i += 5;
                }
                if (data[i + 1] == Quote)
                { // str
                    buf.type = data_type::str;
                    for (j = i + 2; (data[j + 1] != ConE || data[j] != Quote) &&
                                    (data[j + 1] != FieldE || data[j] != Quote);
                         ++j)
                    {
                        buf.value.push_back(data[j]);
                    }

                    map.emplace_back(buf);
                    map[current_root_idx].Child_idx.emplace_back(map.size() -
                                                                 1);

                    buf.clear();

                    i = j - 1;
                    j = 0;
                }
                if (std::isdigit(data[i + 1]) or
                    (data[i + 1] == '-' && std::isdigit(data[i + 2])))
                { // digit_all
                    /////////////////////////////////////////////////////////////
                    buf.type = data_type::digit_int;
                    for (j = i + 1; data[j] != ConE && data[j] != FieldE; j++)
                    {
                        if (data[j] == '.')
                        {
                            buf.type = data_type::digit_double;
                        }
                        buf.value.push_back(data[j]);
                    }

                    map.emplace_back(buf);
                    map[current_root_idx].Child_idx.emplace_back(map.size() -
                                                                 1);

                    buf.clear();
                    i = j - 1;
                    j = 0;
                }

                /////////////////////////////////////////////////////////////////////////////////
                if (data[i + 1] == FieldS && data[i + 2] == FieldE)
                { // void list
                    buf.type = data_type::array_void;
                    buf.value = "[]";
                    map.emplace_back(buf);
                    map[current_root_idx].Child_idx.emplace_back(map.size() -
                                                                 1);
                    buf.clear();
                    i = i + 2;
                }

                if (data[i + 1] == FieldS)
                { // Dimension List
                    buf.type = data_type::array_list;

                    map.emplace_back(buf);
                    map[current_root_idx].Child_idx.emplace_back(map.size() -
                                                                 1);
                    j = DimensionArray_Expect_Pool(data, map, map.size() - 1,
                                                   i + 1, current_layer + 1);

                    buf.clear();

                    i = j;
                }

                /////////////////////////////////////////////////////////////////////////////

                if (data[i + 1] == LayerS && data[i + 2] == LayerE)
                { // object_void {}   Object
                    buf.type = data_type::object_void;

                    buf.value = "{}";
                    map.emplace_back(buf);
                    map[current_root_idx].Child_idx.emplace_back(map.size() -
                                                                 1);

                    buf.clear();
                    i = i + 2;
                }

                if (data[i + 1] == LayerS)
                { // object_list {} Object///////////////////
                    buf.type = data_type::object_list;

                    map.emplace_back(buf);
                    map[current_root_idx].Child_idx.emplace_back(map.size() -
                                                                 1);
                    j = PairList_Expect_Pool(data, map, map.size() - 1, i + 1,
                                             current_layer + 1);

                    buf.clear();

                    i = j - 1; // LayerE
                    j = 0;
                }
            }
        }

        return i;
    }

    int JSON_Parse_Pool(json_map& map, std::string&& data)
    {
        json buf;
        int j = 0;
        int k = 0;
        int t = 0;

        for (int i = 0; i < (int)(data.length() - 1); ++i)
        {
            if (data[i] == ConS)
            {
                /// title
                for (t = i; data[t] != ConE && data[t] != LayerS; t--)
                {
                }
                for (k = t + 1; data[k] != ConS; k++)
                {
                    if (data[k] != Quote)
                    {
                        buf.key.push_back(data[k]);
                    }
                }
                buf.Father_idx = 0;
                buf.layer = 1;
                if (data[i + 1] == 'n' && data[i + 2] == 'u' &&
                    data[i + 3] == 'l' && data[i + 4] == 'l')
                {
                    buf.value = "null";
                    buf.type = data_type::null;

                    map.emplace_back(buf);
                    buf.clear();
                }
                if (data[i + 1] == 't' && data[i + 2] == 'r' &&
                    data[i + 3] == 'u' && data[i + 4] == 'e')
                { // bool true
                    buf.value = "true";
                    buf.type = data_type::bool_t;
                    map.emplace_back(buf);
                    buf.clear();
                }
                if (data[i + 1] == 'f' && data[i + 2] == 'a' &&
                    data[i + 3] == 'l' && data[i + 4] == 's' &&
                    data[i + 5] == 'e')
                { // bool false
                    buf.value = "false";
                    buf.type = data_type::bool_t;
                    map.emplace_back(buf);
                    buf.clear();
                }
                if (data[i + 1] == Quote)
                { // str
                    buf.type = data_type::str;
                    for (j = i + 2; (data[j + 1] != ConE || data[j] != Quote) &&
                                    (data[j + 1] != LayerE || data[j] != Quote);
                         j++)
                    {
                        buf.value.push_back(data[j]);
                    }
                    map.emplace_back(buf);
                    buf.clear();
                    i = j - 1;
                    j = 0;
                }
                if (std::isdigit(data[i + 1]) ||
                    (data[i + 1] == '-' && std::isdigit(data[i + 2])))
                { // digit_all
                    buf.type = data_type::digit_int;
                    for (j = i + 1; data[j] != ConE && data[j] != LayerE; j++)
                    {
                        if (data[j] == '.')
                        {
                            buf.type = data_type::digit_double;
                        }
                        buf.value.push_back(data[j]);
                    }
                    map.emplace_back(buf);
                    buf.clear();
                    i = j - 1;
                    j = 0;
                }
                /////////////////////////////////////////////////////////////////////////////////
                if (data[i + 1] == FieldS && data[i + 2] == FieldE)
                { // void list
                    buf.type = data_type::array_void;
                    buf.value = "[]";

                    map.emplace_back(buf);
                    buf.clear();
                    i = i + 2;
                }
                if (data[i + 1] == LayerS && data[i + 2] == LayerE)
                { // void list
                    buf.type = data_type::object_void;
                    buf.value = "{}";

                    map.emplace_back(buf);
                    buf.clear();
                    i = i + 2;
                }

                if (data[i + 1] == FieldS)
                { // Dimension List
                    buf.type = data_type::array_list;
#define Main_Dimension
                    map.emplace_back(buf);
                    j = DimensionArray_Expect_Pool(data, map, map.size() - 1,
                                                   i + 1, 1);
                    // std::cout << "Dimen:" << data[i + 1] << "\n";

                    buf.clear();
                    i = j;
                }

                /////////////////////////////////////////////////////////////////////////////
                if (data[i + 1] == LayerS)
                { // object_list {}   Object
                    buf.type = data_type::object_list;

#define Main_Pair
                    map.emplace_back(buf);
                    j = PairList_Expect_Pool(data, map, map.size() - 1, i + 1,
                                             1);

                    buf.clear();
                    i = j - 1;
                    // std::cout << "                                       " <<
                    // data[i]
                    // << "\n";
                    j = 0;
                }
            }
        }
        return 1;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int JSON_Serialize_Child(json_map& map, std::string& result,
                             int current_root = 0)
    {
        if (map.at(current_root).type != data_type::notype &&
            map.at(current_root).type != data_type::array_list &&
            map.at(current_root).type != data_type::object_list)
        {
            if (map.at(current_root).type != data_type::str)
            {
                result.push_back('"');
                result.append(map.at(current_root).key);
                result.push_back('"');

                result.push_back(':');

                result.append(map.at(current_root).value);
                result.push_back(',');
            }
            if (map.at(current_root).type == data_type::str)
            {
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
        if (map.at(current_root).type == data_type::array_list)
        {
            int tmp_idx_dm = 0;
            if (!map.at(current_root).key.empty())
            {
                result.append("\"");
                result.append(map.at(current_root).key);
                result.push_back('"');
            }

            if (map.at(map.at(current_root).Father_idx).type ==
                data_type::array_list)
            { // if its father type is dimension,it might
              // broke
              // json syntax
                result.append("[");
            }
            if (map.at(map.at(current_root).Father_idx).type !=
                data_type::array_list)
            {
                result.append(":[");
            }
            for (int i = 0; i < map.at(current_root).Child_idx.size(); i++)
            {
                tmp_idx_dm = map.at(current_root).Child_idx.at(i);
                if (map.at(tmp_idx_dm).type != data_type::array_list &&
                    map.at(tmp_idx_dm).type != data_type::object_list &&
                    map.at(tmp_idx_dm).type != data_type::notype)
                {
                    if (map.at(tmp_idx_dm).type == data_type::str)
                    {
                        result.push_back('"');
                        result.append(map.at(tmp_idx_dm).value);
                        result.push_back('"');
                    }
                    if (map.at(tmp_idx_dm).type != data_type::str)
                    {
                        result.append(map.at(tmp_idx_dm).value);
                    }
                    if (i != map.at(current_root).Child_idx.size() - 1)
                    {
                        result.push_back(',');
                    }
                }
                if (map.at(tmp_idx_dm).type == data_type::array_list)
                {
                    JSON_Serialize_Child(map, result, tmp_idx_dm);
                    if (i != map.at(current_root).Child_idx.size() - 1)
                    {
                        result.push_back(',');
                    }
                }
                if (map.at(tmp_idx_dm).type == data_type::object_list)
                {
                    JSON_Serialize_Child(map, result, tmp_idx_dm);
                }
            }
            if (result.back() == ',')
            {
                result.pop_back();
            }
            result.append("]");
            if (map.at(current_root).Father_idx == 0)
            {
                result.push_back(',');
            }
        }
        if (map.at(current_root).type == data_type::object_list)
        {
            int tmp_idx_pl = 0;

            if (!map.at(current_root).key.empty())
            {
                result.append("\"");
                // result.push_back('"');
                result.append(map.at(current_root).key);
                result.push_back('"');
            }

            if (!map.at(current_root).key.empty())
            {
                result.append(":");
            }
            result.append("{");
            int i = 0;
            for (; i < (int)map.at(current_root).Child_idx.size(); i++)
            {
                tmp_idx_pl = map.at(current_root).Child_idx.at(i);
                if (map.at(tmp_idx_pl).type != data_type::array_list &&
                    map.at(tmp_idx_pl).type != data_type::object_list &&
                    map.at(tmp_idx_pl).type != data_type::notype)
                {
                    // result.pop_back();
                    if (map.at(tmp_idx_pl).type != data_type::str)
                    {
                        result.push_back('"');
                        result.append(map.at(tmp_idx_pl).key);
                        result.push_back('"');

                        result.push_back(':');

                        result.append(map.at(tmp_idx_pl).value);
                        result.push_back(',');
                    }
                    if (map.at(tmp_idx_pl).type == data_type::str)
                    {
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
                if (map.at(tmp_idx_pl).type == data_type::object_list)
                {
                    JSON_Serialize_Child(map, result, tmp_idx_pl);
                    // result.append("PairList");
                }
                if (map.at(tmp_idx_pl).type == data_type::array_list)
                {
                    JSON_Serialize_Child(map, result, tmp_idx_pl);
                    if (tmp_idx_pl != map.at(current_root).Child_idx.size() - 1)
                    {
                        result.push_back(',');
                    }
                }
            }
            if (result.back() == ',')
            {
                result.pop_back();
            }
            result.push_back('}');
            result.push_back(',');
        }

        // result.pop_back();

        return 1;
    }
    int JSON_Serialize_Pool(json_map& map, std::string& result,
                            int current_root = 0)
    {
        result.push_back('{');
        int target_idx = 0;
        for (int i = 0; i < map.size(); i++)
        {
            if (map.at(i).Father_idx == 0)
            {
                target_idx = i;
                // << i << "\n";
                JSON_Serialize_Child(map, result, i);
            }
        }
        if (result.back() == ',')
        {
            result.pop_back();
        }
        result.push_back('}');
        return 1;
    }

public:
    void parse(std::string& data) { JSON_Parse_Pool(*this, std::move(data)); }
    std::string serialize()
    {
        std::string buf;
        JSON_Serialize_Pool(*this, buf, 0);
        return buf;
    }

private:
public:
    void iter_parse(std::string& data);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void json_map::iter_parse(std::string& data)
{
    struct iter_info
    {
        struct key
        {
            int begin{0};
            int end{0};
        };
        struct val
        {
            int begin{0};
            int end{0};
        };
        int layer_through{0};
        int field_through{0};
    };
    json buf;
    json_map& map = *this;
    int layer = 0;
    int object = 0;
    std::vector<iter_info> infos;
    for (int i = 0; i < (int)(data.length() - 1); ++i)
    {
        switch (data[i])
        {
            case LayerS: break;
            case LayerE: break;
            case ConS: break;
            case ConE: break;
            case FieldS: break;
            case FieldE: break;
            case Quote: break;
            default: break;
        }
    }
}

} // namespace json_acc_layer_np
using json_map = json_acc_layer_np::json_map;
using json = json_acc_layer_np::json;
using data_type = json_acc_layer_np::data_type;
/*
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
*/