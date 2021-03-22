#include "exercises.h"

bool changeMakingBacktrackingRec(unsigned int C[], unsigned int Stock[], unsigned int n, unsigned int T,
                                 unsigned curIndex, unsigned &curBestNumberOfCoins, unsigned curCoins[], unsigned bestCoins[]){
    // Base cases
    if (T == 0){
        unsigned used = 0;
        for (unsigned i = 0; i < n; ++i){
            used += curCoins[i];
        }
        if (used < curBestNumberOfCoins){
            for (unsigned i = 0; i < n; ++i) bestCoins[i] = curCoins[i];
            curBestNumberOfCoins = used;
        }
        return true;
    }
    else if (curIndex == n){
        return false;
    }

    // Try to add one more coin
    bool foundWithCoin = false;
    if (curCoins[curIndex] < Stock[curIndex] && C[curIndex] <= T){
        curCoins[curIndex]++;
        foundWithCoin = changeMakingBacktrackingRec(C,Stock,n,T - C[curIndex], curIndex, curBestNumberOfCoins, curCoins, bestCoins);
        curCoins[curIndex]--; // reset curCoins for the next coin trial
    }

    // Try to switch to the next coin
    bool foundWithoutCoin = changeMakingBacktrackingRec(C,Stock,n,T,curIndex+1, curBestNumberOfCoins, curCoins, bestCoins);

    return foundWithCoin || foundWithoutCoin;
}

bool changeMakingBacktracking(unsigned int C[], unsigned int Stock[], unsigned int n, unsigned int T, unsigned int usedCoins[]) {
    unsigned minCoins = 99999;
    unsigned currCandidate[n];
    for (unsigned i = 0; i < n; ++i){
        currCandidate[i] = 0;
        usedCoins[i] = 0;
    }
    return changeMakingBacktrackingRec(C, Stock, n, T, 0, minCoins, currCandidate, usedCoins);
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP2_Ex3, simpleTest){
    unsigned int C[] = {1,2,3};
    unsigned int Stock[] = {1,2,1};
    unsigned int n = 3;
    unsigned int usedCoins[3];
    EXPECT_EQ(changeMakingBacktracking(C,Stock,n,5,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 1);
}

TEST(TP2_Ex3, hasBFChangeCanonical) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {1,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingBacktracking(C,Stock,n,13,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);

    unsigned int Stock2[] = {1,2,4,2};
    EXPECT_EQ(changeMakingBacktracking(C,Stock2,n,38,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 3);
    EXPECT_EQ(usedCoins[3], 2);

    unsigned int Stock3[] = {10,2,4,2};
    EXPECT_EQ(changeMakingBacktracking(C,Stock3,n,10,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);
}

TEST(TP2_Ex3, hasBFChangeNonCanonical) {
    unsigned int C[] = {1,4,5};
    unsigned int Stock[] = {2,2,1};
    unsigned int n = 3;
    unsigned int usedCoins[3];

    EXPECT_EQ(changeMakingBacktracking(C,Stock,n,6,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingBacktracking(C,Stock,n,8,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);

    unsigned int Stock2[] = {8,2,1};
    EXPECT_EQ(changeMakingBacktracking(C,Stock2,n,7,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 2);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingBacktracking(C,Stock2,n,8,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);
}

TEST(TP2_Ex3, hasNoBFChange) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {0,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingBacktracking(C,Stock,n,18,usedCoins), false);
    EXPECT_EQ(changeMakingBacktracking(C,Stock,n,1,usedCoins), false);
}