#include "exercises.h"

#define UNDEFINED 99999

unsigned long s_recursive(unsigned int n, unsigned int k) {
    if (k < 1 || k > n) return UNDEFINED;
    if (k == 1 || k == n) return 1;
    return s_recursive(n-1,k-1) + k*s_recursive(n-1,k);
}

unsigned long b_recursive(unsigned int n) {
    unsigned long res = 0;
    for (unsigned k = 1; k <= n; k++){
        res += s_recursive(n,k);
    }
    return res;
}

unsigned long s_dynamic(unsigned int n, unsigned int k) {
    static std::unordered_map<std::string, unsigned long> memo;
    if (k < 1 || k > n) return UNDEFINED;
    if (k == 1 || k == n) return 1;
    std::string key = std::to_string(n) + "," + std::to_string(k);
    if (memo.find(key) != memo.end()) return memo.at(key);
    memo[key] = s_dynamic(n-1,k-1) + k*s_dynamic(n-1,k);
    return memo.at(key);
}

unsigned long b_dynamic(unsigned int n) {
    unsigned long res = 0;
    for (unsigned k = 1; k <= n; k++){
        res += s_dynamic(n,k);
    }
    return res;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex5, testPartitioningRecursive) {
    EXPECT_EQ(1,s_recursive(3,3));
    EXPECT_EQ(3,s_recursive(3,2));
    EXPECT_EQ(3025,s_recursive(9,3));
    EXPECT_EQ(22827,s_recursive(10,6));
    EXPECT_EQ(4040709321359012032, s_recursive(30,15));

    EXPECT_EQ(5,b_recursive(3));
    EXPECT_EQ(203,b_recursive(6));
    EXPECT_EQ(1382958545,b_recursive(15));
}

TEST(TP4_Ex5, testPartitioningDynamic) {
    EXPECT_EQ(1,s_dynamic(3,3));
    EXPECT_EQ(3,s_dynamic(3,2));
    EXPECT_EQ(3025,s_dynamic(9,3));
    EXPECT_EQ(22827,s_dynamic(10,6));
    EXPECT_EQ(4040709321359012032, s_dynamic(30,15));

    EXPECT_EQ(5,b_dynamic(3));
    EXPECT_EQ(203,b_dynamic(6));
    EXPECT_EQ(1382958545,b_dynamic(15));
}