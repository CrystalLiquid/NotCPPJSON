#include <execution>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <numeric>
#include <string>

#pragma once

namespace LP_BASIC_IO {
using i64 = long long;
void IO_nospec_Read(std::string &data, const char *PATH)
{
    std::ifstream ifs(PATH);
    std::string s;
    int quote = 0;
    while (std::getline(ifs, s, '\n'))
    {
        for (int i = 0; i < s.size();)
        {
            if (s[i] == '"') ++quote;
            if (quote == 2) quote = 0;
            if (s[i] == ' ' && quote != 1)
            {
                s.erase(s.begin() + i);
                continue;
            }
            if (s[i] != ' ') ++i;
        }
        data.append(s);
    }
}
void IO_Read(std::string *Data, const char *PATH)
{ // 可以读取换行的函数
#define Tail " "
    std::ifstream fp;
    std::string TEMP; // 因为>>符号会覆盖，先写入缓存再append追加
    fp.open(PATH, std::ios::in);
    std::cout << "We are Reading:" << PATH << std::endl;
    //	if (!(&fp)) {
    //		perror("Reading Fail");
    //		std::cout << "Fail" << std::endl;
    //	}
    std::cout << "Is EOF:" << fp.eof() << std::endl;
    fp >> *Data;
    do
    { // 第一次\n是上面的，因为此时已经截断了，后面是从第一个\n开始，跳过这个换行符号，读到\n就换行，因此可以不理换行符全部读取

        fp.seekg(sizeof(char), std::ios::cur);
        fp >> TEMP;
        // std::cout<<"Sync:"<<fp.sync()<<std::endl;

        (*Data).append(Tail);
        (*Data).append(TEMP);

    } while (!(fp.eof()));
    std::cout << "Finish Reading" << std::endl;
    fp.close();
}
void IO_Write(std::string &&Data, const char *PATH)
{
    std::cout << "Start SingleThread Write" << std::endl;
    std::ofstream of;
    of.open(PATH, std::ios::out | std::ios::trunc);
    of << Data;
    of.close();
}
template <typename T>
void IO_BinWrite(T &&val, const char *PATH)
{
    std::cout << "Start Bin Write" << std::endl;
    std::ofstream of;
    of.open(PATH, std::ios::out | std::ios::trunc | std::ios::binary);
    of.write(reinterpret_cast<char *>(&val), sizeof(val));
    of.close();
}
void IO_Write_Append(std::string &&Data, const char *PATH)
{
    std::cout << "Start SingleThread Write" << std::endl;
    std::ofstream of;
    of.open(PATH, std::ios::out | std::ios::app);
    of << (Data);
    of.close();
}
} // namespace LP_BASIC_IO

