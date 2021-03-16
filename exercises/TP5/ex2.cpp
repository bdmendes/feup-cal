#include "Graph.h"
#include "Person.h"

// Complete the functions on the Graph.h file

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP5_Ex2a, test_dfs) {
    Graph<Person> net1;
    createNetwork(net1);
    std::vector<Person> v1 = net1.dfs();
    std::string names[] = {"Ana", "Carlos", "Maria", "Rui", "Filipe", "Vasco", "Ines"};
    for (unsigned i = 0; i < 7; i++)
        if (i < v1.size())
            EXPECT_EQ(names[i], v1[i].getName());
        else
            EXPECT_EQ(names[i], "(null)");
}

TEST(TP5_Ex2b, test_bfs) {
    Graph<Person> net1;
    createNetwork(net1);
    std::vector<Person> v1 = net1.bfs(Person("Ana",19));
    std::string names[] = {"Ana", "Carlos", "Filipe", "Ines", "Maria", "Rui", "Vasco"};
    for (unsigned i = 0; i < 7; i++)
        if (i < v1.size())
            EXPECT_EQ(names[i], v1[i].getName());
        else
            EXPECT_EQ(names[i], "(null)");
}

TEST(TP5_Ex2c, test_topsort) {
    Graph<int> myGraph;
    myGraph.addVertex(1); myGraph.addVertex(2); myGraph.addVertex(3); myGraph.addVertex(4);
    myGraph.addVertex(5); myGraph.addVertex(6); myGraph.addVertex(7);
    myGraph.addEdge(1, 2, 0);
    myGraph.addEdge(1, 4, 0);
    myGraph.addEdge(1, 3, 0);
    myGraph.addEdge(2, 5, 0);
    myGraph.addEdge(2, 4, 0);
    myGraph.addEdge(3, 6, 0);
    myGraph.addEdge(4, 3, 0);
    myGraph.addEdge(4, 6, 0);
    myGraph.addEdge(4, 7, 0);
    myGraph.addEdge(5, 4, 0);
    myGraph.addEdge(5, 7, 0);
    myGraph.addEdge(7, 6, 0);

    std::vector<int> topOrder;

    topOrder = myGraph.topsort();
    std::stringstream ss;
    for( unsigned int i = 0; i < topOrder.size(); i++)
        ss << topOrder[i] << " ";
    EXPECT_EQ("1 2 5 4 3 7 6 ", ss.str());

    // to test including a cycle in the graph
    myGraph.addEdge(3, 1, 0);

    topOrder = myGraph.topsort();
    ss.str("");
    for( unsigned int i = 0; i < topOrder.size(); i++)
        ss << topOrder[i] << " ";
    EXPECT_EQ("", ss.str());
}