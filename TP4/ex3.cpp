#include "exercises.h"
#include <vector>
#include <algorithm>

bool changeMakingDP(const unsigned int C[], const unsigned int Stock[], unsigned int n, unsigned int T, unsigned int usedCoins[]) {
    if (T == 0) return true;
    std::vector<std::vector<unsigned>> table(T+1,std::vector<unsigned>());
    for (int i = 0; i <= T; i++){
        if (!table.at(i).empty() || i == 0){
            for (int j = 0; j < n; j++){ // choose any of the coins
                unsigned coin = C[j];
                unsigned sum = i + coin;
                if (sum > T) continue;
                auto remainingVec = table.at(i);
                remainingVec.push_back(coin); // new pocket is previous plus chosen coin
                if (Stock[j] < std::count(remainingVec.begin(), remainingVec.end(), C[j])) continue;
                if (table.at(sum).empty() || table.at(sum).size() > remainingVec.size()){
                    table.at(sum) = remainingVec;
                }
            }
        }
    }
    if (!table.at(T).empty()){
        // build the solution
        for (int i = 0; i < n; i++) usedCoins[i] = 0;
        for (unsigned coin: table.at(T)){
            for (int i = 0; i < n; ++i){
                if (C[i] == coin) usedCoins[i]++;
            }
        }
        return true;
    }
    return false;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex3, hasChangeCanonical) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {1,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingDP(C,Stock,n,13,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);

    unsigned int Stock2[] = {1,2,4,2};
    EXPECT_EQ(changeMakingDP(C,Stock2,n,38,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 3);
    EXPECT_EQ(usedCoins[3], 2);

    unsigned int Stock3[] = {10,2,4,2};
    EXPECT_EQ(changeMakingDP(C,Stock3,n,10,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);
}

TEST(TP4_Ex3, hasChangeNonCanonical) {
    unsigned int C[] = {1,4,5};
    unsigned int Stock[] = {2,2,1};
    unsigned int n = 3;
    unsigned int usedCoins[3];

    EXPECT_EQ(changeMakingDP(C,Stock,n,6,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingDP(C,Stock,n,8,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);

    unsigned int Stock2[] = {8,2,1};
    EXPECT_EQ(changeMakingDP(C,Stock2,n,7,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 2);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingDP(C,Stock2,n,8,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);
}

TEST(TP4_Ex3, hasNoChange) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {0,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingDP(C,Stock,n,18,usedCoins), false);
    EXPECT_EQ(changeMakingDP(C,Stock,n,1,usedCoins), false);
}