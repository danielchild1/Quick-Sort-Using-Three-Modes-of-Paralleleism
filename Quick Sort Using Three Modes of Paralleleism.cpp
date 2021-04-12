// Quick Sort Using Three Modes of Paralleleism.cpp : Defines the entry point for the application.
//

#include "Quick Sort Using Three Modes of Paralleleism.h"
#include <immintrin.h>
#include <unistd.h>
#include <cstdlib>
#include <omp.h>
#include <cstdio>
#define N 33'554'432 // 2^25

using namespace std;

int main()
{
	float* megaArray[N];
	float* megaOut[N];

	fillArray(*megaArray);



	// openMP thread dynamic scheduler with for loop {
	//#pragma omp parallel for schedual(dynamic) num_threads(64)
	for (unsigned int i = 0; i < N; i+= 65536) {
		printf("Thread %d is ready to work within range [%d, %d).\n", omp_get_thread_num(), i, (i + 65536));


		int startIndex = i; // determine the start index of this block of 65536 elements
		int endIndex = (i + 65536); // determine the end index of this block of 65536 elements

		for (int sort = startIndex; sort < endIndex; sort += 16) {
			selectionSort(*megaArray, sort);
		}
		

		int sortedBlockSize = 16;
		int edingSortedBlockSize = 16384;

		__m512 Aa, Ab, Ba, Bb, Ca, Cb, Da, Db;
		__m512 Aouta, Aoutb, Bouta, Boutb, Couta, Coutb, Douta, Doutb;

		float* inputPointer = *megaArray;
		float* outputPointer = *megaOut;

		while (sortedBlockSize <= edingSortedBlockSize) {
			//sortedBlockSize starts at 16 and is doubled every while loop iteration until it reaches 16384

			for (int arrIndex = startIndex; arrIndex < endIndex; arrIndex += sortedBlockSize * 8) {

				int startA1 = arrIndex;
				int startA2 = arrIndex + sortedBlockSize +1;
				int startB1 = arrIndex + (2 * sortedBlockSize) +1;
				int startB2 = arrIndex + (3 * sortedBlockSize) + 1;;
				int startC1 = arrIndex + (4 * sortedBlockSize) + 1;;
				int startC2 = arrIndex + (5 * sortedBlockSize) + 1;;
				int startD1 = arrIndex + (6 * sortedBlockSize) + 1;;
				int startD2 = arrIndex + (7 * sortedBlockSize) + 1;;


				int endA1 = arrIndex + sortedBlockSize;
				int endA2 = arrIndex + (2 * sortedBlockSize);
				int endB1 = arrIndex + (3 * sortedBlockSize);
				int endB2 = arrIndex + (4 * sortedBlockSize);
				int endC1 = arrIndex + (5 * sortedBlockSize);
				int endC2 = arrIndex + (6 * sortedBlockSize);
				int endD1 = arrIndex + (7 * sortedBlockSize);
				int endD2 = arrIndex + (8 * sortedBlockSize);

				Aa = _mm512_loadu_ps(&outputPointer[startA1]);
				Ab = _mm512_loadu_ps(&outputPointer[startA2]);
				Ba = _mm512_loadu_ps(&outputPointer[startB1]);
				Bb = _mm512_loadu_ps(&outputPointer[startB2]);
				Ca = _mm512_loadu_ps(&outputPointer[startC1]);
				Cb = _mm512_loadu_ps(&outputPointer[startC2]);
				Da = _mm512_loadu_ps(&outputPointer[startD1]);
				Db = _mm512_loadu_ps(&outputPointer[startD2]);
				

				int writeA = startA1;
				int writeB = startB1;
				int writeC = startC1;
				int writeD = startD1;

				for (int j = 0; j < (sortedBlockSize / 8)-1; j++) {
					
					bitonicSort(Aa, Ab, Ba, Bb, Ca, Cb, Da, Db, Aouta, Aoutb, Bouta, Boutb, Couta, Coutb, Douta, Doutb);

					_mm512_storeu_ps(&outputPointer[writeA], Aouta);
					_mm512_storeu_ps(&outputPointer[writeB], Bouta);
					_mm512_storeu_ps(&outputPointer[writeC], Couta);
					_mm512_storeu_ps(&outputPointer[writeD], Douta);

					
					writeA += 16;
					writeB += 16;
					writeC += 16;
					writeD += 16;

					Aa = Aoutb;
					Ba = Boutb;
					Ca = Coutb;
					Da = Doutb;


					if (j == (sortedBlockSize / 8) - 2) {
						_mm512_storeu_ps(&outputPointer[writeA], Aoutb);
						_mm512_storeu_ps(&outputPointer[writeB], Boutb);
						_mm512_storeu_ps(&outputPointer[writeC], Coutb);
						_mm512_storeu_ps(&outputPointer[writeD], Doutb);
					}
					else { //A
						if (startA1 == endA1) {
							Ab = _mm512_loadu_ps(&inputPointer[startA2]);
							startA2 += 16;
						}
						else if (startA2 == endA2) {
							Ab = _mm512_loadu_ps(&inputPointer[startA1]);
							startA1 += 16;
						}
						else if (outputPointer[startA1+16] < outputPointer[startA2+16]) {
							Ab = _mm512_loadu_ps(&inputPointer[startA1]);
							startA1 += 16;
						}
						else {
							Ab = _mm512_loadu_ps(&inputPointer[startA2]);
							startA2 += 16;
						}
						//B
						if (startB1 == endB1) {
							Bb = _mm512_loadu_ps(&inputPointer[startB2]);
							startB2 += 16;
						}
						else if (startB2 == endB2) {
							Bb = _mm512_loadu_ps(&inputPointer[startB1]);
							startB1 += 16;
						}
						else if (outputPointer[startB1 + 16] < outputPointer[startB2 + 16]) {
							Bb = _mm512_loadu_ps(&inputPointer[startB1]);
							startB1 += 16;
						}
						else {
							Bb = _mm512_loadu_ps(&inputPointer[startB2]);
							startB2 += 16;
						}
						//C
						if (startC1 == endC1) {
							Cb = _mm512_loadu_ps(&inputPointer[startC2]);
							startC2 += 16;
						}
						else if (startC2 == endC2) {
							Cb = _mm512_loadu_ps(&inputPointer[startC1]);
							startC1 += 16;
						}
						else if (outputPointer[startC1 + 16] < outputPointer[startC2 + 16]) {
							Cb = _mm512_loadu_ps(&inputPointer[startC1]);
							startC1 += 16;
						}
						else {
							Cb = _mm512_loadu_ps(&inputPointer[startC2]);
							startC2 += 16;
						}
						//D
						if (startD1 == endD1) {
							Db = _mm512_loadu_ps(&inputPointer[startD2]);
							startD2 += 16;
						}
						else if (startD2 == endD2) {
							Db = _mm512_loadu_ps(&inputPointer[startD1]);
							startD1 += 16;
						}
						else if (outputPointer[startD1 + 16] < outputPointer[startD2 + 16]) {
							Db = _mm512_loadu_ps(&inputPointer[startD1]);
							startD1 += 16;
						}
						else {
							Db = _mm512_loadu_ps(&inputPointer[startD2]);
							startD2 += 16;
						}
					}
				}
				float* temp = outputPointer;
				outputPointer = inputPointer;
				inputPointer = temp;

				sortedBlockSize *= 2;
				//exchange input and output pointers, 

			}
			//deallocate output array
			delete[] outputPointer;
			delete[] inputPointer;
		}// End OpenMP for loop
	}

	return 0;
}
