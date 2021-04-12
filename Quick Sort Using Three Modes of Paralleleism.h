// Quick Sort Using Three Modes of Paralleleism.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <immintrin.h>
#include <random>
#define N 33'554'432 // 2^25


void selectionSort(float* newArr) {
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

void selectionSort(float* newArr, int startIndex) {
	int tempStorage, iterator;

	for (int i = startIndex; i < 15 + startIndex; i++) {
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
__m512i m512_generator(int* a) {
	const int maxIntValue = 4294967296;
	int max = 10000000;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distrobution(0, maxIntValue);

	__m512i first = _mm512_load_si512(&a[0]); // Go get the first vector
	for (int i = 16; i < max; i += 16) {
		__m512i second = _mm512_load_si512(&a[i]); // Get the next vector
		first = _mm512_max_epi32(first, second);
	}

	

	return first;
}


void fillArray(int* big) {
	//-2,147,483,647
	//2147483647

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(-2147483647, 2147483647);

	for (unsigned int i = 0; i < N; i++) {
		big[i] = distribution(generator);
	}
}


void fillArray(float* big) {//overrides incase I switch to floats

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(-2147483647, 2147483647);

	for (unsigned int i = 0; i < N; i++) {
		big[i] = (float)distribution(generator);
	}
}


void bitonicSort(const __m512& Aa, const __m512& Ab, const __m512& Ba, const __m512& Bb, const __m512& Ca, const __m512& Cb, const __m512& Da, const __m512& Db,
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


void printVectorInt(__m512i v, string name)
{
#if defined (__GNUC__)
	int* temp = (int*)aligned_alloc(64, sizeof(int) * 16);
#elif defined (_MSC_VER)
	int* temp = (int*)_aligned_malloc(sizeof(int) * 16, 64);
#endif
	_mm512_store_si512(temp, v);
	printf("The vector called %s contains: ", name.c_str());
	for (int i = 0; i < 16; i++)
	{
		printf("%02d ", temp[i]);
	}
	printf("\n");
#if defined (__GNUC__)
	free(temp);
#elif defined (_MSC_VER)
	_aligned_free(temp);
#endif
}
void printVectorFloat(__m512 v, string name)
{
#if defined (__GNUC__)
	float* temp = (float*)aligned_alloc(64, sizeof(float) * 16);
#elif defined (_MSC_VER)
	float* temp = (float*)_aligned_malloc(sizeof(float) * 16, 64);
#endif
	_mm512_store_ps(temp, v);
	printf("The vector called %s contains: ", name.c_str());
	for (int i = 0; i < 16; i++)
	{
		printf("%3f ", temp[i]);
	}
	printf("\n");
#if defined (__GNUC__)
	free(temp);
#elif defined (_MSC_VER)
	_aligned_free(temp);
#endif
}
