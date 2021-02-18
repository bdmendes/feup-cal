// By: Gonçalo Leão

#include "exercises.h"
#include <algorithm>

bool Activity::operator==(const Activity &a2) const {
    return start == a2.start && finish == a2.finish;
}

bool Activity::operator<(const Activity &a2) const {
    return start < a2.start;
}

std::vector<Activity> makeSchedule(std::vector<Activity> A){
    if (A.empty()) return std::vector<Activity>();
    std::vector<Activity> allowed = {A.at(0)};
    std::vector<Activity> nonOverlapping;
    for (int i = 1; i < A.size(); ++i) if (A.at(i).start >= allowed.at(0).finish) nonOverlapping.push_back(A.at(i));
    auto scan = makeSchedule(nonOverlapping);
    for (const auto& a : scan) allowed.push_back(a);
    return allowed;
}

std::vector<Activity> earliestFinishScheduling(std::vector<Activity> A) {
    std::sort(A.begin(),A.end());
    return makeSchedule(A);
}

/// TESTS ///
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(TP1_Ex6, activityScheduling) {
    std::vector<Activity> A = {{10,20}, {30, 35}, {5, 15}, {10, 40}, {40, 50}};
    std::vector<Activity> V = earliestFinishScheduling(A);
    EXPECT_EQ(V.size(), 3 );
    ASSERT_THAT(earliestFinishScheduling(A),  ::testing::ElementsAre(Activity(5, 15), Activity(30, 35), Activity(40, 50)));
}