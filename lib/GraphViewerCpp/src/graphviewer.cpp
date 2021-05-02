#include "graphviewer.h"

#include <cmath>
#include <iostream>

using namespace std;
using namespace sf;

std::mutex GraphViewer::createWindowMutex;

const GraphViewer::Color GraphViewer::BLACK   = sf::Color::Black;
const GraphViewer::Color GraphViewer::WHITE   = sf::Color::White;
const GraphViewer::Color GraphViewer::RED     = sf::Color::Red;
const GraphViewer::Color GraphViewer::GREEN   = sf::Color::Green;
const GraphViewer::Color GraphViewer::BLUE    = sf::Color::Blue;
const GraphViewer::Color GraphViewer::YELLOW  = sf::Color::Yellow;
const GraphViewer::Color GraphViewer::MAGENTA = sf::Color::Magenta;
const GraphViewer::Color GraphViewer::CYAN    = sf::Color::Cyan;
const GraphViewer::Color GraphViewer::PINK  (255, 192, 203);
const GraphViewer::Color GraphViewer::ORANGE(255, 128,   0);
const GraphViewer::Color GraphViewer::GRAY  (128, 128, 128);
const GraphViewer::Color GraphViewer::LIGHT_GRAY(192, 192, 192);
const GraphViewer::Color GraphViewer::DARK_GRAY(64, 64, 64);

void GraphViewer::ZipEdges::append(const VertexArray &a){
    for(size_t i = 0; i < a.getVertexCount(); ++i){
        vertices.push_back(a[i]);
    }
}
const vector<Vertex>& GraphViewer::ZipEdges::getVertices() const{ return vertices; }

const int DEFAULT_WIDTH  = 800;
const int DEFAULT_HEIGHT = 600;

string getPath(const string &filename){
    const size_t last_slash_idx = min(filename.rfind('\\'), filename.rfind('/'));
    if(last_slash_idx == string::npos){
        throw invalid_argument("filename is not valid");
    }
    string directory = filename.substr(0, last_slash_idx);
    return directory;
}

Font getFont(const string &path){
    Font font;
    string fontPath = getPath(__FILE__)+path;
    if(!font.loadFromFile(fontPath))
        throw runtime_error("Failed to load font from file; check if font exists");
    return font;
}
const Font GraphViewer::DEBUG_FONT = getFont("/../resources/fonts/inconsolata.ttf");
const Font GraphViewer::FONT       = getFont("/../resources/fonts/arial.ttf");

GraphViewer::GraphViewer():
    debug_text("", DEBUG_FONT, DEBUG_FONT_SIZE)
{
    debug_text.setFillColor(Color::Black);
    debug_text.setStyle(Text::Bold);
}

void GraphViewer::createWindow(unsigned int width, unsigned int height){
    if(window != nullptr) throw runtime_error("Window was already created");
    if(width  == 0) width  = DEFAULT_WIDTH ;
    if(height == 0) height = DEFAULT_HEIGHT;
    this->width  = width;
    this->height = height;

    windowOpen = true;
    main_thread = new thread(&GraphViewer::run, this);
}

void GraphViewer::closeWindow(){
    window->close();
    delete window    ; window     = nullptr;
    delete view      ; view       = nullptr;
    delete debug_view; debug_view = nullptr;
}

void GraphViewer::setCenter(const sf::Vector2f &center){
    this->center = center;
    if(isWindowOpen()){
        lock_guard<mutex> lock(graphMutex);
        recalculateView();
    }
}

void GraphViewer::setScale(double scale){
    this->scale = scale;
    if(isWindowOpen()){
        lock_guard<mutex> lock(graphMutex);
        recalculateView();
    }
}

GraphViewer::Node& GraphViewer::addNode(id_t id, const sf::Vector2f &position){
    lock_guard<mutex> lock(graphMutex);
    if(nodes.count(id))
        throw invalid_argument("A node with that ID already exists");
    return *(nodes[id] = new Node(id, position));
}

