// Quick Sort Using Three Modes of Paralleleism.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <immintrin.h>
#include <vector>

using std::vector;

void selectionSort(vector<int> &newArr) {
	int tempStorage, iterator;

	for (int i = 0; i < 15; i++) {
		iterator = i;
		for (int j = i + 1; j < 16; j++) {
			if (newArr[j] < newArr[iterator]) {
				iterator = j;
			}

		}
		
		//swap values
		tempStorage = newArr[iterator];
		newArr[iterator] = newArr[i];
		newArr[i] = tempStorage;

	}
}

//65536
vector<vector<int>> initializer() {
	const int maxIntValue = 4294967296;
	default_random_engine generator;
	uniform_int_distribution<int> distrobution(0, maxIntValue);

	vector<int> first;
	vector<vector<int>> allOfThem;

	for (int i = 0; i < 4; i++) {
		allOfThem.push_back(first);
	}

	for (int i = 0; i < 4; i++) {
		for (int t = 0; t < 16; t++) {
			allOfThem[i].push_back(distrobution(generator));
		}
	}

	for (vector<int> val : allOfThem) {
		selectionSort(val);
	}

	return allOfThem;
}



__m512i vectorTOmm512i(vector<int> x) {
	return _mm512_set_epi32(x[15], x[14], x[13], x[12], x[11], x[10], x[9], x[8], x[7], x[6], x[5], x[4], x[3], x[2], x[1], x[0]);
}


