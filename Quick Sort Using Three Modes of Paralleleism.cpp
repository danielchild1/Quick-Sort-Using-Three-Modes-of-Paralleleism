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
	int* megaArray[N];
	fillArray(*megaArray);



	// openMP thread dynamic scheduler with for loop {
	#pragma omp parallel for schedual(dynamic) num_threads(64)
	for (unsigned int i = 0; i < N; i+= 65536) {
		printf("Thread %d is ready to work within range [%d, %d).\n", omp_get_thread_num(), i, (i + 65536));


		int startIndex = i; // determine the start index of this block of 65536 elements
		int endIndex = (i + 65536); // determine the end index of this block of 65536 elements

		selectionSort(*megaArray, startIndex);

		int sortedBlockSize = 16;
		int edingSortedBlockSize = 16384;

		__m512 Aa, Ab, Ba, Bb, Ca, Cb, Da, Db;
		__m512 Aouta, Aoutb, Bouta, Boutb, Couta, Coutb, Douta, Doutb;

		int* inputPointer;
		int* outputPointer;

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

				Aa = _mm512_loadu_ps(&megaArray[startA1]);
				Ab = _mm512_loadu_ps(&megaArray[startA2]);
				Ba = _mm512_loadu_ps(&megaArray[startB1]);
				Bb = _mm512_loadu_ps(&megaArray[startB2]);
				Ca = _mm512_loadu_ps(&megaArray[startC1]);
				Cb = _mm512_loadu_ps(&megaArray[startC2]);
				Da = _mm512_loadu_ps(&megaArray[startD1]);
				Db = _mm512_loadu_ps(&megaArray[startD2]);

				int writeA = startA1;
				int writeB = startB1;
				int writeC = startC1;
				int writeD = startD1;

				for (int j = 0; j < (sortedBlockSize / 8)-1; j++) {
					
					bitonicSort(Aa, Ab, Ba, Bb, Ca, Cb, Da, Db, Aouta, Aoutb, Bouta, Boutb, Couta, Coutb, Douta, Doutb);

					_mm512_storeu_ps(&megaArray[writeA], Aouta);
					_mm512_storeu_ps(&megaArray[writeB], Bouta);
					_mm512_storeu_ps(&megaArray[writeC], Couta);
					_mm512_storeu_ps(&megaArray[writeD], Douta);

					
					writeA += sortedBlockSize;
					writeB += sortedBlockSize;
					writeC += sortedBlockSize;
					writeD += sortedBlockSize;

					if (j == (sortedBlockSize / 8) - 2) {

					}
					else { //A
						if (startA1 == endA1) {

							startA2 += 16;
						}
						else if (startA2 == endA2) {

							startA1 += 16;
						}
						else if (startA1 < startA2) {

							startA1 += 16;
						}
						else {

							startA2 += 16;
						}
						//B
						if (startB1 == endB1) {

							startB2 += 16;
						}
						else if (startB2 == endB2) {

							startB1 += 16;
						}
						else if (startB1 < startB2) {

							startB1 += 16;
						}
						else {

							startB2 += 16;
						}
						//C
						if (startC1 == endC1) {

							startC2 += 16;
						}
						else if (startC2 == endC2) {

							startC1 += 16;
						}
						else if (startC1 < startC2) {

							startC1 += 16;
						}
						else {

							startC2 += 16;
						}
						//D
						if (startD1 == endD1) {

							startD2 += 16;
						}
						else if (startD2 == endD2) {

							startD1 += 16;
						}
						else if (startD1 < startD2) {

							startD1 += 16;
						}
						else {

							startD2 += 16;
						}
					}
				}
				sortedBlockSize *= 2;
				//exchange input and output pointers, 
			}
			//deallocate output array
		
		}// End OpenMP for loop
	}

	return 0;
}



//return 0;
//}