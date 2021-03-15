#include <sstream>
#include "exercises.h"

std::string calcSum(const int sequence[], unsigned long n) {
    int table[n+1][n]; // table[i][j] = sum with len i, starting at j
    std::stringstream res;

    // Initialize table
    for (int i = 0; i <= n; i++){
        for (int j = 0; j < n; j++){
            if (i == 1) table[i][j] = sequence[j]; // base cases
            else table[i][j] = INT32_MAX;
        }
    }

    for (int len = 1; len <= n; len++){
        int bestIdx = 0;
        int currLessSum = INT32_MAX;
        for (int i = 0; i <= n - len; i++){
            if (len != 1){
                table[len][i] = table[len-1][i] + sequence[i + len - 1];
            }
            if (table[len][i] < currLessSum){
                currLessSum = table[len][i];
                bestIdx = i;
            }
        }
        res << currLessSum << ',' << bestIdx << ';';
    }

    return res.str();
}

void testPerformanceCalcSum() {
    //TODO
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex4, CalcSumArrayTest) {
    int sequence[5] = {4,7,2,8,1};
    int sequence2[9] = {6,1,10,3,2,6,7,2,4};

    EXPECT_EQ("1,4;9,1;11,2;18,1;22,0;",calcSum(sequence, 5));
    EXPECT_EQ("1,1;5,3;11,3;16,1;20,3;24,3;31,1;35,1;41,0;",calcSum(sequence2, 9));
	
	testPerformanceCalcSum();
}
