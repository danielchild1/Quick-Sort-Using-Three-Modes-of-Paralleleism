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

