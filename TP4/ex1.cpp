#include "exercises.h"

unsigned long factorialRecurs(unsigned long n) {
    if (n == 0 || n == 1) return 1;
    return n * factorialRecurs(n-1);
}

unsigned long factorialDP(unsigned long n) {
    unsigned long memo[n+1];
    memo[0] = memo[1] = 1;
    for (int i = 2; i <= n; i++) memo[i] = i;
    for (int i = 1; i < n; ++i){
        memo[i+1] *= memo[i];
    }
    return memo[n];
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex1, FactorialTestRecursive) {
    EXPECT_EQ(1,factorialRecurs(0));
    EXPECT_EQ(1,factorialRecurs(1));
    EXPECT_EQ(2,factorialRecurs(2));
    EXPECT_EQ(6,factorialRecurs(3));
    EXPECT_EQ(24,factorialRecurs(4));
    EXPECT_EQ(120,factorialRecurs(5));
    EXPECT_EQ(3628800,factorialRecurs(10));
    EXPECT_EQ(1307674368000, factorialRecurs(15));
}

TEST(TP4_Ex1, FactorialTestDynamicProgramming) {
    EXPECT_EQ(1,factorialDP(0));
    EXPECT_EQ(1,factorialDP(1));
    EXPECT_EQ(2,factorialDP(2));
    EXPECT_EQ(6,factorialDP(3));
    EXPECT_EQ(24,factorialDP(4));
    EXPECT_EQ(120,factorialDP(5));
    EXPECT_EQ(3628800,factorialDP(10));
    EXPECT_EQ(1307674368000, factorialDP(15));
}