namespace HP_IO {
#define SIGNAL_FINISH 1
#define SIGNAL_FAIL_NULL_PTR 4
#define SIGNAL_IN_PROCESS 3
#define SIGNAL_IDLE_READY 0
#define WR_R 1
#define WR_W 2
#define WR_IDLE 0
struct json_io
{
protected:
    int file_sz{0};
    int file_n{0};
    bool w_r{WR_IDLE};
    int signal{SIGNAL_IDLE_READY};

protected:
public:
    int read()
    {
        printf("IOR_NO_SPECIFIC_BEHAVE\n");
        return 0;
    };
    int write()
    {
        printf("IOW_NO_SPECIFIC_BEHAVE\n");
        return 0;
    };
};
struct json_file : public json_io
{
public:
    std::ifstream fp;

private:
    std::istreambuf_iterator<char> buf_it;

public:
    void read_nospace(const char *path, std::string &data)
    {
        fp.open(path, std::ios::in);
        if (!fp.is_open())
        {
            return;
        }
        data.assign(std::istreambuf_iterator<char>(fp), std::istreambuf_iterator<char>()); // 构造了一个流迭代器
                                                                                           // 分配器由流迭代器管理
                                                                                           // using streambuf to accelerate the process of
                                                                                           // assign and cloning data

        data.erase(std::remove_if(data.begin(), data.end(), [](char &c) { // UCRT Clang
                       return std::isspace<char>(c, std::locale::classic());
                   }),
                   data.end()); // 后处理的
    }
    void read_tocompact(const char *path, std::string &data)
    {
        fp.open(path, std::ios::in);
        if (!fp.is_open())
        {
            return;
        }
        data.assign(std::istreambuf_iterator<char>(fp), std::istreambuf_iterator<char>()); // 构造了一个流迭代器
                                                                                           // 分配器由流迭代器管理
                                                                                           // using streambuf to accelerate the process of
                                                                                           // assign and cloning data

        data.erase(std::remove_if(data.begin(), data.end(), [](char &c) { // UCRT Clang
                       return std::iscntrl<char>(c, std::locale::classic());
                   }),
                   data.end()); // 后处理的

        auto del_between = [](std::string &data, int beginpos, char c) {
            int i = 0;
            for (i = beginpos; data[i] != c; ++i)
                ;
            data.erase(beginpos + 1, i - beginpos - 1);
            return beginpos + 1;
        };
        for (int i = 0; i < data.size(); ++i)
        {
            if (data[i] == '{')
            {
                del_between(data, i, '"');
            }
            if (data[i] == ',')
            {
                int k = i + 1;
                for (; data[k] == ' '; ++k)
                    ;
                data.erase(i + 1, k - i - 1);
            }
            if (data[i] == '[' || data[i] == '}' || data[i] == ']' || data[i] == ':')
            {
                int k = i + 1;
                for (; data[k] == ' '; ++k)
                    ;
                data.erase(i + 1, k - i - 1);
            }

            if (data[i] == '}' || data[i] == ']')
            {
                int r = i - 1;
                for (; data[r] == ' '; --r)
                    ;
                data.erase(r + 1, i - r - 1);
            }
        }

        for (int i = 0; i < data.size(); ++i)
        {
            if (data[i] == ',')
            {
                int k = i + 1;
                for (; data[k] == ' '; ++k)
                    ;
                data.erase(i + 1, k - i - 1);
            }
        }

        // erase all uncompact JSON tokens to raise compatibility

        // below is the compatible version of erasing with different compiler,DO NOT DELETE THEM
        // data.erase(std::remove(data.begin(), data.end(), ' '), data.end());
        // data.erase(std::remove(data.begin(), data.end(), '	'), data.end());
        //	data.erase(std::remove_if(data.begin(), data.end(), [](char &c) {//UCRT Clang
        //		return '\n';
        //	}), data.end());
    }
    void to_compact(std::string& data){
        auto del_between = [](std::string &data, int beginpos, char c) {
            int i = 0;
            for (i = beginpos; data[i] != c; ++i)
                ;
            data.erase(beginpos + 1, i - beginpos - 1);
            return beginpos + 1;
        };
        for (int i = 0; i < data.size(); ++i)
        {
            if (data[i] == '{')
            {
                del_between(data, i, '"');
            }
            if (data[i] == ',')
            {
                int k = i + 1;
                for (; data[k] == ' '; ++k)
                    ;
                data.erase(i + 1, k - i - 1);
            }
            if (data[i] == '[' || data[i] == '}' || data[i] == ']' || data[i] == ':')
            {
                int k = i + 1;
                for (; data[k] == ' '; ++k)
                    ;
                data.erase(i + 1, k - i - 1);
            }

            if (data[i] == '}' || data[i] == ']')
            {
                int r = i - 1;
                for (; data[r] == ' '; --r)
                    ;
                data.erase(r + 1, i - r - 1);
            }
        }

        for (int i = 0; i < data.size(); ++i)
        {
            if (data[i] == ',')
            {
                int k = i + 1;
                for (; data[k] == ' '; ++k)
                    ;
                data.erase(i + 1, k - i - 1);
            }
        }
    }
    void write(const char *path, std::string &data)
    {
        fp.open(path, std::ios::out);
        if (!fp.is_open())
        {
            return;
        }
    }

};
struct json_mtfile : public json_io
{
};

} // namespace HP_IO
