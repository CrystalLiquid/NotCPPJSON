#include "JSON_Base.h"
#include <thread>
#pragma once
#ifndef __ACC_MT_CXX_
#define __ACC_MT_CXX_
inline int Expect_Sign(std::string data, char sign, int startpos) {
	int i = 0;
	for (i = startpos; i < (int)data.length() && data[i] != sign; i++);
	return i;
}


//改为预先读取 再memcpy（现在是memmove）
int JSON_PreRead_Memcpy_Pool( std::string& data) {
	JSON_ACC buf;
	int j = 0;
	for (int i = 0; i < (int)(data.length() - 1); ++i) {
		//std::cout << data[i];
		if (data[i] == ConS) {
			///title

			//std::cout << t << "," << i << "\n";
			if (data[i + 1] == 'n' && data[i + 2] == 'u' && data[i + 3] == 'l' && data[i + 4] == 'l') {

			}
			if (data[i + 1] == 't' && data[i + 2] == 'r' && data[i + 3] == 'u' && data[i + 4] == 'e') {//bool true

			}
			if (data[i + 1] == 'f' && data[i + 2] == 'a' && data[i + 3] == 'l' && data[i + 4] == 's' && data[i + 5] == 'e') { //bool false

			}
			if (data[i + 1] == Quote) {//str

			}
			if (std::isdigit(data[i + 1]) || (data[i + 1] == '-' && std::isdigit(data[i + 2]))) {//digit_all

			}
			/////////////////////////////////////////////////////////////////////////////////
			if (data[i + 1] == FieldS && data[i + 2] == FieldE) {//void list

			}

			if (data[i + 1] == FieldS) { //Dimension List
				j = Expect_Sign(data, FieldE, i + 1);
				int field = 1;
				int k = i + 1;
				for (; field != 0; k++) {
					if (data[k] == FieldS) {
						++field;
					}
					if (data[k] == FieldE) {
						--field;
					}

				}
				i = k;
			}

			/////////////////////////////////////////////////////////////////////////////
			if (data[i + 1] == LayerS) { //pair_list {}   Object
				j = Expect_Sign(data, LayerE, i + 2);

			}

		}
	}
	return 1;
}



#endif
