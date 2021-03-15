#include "exercises.h"
#include "../TP1/exercises.h"
#include "../TP3/exercises.h"

int maxSubsequenceDP(int A[], unsigned int n, unsigned int &i, unsigned int &j) {
    int table[n]; // max value ending at index
    int bestMax = A[0], curI = 0;
    table[0] = A[0];

    for (int idx = 1; idx < (int)n; idx++){
        if (A[idx] + table[idx-1] > A[idx]) {
            table[idx] = A[idx] + table[idx - 1];
        } else {
            curI = idx;
            table[idx] = A[idx];
        }
        if (table[idx] > bestMax){
            i = curI;
            j = idx;
            bestMax = table[idx];
        }
    }
    return bestMax;
}

void testPerformanceMaxSubsequence() {
	// TODO
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex6, testMaxSubsequence) {
    int A1[] = {1, 2, 3, 4};
    unsigned int n1 = 4;
    unsigned int i, j;
    EXPECT_EQ(maxSubsequenceDP(A1,n1,i,j), 10);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A2[] = {2, -1, -3, 4};
    unsigned int n2 = 4;
    EXPECT_EQ(maxSubsequenceDP(A2,n2,i,j), 4);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 3);

    int A3[] = {2, -1, 3, 4};
    unsigned int n3 = 4;
    EXPECT_EQ(maxSubsequenceDP(A3,n3,i,j), 8);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A4[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    unsigned int n4 = 9;
    EXPECT_EQ(maxSubsequenceDP(A4,n4,i,j), 6);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 6);

    testPerformanceMaxSubsequence();
}