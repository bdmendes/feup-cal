#ifndef TEST_AUX_H_
#define TEST_AUX_H_

#include <gtest/gtest.h>

#include <sstream>
#include <random>
#include <time.h>
#include <chrono>
#include "Graph.h"

/**
 * Auxiliary functions to tests...
 *
 */
Graph<int> CreateTestGraph();

void generateRandomGridGraph(int n, Graph<std::pair<int,int>> & g);

#endif //TEST_AUX_H_