void bitonicSort(const __m512 Aa, const __m512 Ab, const __m512 Ba, const __m512 Bb, const __m512 Ca, const __m512 Cb, const __m512 Da, const __m512 Db,
	__m512& Aouta, __m512& Aoutb, __m512& Bouta, __m512& Boutb, __m512& Couta, __m512& Coutb, __m512& Douta, __m512& Doutb) {

	//revers B
	Aoutb = _mm512_permutexvar_ps(_mm512_set_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15), Ab);
	Boutb = _mm512_permutexvar_ps(_mm512_set_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15), Bb);
	Coutb = _mm512_permutexvar_ps(_mm512_set_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15), Cb);
	Doutb = _mm512_permutexvar_ps(_mm512_set_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15), Db);
	
	// L1 
	__m512 L1 = _mm512_min_ps(Aa, Aoutb);
	__m512 L2 = _mm512_min_ps(Ba, Boutb);
	__m512 L3 = _mm512_min_ps(Ca, Coutb);
	__m512 L4 = _mm512_min_ps(Da, Doutb);

	__m512 H1 = _mm512_max_ps(Aa, Aoutb);
	__m512 H2 = _mm512_max_ps(Ba, Boutb);
	__m512 H3 = _mm512_max_ps(Ca, Coutb);
	__m512 H4 = _mm512_max_ps(Da, Doutb);

	Aouta = _mm512_permutex2var_ps(L1, _mm512_set_epi32(23, 22, 21, 20, 19, 18, 17, 16, 7,6,5,4,3,2,1, 0), H1);
	Bouta = _mm512_permutex2var_ps(L2, _mm512_set_epi32(23, 22, 21, 20, 19, 18, 17, 16, 7, 6, 5, 4, 3, 2, 1, 0), H2);
	Couta = _mm512_permutex2var_ps(L3, _mm512_set_epi32(23, 22, 21, 20, 19, 18, 17, 16, 7, 6, 5, 4, 3, 2, 1, 0), H3);
	Douta = _mm512_permutex2var_ps(L4, _mm512_set_epi32(23, 22, 21, 20, 19, 18, 17, 16, 7, 6, 5, 4, 3, 2, 1, 0), H4);

	Aoutb = _mm512_permutex2var_ps(L1, _mm512_set_epi32(31, 30, 29, 28, 27, 26, 25, 24, 15, 14, 13, 12, 11, 10, 9, 8), H1);
	Boutb = _mm512_permutex2var_ps(L2, _mm512_set_epi32(31, 30, 29, 28, 27, 26, 25, 24, 15, 14, 13, 12, 11, 10, 9, 8), H2);
	Coutb = _mm512_permutex2var_ps(L3, _mm512_set_epi32(31, 30, 29, 28, 27, 26, 25, 24, 15, 14, 13, 12, 11, 10, 9, 8), H3);
	Doutb = _mm512_permutex2var_ps(L4, _mm512_set_epi32(31, 30, 29, 28, 27, 26, 25, 24, 15, 14, 13, 12, 11, 10, 9, 8), H4);



	//L2 to L3
	L1 = _mm512_min_ps(Aouta, Aoutb);
	L2 = _mm512_min_ps(Bouta, Boutb);
	L3 = _mm512_min_ps(Couta, Coutb);
	L4 = _mm512_min_ps(Douta, Doutb);

	H1 = _mm512_max_ps(Aouta, Aoutb);
	H2 = _mm512_max_ps(Bouta, Boutb);
	H3 = _mm512_max_ps(Couta, Coutb);
	H4 = _mm512_max_ps(Douta, Doutb);

	Aouta = _mm512_permutex2var_ps(L1, _mm512_set_epi32(27, 26, 25, 24, 11, 10, 9, 8, 19, 18, 17, 16, 3, 2, 1, 0), H1);
	Bouta = _mm512_permutex2var_ps(L2, _mm512_set_epi32(27, 26, 25, 24, 11, 10, 9, 8, 19, 18, 17, 16, 3, 2, 1, 0), H2);
	Couta = _mm512_permutex2var_ps(L3, _mm512_set_epi32(27, 26, 25, 24, 11, 10, 9, 8, 19, 18, 17, 16, 3, 2, 1, 0), H3);
	Douta = _mm512_permutex2var_ps(L4, _mm512_set_epi32(27, 26, 25, 24, 11, 10, 9, 8, 19, 18, 17, 16, 3, 2, 1, 0), H4);

	Aoutb = _mm512_permutex2var_ps(L1, _mm512_set_epi32(31, 30, 29, 28, 15, 14, 13, 12, 23, 22, 21, 20, 7, 6, 5, 4), H1);
	Boutb = _mm512_permutex2var_ps(L2, _mm512_set_epi32(31, 30, 29, 28, 15, 14, 13, 12, 23, 22, 21, 20, 7, 6, 5, 4), H2);
	Coutb = _mm512_permutex2var_ps(L3, _mm512_set_epi32(31, 30, 29, 28, 15, 14, 13, 12, 23, 22, 21, 20, 7, 6, 5, 4), H3);
	Doutb = _mm512_permutex2var_ps(L4, _mm512_set_epi32(31, 30, 29, 28, 15, 14, 13, 12, 23, 22, 21, 20, 7, 6, 5, 4), H4);


	//L3 to L4
	L1 = _mm512_min_ps(Aouta, Aoutb);
	L2 = _mm512_min_ps(Bouta, Boutb);
	L3 = _mm512_min_ps(Couta, Coutb);
	L4 = _mm512_min_ps(Douta, Doutb);

	H1 = _mm512_max_ps(Aouta, Aoutb);
	H2 = _mm512_max_ps(Bouta, Boutb);
	H3 = _mm512_max_ps(Couta, Coutb);
	H4 = _mm512_max_ps(Douta, Doutb);


	Aouta = _mm512_permutex2var_ps(L1, _mm512_set_epi32(29, 28, 13, 12, 25, 24, 9, 8, 21, 20, 5, 4, 17, 16, 1, 0), H1);
	Bouta = _mm512_permutex2var_ps(L2, _mm512_set_epi32(29, 28, 13, 12, 25, 24, 9, 8, 21, 20, 5, 4, 17, 16, 1, 0), H2);
	Couta = _mm512_permutex2var_ps(L3, _mm512_set_epi32(29, 28, 13, 12, 25, 24, 9, 8, 21, 20, 5, 4, 17, 16, 1, 0), H3);
	Douta = _mm512_permutex2var_ps(L4, _mm512_set_epi32(29, 28, 13, 12, 25, 24, 9, 8, 21, 20, 5, 4, 17, 16, 1, 0), H4);

	Aoutb = _mm512_permutex2var_ps(L1, _mm512_set_epi32(31, 30, 15, 14, 27, 26, 11, 10, 23, 22, 7, 6, 19, 18, 3, 2), H1);
	Boutb = _mm512_permutex2var_ps(L2, _mm512_set_epi32(31, 30, 15, 14, 27, 26, 11, 10, 23, 22, 7, 6, 19, 18, 3, 2), H2);
	Coutb = _mm512_permutex2var_ps(L3, _mm512_set_epi32(31, 30, 15, 14, 27, 26, 11, 10, 23, 22, 7, 6, 19, 18, 3, 2), H3);
	Doutb = _mm512_permutex2var_ps(L4, _mm512_set_epi32(31, 30, 15, 14, 27, 26, 11, 10, 23, 22, 7, 6, 19, 18, 3, 2), H4);


	//L4 to L5
	L1 = _mm512_min_ps(Aouta, Aoutb);
	L2 = _mm512_min_ps(Bouta, Boutb);
	L3 = _mm512_min_ps(Couta, Coutb);
	L4 = _mm512_min_ps(Douta, Doutb);

	H1 = _mm512_max_ps(Aouta, Aoutb);
	H2 = _mm512_max_ps(Bouta, Boutb);
	H3 = _mm512_max_ps(Couta, Coutb);
	H4 = _mm512_max_ps(Douta, Doutb);

	Aouta = _mm512_permutex2var_ps(L1, _mm512_set_epi32(30, 14, 28, 12, 26, 10, 24, 8, 22, 6, 20, 4, 18, 2, 16, 0), H1);
	Bouta = _mm512_permutex2var_ps(L2, _mm512_set_epi32(30, 14, 28, 12, 26, 10, 24, 8, 22, 6, 20, 4, 18, 2, 16, 0), H2);
	Couta = _mm512_permutex2var_ps(L3, _mm512_set_epi32(30, 14, 28, 12, 26, 10, 24, 8, 22, 6, 20, 4, 18, 2, 16, 0), H3);
	Douta = _mm512_permutex2var_ps(L4, _mm512_set_epi32(30, 14, 28, 12, 26, 10, 24, 8, 22, 6, 20, 4, 18, 2, 16, 0), H4);

	Aoutb = _mm512_permutex2var_ps(L1, _mm512_set_epi32(31, 15, 29, 13, 27, 11, 25, 9, 23, 7, 21, 5, 19, 3, 17, 1), H1);
	Boutb = _mm512_permutex2var_ps(L2, _mm512_set_epi32(31, 15, 29, 13, 27, 11, 25, 9, 23, 7, 21, 5, 19, 3, 17, 1), H2);
	Coutb = _mm512_permutex2var_ps(L3, _mm512_set_epi32(31, 15, 29, 13, 27, 11, 25, 9, 23, 7, 21, 5, 19, 3, 17, 1), H3);
	Doutb = _mm512_permutex2var_ps(L4, _mm512_set_epi32(31, 15, 29, 13, 27, 11, 25, 9, 23, 7, 21, 5, 19, 3, 17, 1), H4);

	//L5
	L1 = _mm512_min_ps(Aouta, Aoutb);
	L2 = _mm512_min_ps(Bouta, Boutb);
	L3 = _mm512_min_ps(Couta, Coutb);
	L4 = _mm512_min_ps(Douta, Doutb);

	H1 = _mm512_max_ps(Aouta, Aoutb);
	H2 = _mm512_max_ps(Bouta, Boutb);
	H3 = _mm512_max_ps(Couta, Coutb);
	H4 = _mm512_max_ps(Douta, Doutb);

	Aouta = _mm512_permutex2var_ps(L1, _mm512_set_epi32(23, 7, 22, 6, 21, 5, 20, 4, 19, 3, 18, 2, 17, 1, 16, 0), H1);
	Bouta = _mm512_permutex2var_ps(L2, _mm512_set_epi32(23, 7, 22, 6, 21, 5, 20, 4, 19, 3, 18, 2, 17, 1, 16, 0), H2);
	Couta = _mm512_permutex2var_ps(L3, _mm512_set_epi32(23, 7, 22, 6, 21, 5, 20, 4, 19, 3, 18, 2, 17, 1, 16, 0), H3);
	Douta = _mm512_permutex2var_ps(L4, _mm512_set_epi32(23, 7, 22, 6, 21, 5, 20, 4, 19, 3, 18, 2, 17, 1, 16, 0), H4);

	Aoutb = _mm512_permutex2var_ps(L1, _mm512_set_epi32(31, 15, 30, 14, 29, 13, 28, 12, 27, 11, 26, 10, 25, 9, 24, 8), H1);
	Boutb = _mm512_permutex2var_ps(L2, _mm512_set_epi32(31, 15, 30, 14, 29, 13, 28, 12, 27, 11, 26, 10, 25, 9, 24, 8), H2);
	Coutb = _mm512_permutex2var_ps(L3, _mm512_set_epi32(31, 15, 30, 14, 29, 13, 28, 12, 27, 11, 26, 10, 25, 9, 24, 8), H3);
	Doutb = _mm512_permutex2var_ps(L4, _mm512_set_epi32(31, 15, 30, 14, 29, 13, 28, 12, 27, 11, 26, 10, 25, 9, 24, 8), H4);

}
