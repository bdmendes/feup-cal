// By: Gonçalo Leão

#include "exercises.h"
#include "algorithm"

bool isCanonical(unsigned int C[], unsigned int n) {
    if (n < 3) return false;
    unsigned min = C[2] + 1, max = C[n-1] + C[n-2];
    unsigned Stock[n];
    for (unsigned T = min + 1; T < max; ++T){
        for (unsigned j = 0; j < n; ++j){
            Stock[j] = n;
        }
        unsigned usedCoinsBruteForce[n];
        unsigned usedCoinsGreedy[n];
        changeMakingBF(C,Stock,n,T,usedCoinsBruteForce);
        changeMakingGreedy(C,Stock,n,T,usedCoinsGreedy);
        unsigned sumBruteForce = 0, sumGreedy = 0;
        for (unsigned i = 0; i < n; ++i){
            sumBruteForce += usedCoinsBruteForce[i];
            sumGreedy += usedCoinsGreedy[i];
        }
        if (sumBruteForce != sumGreedy) return false;
    }
    return true;
}


/// TESTS ///
#include <gtest/gtest.h>

TEST(TP1_Ex5, canonicalTrue) {
    unsigned int C[] = {1,2,5};
    EXPECT_EQ(isCanonical(C,3), true);

    unsigned int C2[] = {1,2,5,10};
    EXPECT_EQ(isCanonical(C2,4), true);
}

TEST(TP1_Ex5, canonicalFalse) {
    unsigned int C[] = {1,4,5};
    EXPECT_EQ(isCanonical(C,3), false);
}