GraphViewer::Node& GraphViewer::getNode(GraphViewer::id_t id){
    return *nodes.at(id);
}

vector<GraphViewer::Node *> GraphViewer::getNodes() {
    vector<Node*> ret;
    ret.reserve(nodes.size());
    for(auto &p: nodes){
        ret.push_back(p.second);
    }
    return ret;
}

void GraphViewer::removeNode(GraphViewer::id_t id){
    lock_guard<mutex> lock(graphMutex);
    Node *node = nodes.at(id);
    while(!node->edges.empty()){
        Edge *edge = *node->edges.begin();
        removeEdge_noLock(edge->getId());
    }
    delete node;
    nodes.erase(id);
}

GraphViewer::Edge& GraphViewer::addEdge(id_t id, Node &u, Node &v, Edge::EdgeType edge_type){
    lock_guard<mutex> lock(graphMutex);
    if(edges.count(id))
        throw invalid_argument("An edge with that ID already exists");
    Edge &ret = *(edges[id] = new Edge(id, u, v, edge_type));
    if(zipEdges) updateZip();
    return ret;
}

GraphViewer::Edge &GraphViewer::getEdge(GraphViewer::id_t id) {
    return *edges.at(id);
}

vector<GraphViewer::Edge *> GraphViewer::getEdges() {
    vector<Edge*> ret;
    ret.reserve(edges.size());
    for(auto &p: edges){
        ret.push_back(p.second);
    }
    return ret;
}

void GraphViewer::removeEdge(GraphViewer::id_t id){
    lock_guard<mutex> lock(graphMutex);
    removeEdge_noLock(id);
}

void GraphViewer::removeEdge_noLock(GraphViewer::id_t id){
    Edge *edge = edges.at(id);
    edge->u->edges.erase(edge->u->edges.find(edge));
    edge->v->edges.erase(edge->v->edges.find(edge));
    delete edge;
    edges.erase(id);
    if(zipEdges) updateZip();
}

void GraphViewer::setBackground(const string &path, const sf::Vector2f &position, const sf::Vector2f &scale, double alpha){
    lock_guard<mutex> lock(graphMutex);
    background_texture.loadFromFile(path);
    background_sprite.setTexture(background_texture);
    background_sprite.setPosition(position);
    background_sprite.setScale(scale);
    background_sprite.setColor(sf::Color(255, 255, 255, alpha*255.0));
}

void GraphViewer::clearBackground(){
    lock_guard<mutex> lock(graphMutex);
    background_texture = Texture();
    background_sprite.setTexture(background_texture);
}

void GraphViewer::join(){
    main_thread->join();
}

void GraphViewer::setEnabledNodes(bool b){ enabledNodes = b; }
void GraphViewer::setEnabledEdges(bool b){ enabledEdges = b; }
void GraphViewer::setEnabledNodesText(bool b){ enabledNodesText = b; }
void GraphViewer::setEnabledEdgesText(bool b){ enabledEdgesText = b; }

void GraphViewer::setZipEdges(bool b){
    zipEdges = b;
    if(zipEdges) updateZip();
}

void GraphViewer::lock  (){ graphMutex.lock  (); }
void GraphViewer::unlock(){ graphMutex.unlock(); }

void GraphViewer::updateZip(){
    lock_guard<mutex> lock(graphMutex);
    zip = ZipEdges();
    for(const auto &p: edges)
        zip.append(*p.second->getShape());
}

