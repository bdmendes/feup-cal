#include "exercises.h"

int maxIndex(const int A[], int n){
    int curMax = -99999, curIdx = 0;
    for (int i = 0; i < n; ++i){
        if (A[i] > curMax){
            curMax = A[i];
            curIdx = i;
        }
    }
    return curIdx;
}

int maxCrossingSubsequence(int A[], int i, int j, int& iRes, int& jRes){
    int mid = (i+j)/2;

    // Start at mid, and go left to find max
    int leftSum = 0, maxLeftSum = -99999;
    for (int l = mid; l >= 0; --l){
        leftSum += A[l];
        if (leftSum > maxLeftSum){
            maxLeftSum = leftSum;
            iRes = l;
        }
    }

    // Start after mid, and go right to find max
    int rightSum = 0, maxRightSum = -99999;
    for (int l = mid + 1; l <= j; ++l){
        rightSum += A[l];
        if (rightSum > maxRightSum){
            maxRightSum = rightSum;
            jRes = l;
        }
    }

    return maxLeftSum + maxRightSum;
}

int maxSubsequenceDCRec(int A[], int i, int j, int& bestI, int& bestJ, int& curMax){
    if (i == j){
        return A[i];
    }

    int mid = (i+j)/2;
    int leftMax = maxSubsequenceDCRec(A, i, mid, bestI, bestJ, curMax);
    int rightMax = maxSubsequenceDCRec(A, mid + 1, j, bestI, bestJ, curMax);
    int crossingI = mid, crossingJ = mid;
    int crossingMax = maxCrossingSubsequence(A, i, j, crossingI, crossingJ);

    int options[] = {leftMax, rightMax, crossingMax};
    int maxIdx = maxIndex(options, 3);
    switch (maxIdx){
        case 0:
            if (leftMax > curMax){
                curMax = leftMax;
                bestI = i;
                bestJ = mid;
            }
            break;
        case 1:
            if (rightMax > curMax){
                curMax = rightMax;
                bestI = mid + 1;
                bestJ = j;
            }
            break;
        case 2:
            if (crossingMax > curMax){
                curMax = crossingMax;
                bestI = crossingI;
                bestJ = crossingJ;
            }
            break;
        default:
            break;
    }
    return options[maxIdx];
}

int maxSubsequenceDC(int A[], unsigned int n, int &i, int &j) {
    if (n == 0) return 0;
    int bestI, bestJ, curMax = -99999;
    int max = maxSubsequenceDCRec(A, 0, (int)n-1, bestI, bestJ, curMax);
    i = bestI;
    j = bestJ;
    return max;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP3_Ex2, testMaxSubsequence) {
    int A1[] = {1, 2, 3, 4};
    unsigned int n1 = 4;
    int i, j;
    EXPECT_EQ(maxSubsequenceDC(A1,n1,i,j), 10);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A2[] = {2, -1, -3, 4};
    unsigned int n2 = 4;
    EXPECT_EQ(maxSubsequenceDC(A2,n2,i,j), 4);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 3);

    int A3[] = {2, -1, 3, 4};
    unsigned int n3 = 4;
    EXPECT_EQ(maxSubsequenceDC(A3,n3,i,j), 8);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A4[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    unsigned int n4 = 9;
    EXPECT_EQ(maxSubsequenceDC(A4,n4,i,j), 6);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 6);
}