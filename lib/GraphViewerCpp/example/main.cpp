#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>

#include "graphviewer.h"

#ifndef WIN32
    #include <X11/Xlib.h>
#endif

GraphViewer* drawGraphFromFile(std::string name);

int main() {
    #ifndef WIN32
        XInitThreads();
    #endif
    GraphViewer *gv1 = drawGraphFromFile("random");
    GraphViewer *gv2 = drawGraphFromFile("star");
    GraphViewer *gv3 = drawGraphFromFile("rainbow");
    gv1->join();
    gv2->join();
    gv3->join();
    delete gv1;
    delete gv2;
    delete gv3;
    
    return 0;
}

std::string getPathFromFilename(const std::string &filename){
    const size_t last_slash_idx = std::min(filename.rfind('\\'), filename.rfind('/'));
    if(last_slash_idx == std::string::npos){
        throw std::invalid_argument("filename is not valid");
    }
    std::string directory = filename.substr(0, last_slash_idx);
    return directory;
}

sf::Color colorStringToSFColor(std::string colorStr){
    std::transform(colorStr.begin(), colorStr.end(),colorStr.begin(), ::toupper);
    if(colorStr == "BLUE"       ) return sf::Color::Blue;
    if(colorStr == "RED"        ) return sf::Color::Red;
    if(colorStr == "PINK"       ) return sf::Color(255, 192, 203);
    if(colorStr == "PURPLE"     ) return sf::Color(128, 0, 128);
    if(colorStr == "BLACK"      ) return sf::Color::Black;
    if(colorStr == "WHITE"      ) return sf::Color::White;
    if(colorStr == "ORANGE"     ) return sf::Color(255, 129,   0);
    if(colorStr == "YELLOW"     ) return sf::Color::Yellow;
    if(colorStr == "GREEN"      ) return sf::Color::Green;
    if(colorStr == "CYAN"       ) return sf::Color::Cyan;
    if(colorStr == "GRAY"       ) return sf::Color(128, 128, 128);
    if(colorStr == "DARK_GRAY"  ) return sf::Color(192, 192, 192);
    if(colorStr == "LIGHT_GRAY" ) return sf::Color( 64,  64,  64);
    if(colorStr == "MAGENTA"    ) return sf::Color::Magenta;
    throw std::domain_error("No such color '"+colorStr+"'");
}

GraphViewer* drawGraphFromFile(std::string name){
    std::ifstream nodes(getPathFromFilename(__FILE__)+"/resources/graphs/"+name+"/nodes.txt");
    std::ifstream edges(getPathFromFilename(__FILE__)+"/resources/graphs/"+name+"/edges.txt");
    std::ifstream window(getPathFromFilename(__FILE__)+"/resources/graphs/"+name+"/window.txt");
    std::string line, background_path;
    std::istringstream iss;
    unsigned int n_nodes, n_edges;
    unsigned int height, width;
    unsigned int v1, v2;
    unsigned int type, scale, dynamic, thickness, size, dashed, curved;
    float x, y;
    char color[20], label[256], icon_path[256], flow[256], weight[256];

    window >> width >> height >> dynamic >> scale >> dashed >> curved >> background_path;
    GraphViewer *gv = new GraphViewer();
    if (background_path[0] != '-')
        gv->setBackground(getPathFromFilename(__FILE__)+"/"+background_path);

    // read num of nodes
    std::getline(nodes, line);
    iss.str(line);
    iss >> n_nodes;

    // draw nodes
    for(int i = 0; i < n_nodes;i++) {
        std::getline(nodes, line);
        sscanf(
            line.c_str(),
            "(%f, %f, %s , %s , %u, %s )",
            &x, &y, color, label, &size, icon_path
        );
        GraphViewer::Node &node = gv->addNode(
                i,
                sf::Vector2f(x,y)*float(scale)
        );
        gv->lock();
        node.setColor(colorStringToSFColor(color));
        if (label[0] != '-')
            node.setLabel(label);
        if (icon_path[0] != '-')
            node.setIcon(getPathFromFilename(__FILE__)+"/"+icon_path);
        node.setSize(size);
        gv->unlock();
    }

    // read num of edges
    std::getline(edges, line);
    sscanf(line.c_str(), "%d", &n_edges);

    //draw edges
    for(int i = 0; i < n_edges ; i++) {
        std::getline(edges, line);
        sscanf(
            line.c_str(),
            "(%u, %u, %u, %s ,%u, %s , %s , %s )", 
            &v1, &v2, &type, color, &thickness, label, flow, weight
        );
        GraphViewer::Edge &edge = gv->addEdge(
                i,
                gv->getNode(v1),
                gv->getNode(v2),
                (type ?
                    GraphViewer::Edge::EdgeType::DIRECTED :
                    GraphViewer::Edge::EdgeType::UNDIRECTED
                )
        );
        gv->lock();
        edge.setColor(colorStringToSFColor(color));
        edge.setThickness(thickness);
        if (label[0] != '-')
            edge.setLabel(label);
        if (flow[0] != '%')
            edge.setFlow(atof(flow));
        if (weight[0] != '%')
            edge.setWeight(atof(weight));
        edge.setDashed(dashed);
        gv->unlock();
    }
    gv->setZipEdges(true);

    gv->createWindow(width, height);

    return gv;
}
