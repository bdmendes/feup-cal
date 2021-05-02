#include "graphviewer.h"

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

void addNetwork(GraphViewer& gv){
    /* Create blue vertex with ID 0 at (200,300) */
    Node &node0 = gv.addNode(0, sf::Vector2f(200, 300));
    node0.setColor(GraphViewer::BLUE);

    /* Create red vertex with ID 1 at (400, 300) */
    Node &node1 = gv.addNode(1, sf::Vector2f(400, 300));

    /* Create black edge from node 0 to node 1 */
    Edge &edge0 = gv.addEdge(0,node1,node0,GraphViewer::Edge::EdgeType::DIRECTED);

    /* Create green vertex */
    Node &node2 = gv.addNode(2, sf::Vector2f(300, 400));
    node1.setColor(GraphViewer::GREEN);

    /* Create labeled orange edge from node 0 to node 2 */
    Edge &edge1 = gv.addEdge(1,node2,node0,GraphViewer::Edge::EdgeType::DIRECTED);
    edge1.setColor(GraphViewer::ORANGE);
    edge1.setLabel("Long road");
}

void ex1() {
    GraphViewer gv;
    gv.setCenter(sf::Vector2f(300, 300));
    gv.createWindow(600, 600);
    addNetwork(gv);
    gv.join();
}
