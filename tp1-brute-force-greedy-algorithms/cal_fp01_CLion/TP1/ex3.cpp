// By: Gonçalo Leão

#include "exercises.h"
#include <vector>
#include <iostream>
#include <algorithm>

template <class T> std::vector<std::vector<T>> makeComb(const std::vector<T>& vec, std::vector<std::vector<T>>& tmp, unsigned curr) {
    for (unsigned i = tmp.size()/2; i < tmp.size(); ++i){ // insert the element in half of the vector
        tmp.at(i).push_back(vec.at(curr));
    }
    if (curr >= vec.size() - 1) return tmp;
    unsigned currTmpSize = tmp.size();
    for (unsigned i = 0; i < currTmpSize; ++i) tmp.push_back(tmp.at(i)); // repeat the elements
    return makeComb(vec, tmp, curr + 1);
}

template <class T> std::vector<std::vector<T>> combinations(const std::vector<T>& vec){
    std::vector<std::vector<T>> tmp = {{},{}};
    return !vec.empty() ? makeComb(vec, tmp, 0) : std::vector<std::vector<T>>();
}

bool changeMakingBF(unsigned int C[], unsigned int Stock[], unsigned int n, unsigned int T, unsigned int usedCoins[]) {
    std::vector<unsigned int> coins;
    for (unsigned i = 0; i < n; ++i){ // push_back all coins in the pocket
        for (unsigned s = 0; s < Stock[i]; ++s) coins.push_back(C[i]);
    }
    auto comb = combinations(coins);
    unsigned minCoinsUsed = 99999;
    std::vector<unsigned> bestComb;
    for (const auto& c: comb) {
        if (c.empty()) continue;
        unsigned sum = 0;
        for (const unsigned e: c) sum += e;
        if (sum == T && c.size() < minCoinsUsed){
            minCoinsUsed = c.size();
            bestComb = c;
        }
    }
    if (bestComb.empty()) return false;
    for (unsigned i = 0; i < n; ++i){
        usedCoins[i] = std::count(bestComb.begin(), bestComb.end(), C[i]);
    }
    return true;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP1_Ex3, myOne){
    unsigned int C[] = {1,2,3};
    unsigned int Stock[] = {2,2,2};
    unsigned int n = 3;
    unsigned int usedCoins[3];

    EXPECT_EQ(changeMakingBF(C,Stock,n,3,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);
}


TEST(TP1_Ex3, hasBFChangeCanonical) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {1,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingBF(C,Stock,n,13,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);

    unsigned int Stock2[] = {1,2,4,2};
    EXPECT_EQ(changeMakingBF(C,Stock2,n,38,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 3);
    EXPECT_EQ(usedCoins[3], 2);
}


TEST(TP1_Ex3, hasBFChangeNonCanonical) {
    unsigned int C[] = {1,4,5};
    unsigned int Stock[] = {3,2,1};
    unsigned int n = 3;
    unsigned int usedCoins[3];

    EXPECT_EQ(changeMakingBF(C,Stock,n,6,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingBF(C,Stock,n,8,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);
}

TEST(TP1_Ex3, hasNoBFChange) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {0,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingBF(C,Stock,n,18,usedCoins), false);
    EXPECT_EQ(changeMakingBF(C,Stock,n,1,usedCoins), false);
}
