#include <chrono>
#include <cstdint>
#include <iostream>

// s#include "bson.hpp"
#include "json.hpp"
#include "json_io.hpp"

#pragma once
#ifndef __MAIN_
#define __MAIN_
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
#define STR(x)     \
    case x:        \
        return #x; \
        break;
std::string penum(int32_t e)
{
    switch (e)
    {
        STR(json_acc_layer_np::data_type::temp_digit)
        STR(json_acc_layer_np::data_type::str)
        STR(json_acc_layer_np::data_type::digit_int)
        STR(json_acc_layer_np::data_type::null)
        STR(json_acc_layer_np::data_type::digit_double)
        STR(json_acc_layer_np::data_type::object_list)
        STR(json_acc_layer_np::data_type::object_void)
        STR(json_acc_layer_np::data_type::bool_t)
        STR(json_acc_layer_np::data_type::array_void)
        STR(json_acc_layer_np::data_type::array_list)
        // STR(json_acc_layer_np::data_type::error)
        STR(json_acc_layer_np::data_type::notype)
        STR(json_acc_layer_np::data_type::invalid_opttype)
        default:
            // TODO
            break;
    }
    return "------------------";
}
int main()
{
    // const char* path = "5.json";
    const char* path = "citm_catalog.json";
    // const char* path = "3.json";
    // const char* path = "4.json";
    // const char* path = "canada.json";
    // const char* path = "med.json";
    // const char* path = "generated.json";
    std::string data;
    HP_IO::json_file FILE;
    json_map map;
    json root;
    map.emplace_back(root);
//#define IO_TIME
#ifdef IO_TIME
    high_resolution_clock::time_point ioStime = high_resolution_clock::now();
    FILE.read_tocompact(path, data);
    high_resolution_clock::time_point ioEtime = high_resolution_clock::now();
#endif
// LP_BASIC_IO::IO_nospec_Read(data, path);
// std::cout << data << "\n\n";
#ifdef IO_TIME
    high_resolution_clock::time_point Stime = high_resolution_clock::now();
    map.parse(data);
    high_resolution_clock::time_point Etime = high_resolution_clock::now();
    std::cout << "JSONParse Finish\n";
    milliseconds Parse_Interval =
        std::chrono::duration_cast<milliseconds>(Etime - Stime);
    std::cout << "ParseTime:" << Parse_Interval.count() << "ms" << std::endl;
#endif
#ifdef IO_TIME
    milliseconds IO_Interval =
        std::chrono::duration_cast<milliseconds>(ioEtime - ioStime);
    std::cout << "IOTime:" << IO_Interval.count() << "ms" << std::endl;
#endif
#ifndef IO_TIME
    high_resolution_clock::time_point Stime = high_resolution_clock::now();
    map.parse_fromfile(path);
    high_resolution_clock::time_point Etime = high_resolution_clock::now();
    std::cout << "JSONParse Finish\n";
    milliseconds Parse_Interval =
        std::chrono::duration_cast<milliseconds>(Etime - Stime);
    std::cout << "ParseTime:" << Parse_Interval.count() << "ms" << std::endl;
#endif
    /////////////////////////////usage////////////////////////////////
    //--map["systembit"][64]; // add
    //--map["key_new"]["wtf"];
    map["Sx"]["32"];

    // map["Long_list"][{json{}("aaa", "bbb"), json{}("aaacc", "bbdddb")}];
    // map["Long_list2"]({json{}("", 2), json{}("", 3), json{}("", 6)});

    // map.find("138586341")[0].list_of()[0].print_info();
    /// map.find("138586341")[0].list_of()[0].delete_which();
    // map.find("name")[0].delete_which();
    // map.find("")[0].set_val()
    // map.find("half-systembit")[0].print_info();
    // map.find("Sx")[0].delete_which();
    // map.find("half-systembit")[0].delete_which();
// #define PRINT
#ifdef PRINT
    int i = 0;
    for (auto x : map)
    {
        std::cout << "key:" << x.key << " | ";
        std::cout << "val:" << x.value << " | ";
        std::cout << "idx:" << i << " | ";
        std::cout << "type:" << penum(x.type) << " | ";
        std::cout << "\n      ";
        for (auto x : x.Child_idx)
        {
            std::cout << x << " | ";
        }
        // std::cout << "\n";
        std::cout << "\n";
        ++i;
        // std::cout<<"csize:" <<x.Child_idx.size()<<" | ";
    }

#endif
    // std::cout << (map("subTopicIds").key) << "\n";       // get idx through
    // op() std::cout << (int)(map("subTopicIds").type) << "\n";
    /////////////////////////////usage////////////////////////////////

    std::cout << "DataSize:" << data.size() << "\n";
    std::cout << "MapSize:" << map.size() << "\n";

    LP_BASIC_IO::IO_Write(map.serialize(), "output.json");
    std::cout << "Finish Serialize"
              << "\n";
    return 0;
}
#endif
