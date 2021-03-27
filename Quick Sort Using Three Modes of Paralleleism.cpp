// Quick Sort Using Three Modes of Paralleleism.cpp : Defines the entry point for the application.
//

#include "Quick Sort Using Three Modes of Paralleleism.h"
#include <immintrin.h>
#include <random>
#include <vector>

using namespace std;

int main()
{
	
	//4 vectors containing 16 ints each
	vector<vector<int>> sixtyFourValues = initializer();//generates the 4 vectors and the numbers inside each vector of 16 is sorted

	vector<__m512i> vectorizedIs;

	for (auto mmi : sixtyFourValues) { //converts the vector<int> to __m512i
		vectorizedIs.push_back(vectorTOmm512i(mmi));
	}
	




	

	
	return 0;
}
