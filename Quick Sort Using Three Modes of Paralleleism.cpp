// Quick Sort Using Three Modes of Paralleleism.cpp : Defines the entry point for the application.
//

#include "Quick Sort Using Three Modes of Paralleleism.h"
#include <immintrin.h>
#include <random>
#include <vector>

using namespace std;

int main()
{
	int sortedBlockSize = 16, endingSortedBlockSize = 16384;

	__m512 Aa, Ab, Ba, Bb, Ca, Cb, Da, Dd;
	__m512 Aouta, Aoutb, Bouta, Boutb, Couta, Coutb, Douta, Doutb;

	while (sortedBlockSize <= endingSortedBlockSize) {
		// This assignment should go up to endingSortedBlockSize = 16384 
		// In other words, sortedBlockSize should be 16, then 32, 64, 128, . . . , 16384

		// Perform any thread pool indexing here (not shown)
		int startIndex = 0; // determine the start index of this block of 65536 elements
		int endIndex = 65536;// determine the start index of this block of 65536 elements
		for (int arrIndex = startIndex; arrIndex < endIndex; arrIndex += sortedBlockSize * 8) {
			// Suppose the arrSize is 2048, sortedBlockSize is 16, and ILP is 4.
			// Then on the first round, this thread must work with a 128 element block (each ILP doing a 32 element block). After
			// the 128 element block is complete, this loop should advance to the next 128 element block, and so on. 

			Compute 8 starting indexes(startA1, startA2, startB1, startB2, startC1, startC2, startD1, startD2)
				Also compute the ending indexes for A(endA1, endA2, . . ., endD2).These will help us see when this inner while is done
				// You can use both sortedBlockSize and arrIndex to easily compute these. For example, if arrIndex = 256 and
				// sortedBlockSize is 32, then startA1 = 256, startA2 = 288, startB1 = 320, startB2 = 352, startC1 = 384, . . ., startD2 = 480.
				//. Also endA1 = 272, endA2 = 304, . . . , endD2 = 464. 
				// 

				Create 4 indexes to track where to write back out to the array(writeA, writeB, . . ., writeD).These are initialized to sta rtA1,
				startB1, . . ., startD1.
				Load 8 vectors(A1in, A2in, B1in, B2in, . . ., D2in) using your start indexes

				for (int j = 0; j < (sortedBlockSize / 8) - 1; j++) {
					// This part should loop 1 time when sortedBlockSize = 16, 3 times when sortedBlockSize = 32, 7 times when // sortedBlockSize = 64, 15 times when sortedBlockSize = 128, and so on.
					Call the bitonicSort() function, passing in the 8 outputs and the 8 inputs

						Store the relevant 4 outputs back to the array(A1out, B1out, C1out, D1out) using the write indexes.
						Increment the 4 write indexes by 16 each.
						Copy the other 4 outputs into the inputs so they can be reused(A2out into A1in, . . ., D2out into D1in)
						// Determine the other input. For example, whatever value is smaller at indexes startA1+16 or startA2+16 determines the next input.
						if (j == (sortedBlockSize / 8) - 1) {
							// This j loop is on its last iteration.
							Store the other 4 outputs back to the array
						}
						else {
							if (startA1 == endA1) {
								// 1’s side has been used up, so just use 2’s side
								startA2 += 16
									Load the A2in vector from array index startA2
							}
							else if (startA2 == endA2) {
								// 2’s side has been used up, so just use 1’s side
								startA1 += 16
									Load the A2in vector from array index startA1
							}
							else if the value at startA1 + 16 is less than the value at startA2 + 16
								// use A1’s value
								startA1 += 16
								Load the A2in vector from array index startA1
						}
		 else {
			 // use A2’s value
			startA2 += 16
			Load the A2in vector from array index startA2
			 }
			 // Repeat this if/else if/else if/else code for B, C, and D.
				}
			}
		}
	sortedBlockSize *= 2;
}



//return 0;
//}