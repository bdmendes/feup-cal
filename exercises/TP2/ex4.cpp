#include "exercises.h"
#include <algorithm>

bool Activity::operator==(const Activity &a2) const {
    return start == a2.start && finish == a2.finish;
}

bool Activity::overlaps(const Activity &a2) const {
    return (start < a2.finish) && (a2.start < finish);
}

void activitySelectionRec(const std::vector<Activity>& activities,
                          std::vector<Activity>& curChosen, std::vector<Activity>& bestChosen, int curIndex, unsigned& curMax){
    // Base case
    if (curIndex == activities.size()){
        if (curChosen.size() > curMax){
            curMax = curChosen.size();
            bestChosen = curChosen;
        }
        return;
    }

    auto curActivity = activities.at(curIndex);
    auto mayPut = !std::any_of(curChosen.begin(), curChosen.end(), [curActivity](const Activity& a){
        return a.overlaps(curActivity);
    });

    // Try to add activity
    if (mayPut){
        curChosen.push_back(curActivity);
        activitySelectionRec(activities, curChosen, bestChosen, curIndex + 1, curMax);
        curChosen.pop_back(); // undo for skip call
    }

    // Try to discard activity
    activitySelectionRec(activities, curChosen, bestChosen, curIndex + 1, curMax);
}

std::vector<Activity> activitySelectionBacktracking(std::vector<Activity> A) {
    std::vector<Activity> curChosen, bestChosen;
    unsigned curMax = 0;
    activitySelectionRec(A, curChosen, bestChosen, 0, curMax);
    return bestChosen;
}

/// TESTS ///
#include <gtest/gtest.h>
#include <gmock/gmock.h>

bool noOverlaps(const std::vector<Activity> &acts) {
    for(unsigned int i = 0; i < acts.size(); i++) {
        Activity A1 = acts.at(i);
        for(unsigned int j = i + 1; j < acts.size(); j++) {
            Activity A2 = acts.at(j);
            if(A1.overlaps(A2)) return false;
        }
    }
    return true;
}

TEST(TP2_Ex4, activityScheduling) {
    std::vector<Activity> A = {{10,20}, {30, 35}, {5, 15}, {10, 40}, {40, 50}};
    std::vector<Activity> V = activitySelectionBacktracking(A);
    EXPECT_EQ(V.size(), 3 );
    EXPECT_EQ(noOverlaps(V), true);
}