void GraphViewer::run(){
    ContextSettings settings;
    settings.antialiasingLevel = 8;
    GraphViewer::createWindowMutex.lock();
    window = new RenderWindow(VideoMode(this->width, this->height), "GraphViewer", Style::Default, settings);
    GraphViewer::createWindowMutex.unlock();

    view = new View(window->getDefaultView());
    debug_view = new View(window->getDefaultView());

    bool isLeftClickPressed = false;
    Vector2f centerInitial;
    Vector2f posMouseInitial;

    recalculateView();

    while (window->isOpen()){
        Event event;
        while (window->pollEvent(event)){
            switch(event.type){
                case Event::Closed            : window->close(); break;
                case Event::Resized           : onResize(); break;
                case Event::MouseWheelScrolled: onScroll(event.mouseWheelScroll.delta); break;
                case Event::MouseButtonPressed:
                    switch(event.mouseButton.button){
                        case Mouse::Button::Left:
                            isLeftClickPressed = true;
                            centerInitial = center;
                            posMouseInitial = Vector2f(
                                event.mouseButton.x,
                                event.mouseButton.y
                            );
                            break;
                        default: break;
                    }
                    break;
                case Event::MouseButtonReleased:
                    switch(event.mouseButton.button){
                        case Mouse::Button::Left:
                            isLeftClickPressed = false;
                            break;
                        default: break;
                    }
                    break;
                case Event::MouseMoved:
                    if(isLeftClickPressed){
                        Vector2f mouse_pos(event.mouseMove.x, event.mouseMove.y);
                        center = centerInitial - (mouse_pos - posMouseInitial)*scale;
                        recalculateView();
                    }
                    break;
                case Event::TextEntered:
                    switch(toupper(event.text.unicode)){
                        case 'D': debug_mode = !debug_mode; break;
                        default: break;
                    }
                    break;
                default: break;
            }
        }
        draw();
        window->display();
    }

    {
        lock_guard<mutex> lock(graphMutex);
        windowOpen = false;
    }
}

void GraphViewer::draw() {
    lock_guard<mutex> lock(graphMutex);
    window->clear(Color::White);

    window->setView(*view);
    window->draw(background_sprite);
    if(enabledEdges){
        if(zipEdges){
            const vector<Vertex> &v = zip.getVertices();
            window->draw(&v[0], v.size(), Quads);
        } else {
            for(const auto &edgeIt: edges){
                const Edge &edge = *edgeIt.second;
                window->draw(*edge.getShape());
            }
        }
    }
    if(enabledNodes){
        for(const auto &nodeIt: nodes){
            const Node &node = *nodeIt.second;
            window->draw(*node.getShape());
        }
    }
    if(enabledEdges && enabledEdgesText){
        for(const auto &edgeIt: edges){
            const Edge &edge = *edgeIt.second;
            if(edge.getText().getString() != "")
                window->draw(edge.getText());
        }
    }
    if(enabledNodes && enabledNodesText){
        for(const auto &nodeIt: nodes){
            const Node &node = *nodeIt.second;
            if(node.getText().getString() != "")
                window->draw(node.getText());
        }
    }

    fps_monitor.count();

    if(debug_mode){
        drawDebug();
    }
}

void GraphViewer::drawDebug(){
    window->setView(*debug_view);
    
    string debugInfo;
    debugInfo += "FPS: " + to_string(int(fps_monitor.getFPS())) + "\n";
    
    if(debugInfo[debugInfo.size()-1] == '\n')
        debugInfo = debugInfo.substr(0, debugInfo.size()-1);
    debug_text.setString(debugInfo);
    Vector2f size = Vector2f(window->getSize());
    FloatRect bounds = debug_text.getLocalBounds();
    debug_text.setOrigin(0, bounds.height);
    debug_text.setPosition(Vector2f(0.2*DEBUG_FONT_SIZE, size.y-0.7*DEBUG_FONT_SIZE));

    window->draw(debug_text);
}

void GraphViewer::onResize(){
    recalculateView();
}

void GraphViewer::onScroll(float delta){
    scale *= pow(SCALE_DELTA, -delta);
    recalculateView();
}

void GraphViewer::recalculateView(){
    Vector2f size = static_cast<Vector2f>(window->getSize());
    *view = View(center, size*scale);
    *debug_view = View(FloatRect(0.0, 0.0, size.x, size.y));

    auto bounds = background_sprite.getLocalBounds();
    Vector2f scaleVec(scale*size.x/bounds.width, scale*size.y/bounds.height);
}

bool GraphViewer::isWindowOpen() {
    lock_guard<mutex> lock(graphMutex);
    return windowOpen;
}
