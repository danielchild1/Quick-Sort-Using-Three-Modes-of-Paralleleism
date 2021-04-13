// Quick Sort Using Three Modes of Paralleleism.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <immintrin.h>
#include <random>
#include <omp.h>
#define N 33'554'432 // 2^25

void selectionSort(float *newArr)
{
	int tempStorage, iterator;

	for (int i = 0; i < 15; i++)
	{
		iterator = i;
		for (int j = i + 1; j < 16; j++)
		{
			if (newArr[j] < newArr[iterator])
			{
				iterator = j;
			}
		}

		//swap values
		tempStorage = newArr[iterator];
		newArr[iterator] = newArr[i];
		newArr[i] = tempStorage;
	}
}

//Does not work
void selectionSort(float *newArr, int startIndex)
{
	int iterator;//Does not work
	float tempStorage;//Does not work

	int i, j, smallest;//Does not work
	for (i = startIndex; i < 15 + startIndex; i++)//Does not work
	{
		smallest = i;//Does not work
		for (j = i + 1; j < 16 + startIndex; j++)//Does not work
		{

			float sma = newArr[smallest];//Does not work
			float jj = newArr[j];
//Does not work
			if (newArr[j] < newArr[smallest])//Does not work
			{
				smallest = j;//Does not work

				float temp;
				temp = newArr[i];//Does not work
				newArr[i] = newArr[smallest];//Does not work
				newArr[smallest] = temp;//Does not work
			}
		}
	}
}

void bubbleSort(float *mainArray)
{

#pragma omp parallel for schedule(dynamic) num_threads(64)
	for (int unit = 0; unit < N; unit += 16)
	{
		for (int i = unit; i < unit + 16; i++)
		{
			for (int j = unit + 1; j < unit + 16; j++)
			{
				if (mainArray[j] < mainArray[j - 1])
				{
					float temp = mainArray[j];
					mainArray[j] = mainArray[j - 1];
					mainArray[j - 1] = temp;
				}
			}
		}
	}
}

void fillArray(int *big)
{
	//-2,147,483,647
	//2147483647

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(-2147483647, 2147483647);

	for (unsigned int i = 0; i < N; i++)
	{
		big[i] = distribution(generator);
	}
}

void fillArray(float *big)
{ //overrides incase I switch to floats

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1, 1744483647);

#pragma omp parallel for schedule(dynamic) num_threads(64)
	for (unsigned int spacer = 0; spacer < N; spacer += 16384)
	{
		for (unsigned int i = spacer; i < spacer+16384; i++)
		{
			big[i] = (float)distribution(generator);
		}
	}
}

bool validator(float *bigData)
{
	bool validated = true;

#pragma omp parallel for schedule(dynamic) num_threads(64)
	for (unsigned int k = 0; k < N; k += 16384)
	{
		for (unsigned int i = k; i < k + 16384; i++)
		{

			if (bigData[i] > bigData[i + 1] && (i + 1) % 16384 != 0)
			{

				validated = false;
				//printf("%.0f, %.0f\n", bigData[i],bigData[i+1]);
			}
		}
	}
	return validated;
}

void bitonicSort(const __m512 &Aa, const __m512 &Ab, const __m512 &Ba, const __m512 &Bb, const __m512 &Ca, const __m512 &Cb, const __m512 &Da, const __m512 &Db,
				 __m512 &Aouta, __m512 &Aoutb, __m512 &Bouta, __m512 &Boutb, __m512 &Couta, __m512 &Coutb, __m512 &Douta, __m512 &Doutb)
{

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

	Aouta = _mm512_permutex2var_ps(L1, _mm512_set_epi32(23, 22, 21, 20, 19, 18, 17, 16, 7, 6, 5, 4, 3, 2, 1, 0), H1);
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