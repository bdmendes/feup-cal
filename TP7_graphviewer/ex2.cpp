#include "graphviewer.h"
#include <unistd.h>

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

void addNetwork2(GraphViewer& gv){
    /* Create nodes */
    Node &node0 = gv.addNode(0, sf::Vector2f(300, 50));
    Node &node1 = gv.addNode(1, sf::Vector2f(318, 58));
    Node &node2 = gv.addNode(2, sf::Vector2f(325, 75));
    Node &node3 = gv.addNode(3, sf::Vector2f(318, 93));
    Node &node4 = gv.addNode(4, sf::Vector2f(300, 100));
    Node &node5 = gv.addNode(5, sf::Vector2f(282, 93));
    Node &node6 = gv.addNode(6, sf::Vector2f(275, 75));
    Node &node7 = gv.addNode(7, sf::Vector2f(282, 58));
    Node &node8 = gv.addNode(8, sf::Vector2f(150, 200));
    Node &node9 = gv.addNode(9, sf::Vector2f(300, 200));
    Node &node10 = gv.addNode(10, sf::Vector2f(450, 200));
    Node &node11 = gv.addNode(11, sf::Vector2f(300, 400));
    Node &node12 = gv.addNode(12, sf::Vector2f(200, 550));
    Node &node13 = gv.addNode(13, sf::Vector2f(400, 500));

    /* Create edges */
    Edge& edge0 = gv.addEdge(0, node1, node0);
    Edge& edge1 = gv.addEdge(1, node2, node1);
    Edge& edge2 = gv.addEdge(2, node3, node2);
    Edge& edge3 = gv.addEdge(3, node4, node3);
    Edge& edge4 = gv.addEdge(4, node5, node4);
    Edge& edge5 = gv.addEdge(5, node6, node5);
    Edge& edge6 = gv.addEdge(6, node7, node6);
    Edge& edge7 = gv.addEdge(7, node0, node7);
    Edge& edge8 = gv.addEdge(8, node9, node4);
    Edge& edge9 = gv.addEdge(9, node8, node9);
    Edge& edge10 = gv.addEdge(10, node10, node9);
    Edge& edge11 = gv.addEdge(11, node11, node9);
    Edge& edge12 = gv.addEdge(12, node12, node11);
    Edge& edge13 = gv.addEdge(13, node13, node11);
}

void animate(GraphViewer& gv){
    for (int i = 0; i < 3; i++){
        sleep(1);

        Node &node12 = gv.getNode(12);// Get reference to node
        auto node12Position = node12.getPosition();
        gv.lock();
        node12.setPosition(sf::Vector2f(250, 550));// Set position
        gv.unlock();

        Node &node13 = gv.getNode(13);// Get reference to node
        auto node13Position = node13.getPosition();
        gv.lock();
        node13.setPosition(sf::Vector2f(350, 550));// Set position
        gv.unlock();

        sleep(1);

        gv.lock();
        node12.setPosition(node12Position);
        node13.setPosition(node13Position);
        gv.unlock();
    }
}

void ex2() {
    GraphViewer gv;
    gv.setCenter(sf::Vector2f(300, 300));
    gv.createWindow(600, 600);
    addNetwork2(gv);
    animate(gv);
    gv.join();
}

