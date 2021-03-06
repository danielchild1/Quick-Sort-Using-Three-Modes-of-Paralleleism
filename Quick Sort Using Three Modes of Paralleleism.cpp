// Quick Sort Using Three Modes of Paralleleism.cpp : Defines the entry point for the application.
//

#include "Quick Sort Using Three Modes of Paralleleism.h"
#include <immintrin.h>
#include <unistd.h>
#include <cstdlib>
#include <omp.h>
#include <cstdio>
#include <chrono>
#define N 33'554'432 // 2^25

using namespace std;

int main()
{
	float *megaArray = new float[N];
	float *megaOut = new float[N];
	fillArray(megaArray);
	bubbleSort(megaArray);

	auto start = std::chrono::high_resolution_clock::now();

// openMP thread dynamic scheduler with for loop {
#pragma omp parallel for schedule(dynamic) num_threads(64)
	for (int i = 0; i < N; i += 65536)
	{
		//printf("Thread %d is ready to work within range [%d, %d).\n", omp_get_thread_num(), i, (i + 65536));

		int startIndex = i;			// determine the start index of this block of 65536 elements
		int endIndex = (i + 65536); // determine the end index of this block of 65536 elements

		int sortedBlockSize = 16;
		int edingSortedBlockSize = 16384;

		__m512 Aa, Ab, Ba, Bb, Ca, Cb, Da, Db;
		__m512 Aouta, Aoutb, Bouta, Boutb, Couta, Coutb, Douta, Doutb;

		float *inputPointer = megaArray;
		float *outputPointer = megaOut;

		while (sortedBlockSize < edingSortedBlockSize)
		{
			//sortedBlockSize starts at 16 and is doubled every while loop iteration until it reaches 16384

			for (int arrIndex = startIndex; arrIndex < endIndex; arrIndex += sortedBlockSize * 8)
			{

				int startA1 = arrIndex;
				int startA2 = arrIndex + sortedBlockSize;
				int startB1 = arrIndex + (2 * sortedBlockSize);
				int startB2 = arrIndex + (3 * sortedBlockSize);
				int startC1 = arrIndex + (4 * sortedBlockSize);
				int startC2 = arrIndex + (5 * sortedBlockSize);
				int startD1 = arrIndex + (6 * sortedBlockSize);
				int startD2 = arrIndex + (7 * sortedBlockSize);

				int endA1 = arrIndex + sortedBlockSize;
				int endA2 = arrIndex + (2 * sortedBlockSize);
				int endB1 = arrIndex + (3 * sortedBlockSize);
				int endB2 = arrIndex + (4 * sortedBlockSize);
				int endC1 = arrIndex + (5 * sortedBlockSize);
				int endC2 = arrIndex + (6 * sortedBlockSize);
				int endD1 = arrIndex + (7 * sortedBlockSize);
				int endD2 = arrIndex + (8 * sortedBlockSize);

				Aa = _mm512_loadu_ps(&inputPointer[startA1]);
				Ab = _mm512_loadu_ps(&inputPointer[startA2]);
				Ba = _mm512_loadu_ps(&inputPointer[startB1]);
				Bb = _mm512_loadu_ps(&inputPointer[startB2]);
				Ca = _mm512_loadu_ps(&inputPointer[startC1]);
				Cb = _mm512_loadu_ps(&inputPointer[startC2]);
				Da = _mm512_loadu_ps(&inputPointer[startD1]);
				Db = _mm512_loadu_ps(&inputPointer[startD2]);

				int writeA = startA1;
				int writeB = startB1;
				int writeC = startC1;
				int writeD = startD1;

				startA1 += 16;
				startA2 += 16;
				startB1 += 16;
				startB2 += 16;
				startC1 += 16;
				startC2 += 16;
				startD1 += 16;
				startD2 += 16;

				for (int j = 0; j < (sortedBlockSize / 8) - 1; j++)
				{

					bitonicSort(Aa, Ab, Ba, Bb, Ca, Cb, Da, Db, Aouta, Aoutb, Bouta, Boutb, Couta, Coutb, Douta, Doutb);

					_mm512_storeu_ps(&outputPointer[writeA], Aouta);
					_mm512_storeu_ps(&outputPointer[writeB], Bouta);
					_mm512_storeu_ps(&outputPointer[writeC], Couta);
					_mm512_storeu_ps(&outputPointer[writeD], Douta);

					// for(int i = 0; i < )

					writeA += 16;
					writeB += 16;
					writeC += 16;
					writeD += 16;

					Aa = Aoutb;
					Ba = Boutb;
					Ca = Coutb;
					Da = Doutb;

					if (j == (sortedBlockSize / 8) - 2)
					{
						_mm512_storeu_ps(&outputPointer[writeA], Aoutb);
						_mm512_storeu_ps(&outputPointer[writeB], Boutb);
						_mm512_storeu_ps(&outputPointer[writeC], Coutb);
						_mm512_storeu_ps(&outputPointer[writeD], Doutb);
					}
					else
					{ //A
						if (startA1 == endA1)
						{
							Ab = _mm512_loadu_ps(&inputPointer[startA2]);
							startA2 += 16;
						}
						else if (startA2 == endA2)
						{
							Ab = _mm512_loadu_ps(&inputPointer[startA1]);
							startA1 += 16;
						}
						else if (inputPointer[startA1] < inputPointer[startA2])
						{
							Ab = _mm512_loadu_ps(&inputPointer[startA1]);
							startA1 += 16;
						}
						else
						{
							Ab = _mm512_loadu_ps(&inputPointer[startA2]);
							startA2 += 16;
						}
						//B
						if (startB1 == endB1)
						{
							Bb = _mm512_loadu_ps(&inputPointer[startB2]);
							startB2 += 16;
						}
						else if (startB2 == endB2)
						{
							Bb = _mm512_loadu_ps(&inputPointer[startB1]);
							startB1 += 16;
						}
						else if (inputPointer[startB1] < inputPointer[startB2])
						{
							Bb = _mm512_loadu_ps(&inputPointer[startB1]);
							startB1 += 16;
						}
						else
						{
							Bb = _mm512_loadu_ps(&inputPointer[startB2]);
							startB2 += 16;
						}
						//C
						if (startC1 == endC1)
						{
							Cb = _mm512_loadu_ps(&inputPointer[startC2]);
							startC2 += 16;
						}
						else if (startC2 == endC2)
						{
							Cb = _mm512_loadu_ps(&inputPointer[startC1]);
							startC1 += 16;
						}
						else if (inputPointer[startC1] < inputPointer[startC2])
						{
							Cb = _mm512_loadu_ps(&inputPointer[startC1]);
							startC1 += 16;
						}
						else
						{
							Cb = _mm512_loadu_ps(&inputPointer[startC2]);
							startC2 += 16;
						}
						//D
						if (startD1 == endD1)
						{
							Db = _mm512_loadu_ps(&inputPointer[startD2]);
							startD2 += 16;
						}
						else if (startD2 == endD2)
						{
							Db = _mm512_loadu_ps(&inputPointer[startD1]);
							startD1 += 16;
						}
						else if (inputPointer[startD1 ] < inputPointer[startD2 ])
						{
							Db = _mm512_loadu_ps(&inputPointer[startD1]);
							startD1 += 16;
						}
						else
						{
							Db = _mm512_loadu_ps(&inputPointer[startD2]);
							startD2 += 16;
						}
					}
				}

				//exchange input and output pointers,
			}
			float *temp = outputPointer;
			outputPointer = inputPointer;
			inputPointer = temp;

			sortedBlockSize *= 2;
		} // End OpenMP for loop
		  //deallocate output array
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> running_time = end - start;

	printf("Time: %f \n", running_time.count());

	if (validator(megaArray))
	{
		printf("Validated!\n\n");
	}
	else
	{
		printf("failed!\n\n");
		printf("%.0f, %.0f, %.0f, %.0f \n", megaArray[16382], megaArray[16383],megaArray[16384], megaArray[16385]);
	}

	delete[] megaArray;
	delete[] megaOut;

	return 0;
}
