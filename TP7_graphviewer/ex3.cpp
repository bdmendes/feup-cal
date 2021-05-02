#include <fstream>
#include <stdexcept>
#include <string>
#include <sstream>
#include "graphviewer.h"

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

static bool isAllDigits(const std::string& line){
    return std::all_of(line.begin(), line.end(), [](char c){
        return isdigit(c);
    });
}

void readNodesFromFile(GraphViewer& gv, const std::string& path){
    std::ifstream file(path);
    if (!file) throw std::invalid_argument("Invalid path");
    for (std::string line; std::getline(file, line); ){
        if (isAllDigits(line)) continue;
        int id;
        float x,y;
        std::stringstream ss(line);
        ss >> id >> x >> y;
        gv.addNode(id, sf::Vector2<float>(x, y));
    }
}

void readEdgesFromFile(GraphViewer& gv, const std::string& path){
    std::ifstream file(path);
    if (!file) throw std::invalid_argument("Invalid path");
    for (std::string line; std::getline(file, line); ){
        if (isAllDigits(line)) continue;
        int id;
        int sourceID, targetID;
        std::stringstream ss(line);
        ss >> id >> sourceID >> targetID;
        auto sourceNode = gv.getNode(sourceID);
        auto targetNode = gv.getNode(targetID);
        gv.addEdge(id, targetNode, sourceNode);
    }
}

void ex3() {
    GraphViewer gv;
    gv.setCenter(sf::Vector2f(300, 300));
    gv.createWindow(600, 600);
    readNodesFromFile(gv, "resources/map1/nodes.txt");
    readEdgesFromFile(gv, "resources/map1/edges.txt");
    gv.join();
}

