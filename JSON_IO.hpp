#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <string_view>
#include "JSON_Base.h"
#pragma once
namespace LP_BASIC_IO {
	void IO_Read(std::string *Data, const char *PATH) { //可以读取换行的函数
#define Tail " "

		std::ifstream fp;
		std::string TEMP;//因为>>符号会覆盖，先写入缓存再append追加
		fp.open(PATH, std::ios::in);
		std::cout << "We are Reading:" << PATH << std::endl;
//	if (!(&fp)) {
//		perror("Reading Fail");
//		std::cout << "Fail" << std::endl;
//	}
		std::cout << "Is EOF:" << fp.eof() << std::endl;
		fp >> *Data;
		do { //第一次\n是上面的，因为此时已经截断了，后面是从第一个\n开始，跳过这个换行符号，读到\n就换行，因此可以不理换行符全部读取

			fp.seekg(sizeof(char), std::ios::cur);
			fp >> TEMP;
			//std::cout<<"Sync:"<<fp.sync()<<std::endl;

			(*Data).append(Tail);
			(*Data).append(TEMP);

		} while (!(fp.eof()));
		std::cout << "Finish Reading" << std::endl;
		fp.close();
	}
	void IO_Write(std::string *Data, const char *PATH) {
		std::cout << "Start SingleThread Write" << std::endl;
		std::ofstream of;
		of.open(PATH, std::ios::out | std::ios::trunc);
		of << (*Data);
	}
}


namespace HP_IO {
#define SIGNAL_FINISH 1
#define SIGNAL_FAIL_NULL_PTR 4
#define SIGNAL_IN_PROCESS 3
#define SIGNAL_IDLE_READY 0
#define WR_R 1
#define WR_W 2
#define WR_IDLE 0
	struct JSON_IO {
	  protected:
		int FILE_SZ{0};
		int FILE_N{0};
		bool W_R{WR_IDLE};
		int SIGNAL{SIGNAL_IDLE_READY};
	  protected:

	  public:
		int READ() {
			printf("IOR_NO_SPECIFIC_BEHAVE\n");
			return 0;
		};
		int WRITE() {
			printf("IOW_NO_SPECIFIC_BEHAVE\n");
			return 0;
		};
	};
	struct JSON_IO_FILE : public JSON_IO {
	  public:
		std::ifstream fp;
	  private:
		std::istreambuf_iterator<char> buf_it;
	  public:
		void READ(const char* path, std::string& data) {
			fp.open(path, std::ios::in);
			if (!fp.is_open()) {
				return;
			}
			data.assign(std::istreambuf_iterator<char>(fp), std::istreambuf_iterator<char>());//构造了一个流迭代器
			// 分配器由流迭代器管理
			data.erase(std::remove_if(data.begin(), data.end(), [](char &c) {//UCRT Clang
				return std::isspace<char>(c, std::locale::classic());
			}), data.end());//后处理的
			//data.erase(std::remove(data.begin(), data.end(), ' '), data.end());
			//data.erase(std::remove(data.begin(), data.end(), '	'), data.end());
			//	data.erase(std::remove_if(data.begin(), data.end(), [](char &c) {//UCRT Clang
			//		return '\n';
			//	}), data.end());
		}
		void WRITE(const char* path, std::string& data) {
			fp.open(path, std::ios::out);
			if (!fp.is_open()) {
				return;
			}

		}
		std::istreambuf_iterator<char>& VDF_STREAMBUF_IT() {
			buf_it = fp;
			return buf_it;
		}
	};
	struct JSON_IO_MFILE : public JSON_IO {

	};

}

