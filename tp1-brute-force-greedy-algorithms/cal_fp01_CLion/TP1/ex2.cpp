// By: Gonçalo Leão

#include "exercises.h"

int maxSubsequence(int A[], unsigned int n, unsigned int &i, unsigned int &j) {
    int currentMax = -999999;
    for (unsigned a = 0; a < n; a++){
        for (unsigned b = a + 1; b < n; b++){
            // sum elements a to b
            int sum = 0;
            for (unsigned l = b; l >= a && l < n; l--) sum += A[l];
            if (sum > currentMax){
                currentMax = sum;
                i = a;
                j = b;
            }
        }
    }
    return currentMax;
}


/// TESTS ///
#include <gtest/gtest.h>

TEST(TP1_Ex2, maxSubsequence) {
    int A[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    unsigned int n = 9;
    unsigned int i, j;
    EXPECT_EQ(maxSubsequence(A,n,i,j), 6);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 6);
}