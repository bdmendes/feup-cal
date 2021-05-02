#ifndef _GRAPH_VIEWER_H_
#define _GRAPH_VIEWER_H_

#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <set>

#include "fpsmonitor.h"

#include <SFML/Graphics.hpp>

/**
 * @brief Class to save and represent a graph.
 */
class GraphViewer {
public:
    typedef int64_t id_t;

    typedef sf::Color Color;
    static const Color BLACK     ;
    static const Color WHITE     ;
    static const Color RED       ;
    static const Color GREEN     ;
    static const Color BLUE      ;
    static const Color YELLOW    ;
    static const Color MAGENTA   ;
    static const Color CYAN      ;
    static const Color PINK      ;
    static const Color ORANGE    ;
    static const Color GRAY      ;
    static const Color LIGHT_GRAY;
    static const Color DARK_GRAY ;

private:
    class LineShape;
    class FullLineShape;
    class DashedLineShape;
    class ArrowHead;

public:
    class Edge;

    /**
     * @brief Class to represent a node.
     */
    class Node {
        friend Edge;
        friend GraphViewer;
    private:
        id_t id;                                    ///< @brief Node ID.
        sf::Vector2f position;                      ///< @brief Node position.
        float size = 10.0;                          ///< @brief Node size.
        std::string label = "";                     ///< @brief Node label.
        sf::Color color = sf::Color::Red;           ///< @brief Node color.
        sf::Texture icon;                           ///< @brief Node icon.
        bool isIcon = false;                        ///< @brief True if node is icon, false otherwise.
        float outlineThickness = 1.0;               ///< @brief Node outline thickness.
        sf::Color outlineColor = sf::Color::Black;  ///< @brief Node outline color.
        sf::Shape *shape = nullptr;                 ///< @brief Node shape.
        sf::Text text;                              ///< @brief Node text.
        
        std::set<Edge*> edges;

        /**
         * @brief Update node shape and text considering changes in properties.
         */
        void update();

    private:
        /**
         * @brief Construct a new Node object
         */
        Node();
        /**
         * @brief Construct a new Node object with ID and position
         * 
         * @param id        Unique node ID
         * @param position  Node position in the window, in pixels
         */
        Node(id_t id, const sf::Vector2f &position);
    
    public:
        /**
         * @brief Get node ID.
         * 
         * @return id_t     Node ID
         */
        id_t getId() const;
        
        /**
         * @brief Set node position.
         * 
         * @param position  Position, in pixels
         */
        void setPosition(const sf::Vector2f &position);
        /**
         * @brief Get node position;
         * 
         * @return const sf::Vector2f&  Position, in pixels
         */
        const sf::Vector2f& getPosition() const;
        
        /**
         * @brief Set node size.
         * 
         * In the case it is not an icon, it is the diameter of the node;
         * if it is an icon, it is the width of the node.
         * 
         * @param size  Node size, in pixels
         */
        void setSize(float size = 10.0);
        /**
         * @brief Get node size.
         * 
         * @return int Node size, in pixels
         */
        float getSize() const;
        
        /**
         * @brief Set node label.
         * 
         * @param label     Node label
         */
        void setLabel(const std::string &label = "");
        /**
         * @brief Get node label.
         * 
         * @return std::string  Node label
         */
        std::string getLabel() const;
        
        /**
         * @brief Set node color.
         * 
         * @param color Node color
         */
        void setColor(const sf::Color &color = sf::Color::Red);
        /**
         * @brief Get node color.
         * 
         * @return const sf::Color&     Node color
         */
        const sf::Color& getColor() const;
        
        /**
         * @brief Set node icon.
         * 
         * @param path  Path of file to be used as icon
         */
        void setIcon(const std::string &path);
        /**
         * @brief Get node icon texture.
         * 
         * @return const sf::Texture& Icon texture
         */
        const sf::Texture& getIcon() const;
        /**
         * @brief Check if node is an icon.
         * 
         * @return true     When it is an icon
         * @return false    Otherwise
         */
        bool getIsIcon() const;

        /**
         * @brief Set node outline thickness.
         * 
         * @param outlineThickness  Outline thickness, in pixels
         */
        void setOutlineThickness(float outlineThickness = 1.0);
        /**
         * @brief Get node outline thickness.
         * 
         * @return float    Outline thickness, in pixels
         */
        float getOutlineThickness() const;
        
        /**
         * @brief Set node outline color.
         * 
         * @param outlineColor  Outline color
         */
        void setOutlineColor(const sf::Color &outlineColor = sf::Color::Black);
        /**
         * @brief Get outline color.
         * 
         * @return const sf::Color&     Outline color
         */
        const sf::Color& getOutlineColor() const;
        
        /**
         * @brief Get shape that represents the node.
         * 
         * @return const sf::Shape*     Pointer to node shape
         */
        const sf::Shape* getShape() const;
        
        /**
         * @brief Get node text.
         * 
         * @return const sf::Text&  Node text 
         */
        const sf::Text& getText() const;
    };

    /**
     * @brief Class to represent an edge.
     */
    class Edge {
        friend Node;
        friend GraphViewer;
    public:
        /**
         * @brief Edge type.
         */
        enum EdgeType {
            DIRECTED,       ///< @brief Directed edge
            UNDIRECTED      ///< @brief Undirected edge
        };
    private:
        id_t id;                            ///< @brief Edge ID.
        Node *u = nullptr;                  ///< @brief Edge origin node.
        Node *v = nullptr;                  ///< @brief Edge destination node.
        EdgeType edge_type;                 ///< @brief Edge type.
        std::string label = "";             ///< @brief Edge label.
        sf::Color color = sf::Color::Black; ///< @brief Edge color.
        bool dashed = false;                ///< @brief True if edge is dashed, false if full.
        float thickness = 5.0;              ///< @brief Edge thickness, in pixels.
        float *weight = nullptr;            ///< @brief Edge weight.
        float *flow = nullptr;              ///< @brief Edge flow.
        LineShape *shape = nullptr;         ///< @brief Edge shape.
        sf::Text text;                      ///< @brief Edge text.

        /**
         * @brief Update edge shape and text considering changes in properties.
         */
        void update();

    private:
        /**
         * @brief Construct a new Edge object
         */
        Edge();
        /**
         * @brief Construct a new Edge object with ID, origin/destination nodes
         *        and direction.
         * 
         * @param id            Unique edge ID
         * @param u             Pointer to origin node
         * @param v             Pointer to destination node
         * @param edge_type     Edge type (directed or undirected)
         */
        Edge(id_t id, Node &u, Node &v, EdgeType edge_type = UNDIRECTED);
        
    public:
        /**
         * @brief Get edge ID
         * 
         * @return id_t     Edge ID
         */
        id_t getId() const;

        /**
         * @brief Set origin node.
         * 
         * @param u     Pointer to origin node.
         */
        void setFrom(Node *u);
        /**
         * @brief Get origin node.
         * 
         * @return const Node*  Pointer to origin node.
         */
        const Node* getFrom() const;

        /**
         * @brief Set destination node.
         * 
         * @param v     Pointer to destination node.
         */
        void setTo(Node *v);
        /**
         * @brief Get destination node.
         * 
         * @return const Node*  Pointer to destination node.
         */
        const Node* getTo() const;

        /**
         * @brief Set edge type.
         * 
         * @param edge_type     Edge type
         */
        void setEdgeType(EdgeType edge_type = UNDIRECTED);
        /**
         * @brief Get edge type.
         * 
         * @return EdgeType     Edge type
         */
        EdgeType getEdgeType() const;

        /**
         * @brief Set edge label.
         * 
         * @param label     Edge label.
         */
        void setLabel(const std::string &label = "");
        /**
         * @brief Get edge label.
         * 
         * @return const std::string&   Edge label.
         */
        const std::string& getLabel() const;

        /**
         * @brief Set edge color.
         * 
         * @param color     Edge color.
         */
        void setColor(const sf::Color &color = sf::Color::Black);
        /**
         * @brief Get edge color.
         * 
         * @return const sf::Color&     Edge color.
         */
        const sf::Color& getColor() const;

        /**
         * @brief Set if the edge is dashed or not.
         * 
         * @param dashed    True if edge is to be drawn dashed, false if full
         */
        void setDashed(bool dashed = false);
        /**
         * @brief Get if edge is dashed.
         * 
         * @return true     If edge is dashed
         * @return false    If edge is full
         */
        bool getDashed() const;

        /**
         * @brief Set edge thickness.
         * 
         * @param thickness     Edge thickness, in pixels
         */
        void setThickness(float thickness = 5.0);
        /**
         * @brief Get edge thickness.
         * 
         * @return float        Edge thickness, in pixels
         */
        float getThickness() const;

        /**
         * @brief Set edge weight.
         * 
         * Edge weight is shown in the edge label.
         * 
         * @param weight    Edge weight
         */
        void setWeight(float weight);
        /**
         * @brief Get pointer to edge weight.
         * 
         * @return const float*     Pointer to edge weight, or nullptr if none
         *                          was set
         */
        const float* getWeight() const;

        /**
         * @brief Set edge flow.
         * 
         * Edge flow is shown in the edge label.
         * 
         * @param flow  Edge flow
         */
        void setFlow(float flow);
        /**
         * @brief Get pointer to edge flow.
         * 
         * @return const float*     Pointer to edge flow, or nullptr if none
         *                          was set
         */
        const float* getFlow() const;

        /**
         * @brief Get shape that represents the edge.
         * 
         * @return const sf::VertexArray*   Pointer to edge shape
         */
        const sf::VertexArray* getShape() const;

        /**
         * @brief Get edge text (includes label).
         * 
         * @return const sf::Text&  Edge text
         */
        const sf::Text& getText() const;
    };
    
public:
    /**
     * @brief Construct a new graph.
     */
    GraphViewer();

    /**
     * @brief Create the visualization window.
     *
     * @param width Window width (in pixels)
     * @param height Window height (in pixels)
     */
    void createWindow(unsigned int width = 800, unsigned int height = 600);

    bool isWindowOpen();

    /**
     * @brief Close visualization window.
     */
    void closeWindow();

    void setCenter(const sf::Vector2f &center);

    void setScale(double scale);

    /**
     * @brief Add node.
     * 
     * @param node Node to be added
     */
    Node& addNode(id_t id, const sf::Vector2f &position);

    /**
     * @brief Get node from ID.
     * 
     * @param id        ID of node
     * @return Node&    Node with that ID.
     * 
     * @throws std::out_of_range    If node with that ID does not exist.
     */
    Node& getNode(id_t id);

    /**
     * @brief Get all nodes.
     *
     * @return          Vector with pointers to all nodes
     */
    std::vector<GraphViewer::Node *> getNodes();

    /**
     * @brief Remove node and all edges connected to it.
     *
     * @param id Unique ID of node to be removed.
     */
    void removeNode(id_t id);

    /**
     * @brief Add edge.
     *
     * @param id Unique edge ID
     * @param v1 Unique ID of origin node
     * @param v2 Unique ID of destination node
     * @param edge_type EdgeType.DIRECTED if the edge is directed,
     *                  EdgeType.UNDIRECTED if the edge is undirected
     */
    Edge& addEdge(id_t id, Node &u, Node &v, Edge::EdgeType edge_type = Edge::EdgeType::UNDIRECTED);

    /**
     * @brief Get edge from ID.
     *
     * @param id        ID of edge
     * @return Edge&    Edge with that ID.
     *
     * @throws std::out_of_range    If edge with that ID does not exist.
     */
    Edge& getEdge(id_t id);

    /**
     * @brief Get all edges.
     *
     * @return          Vector with pointers to all edges
     */
    std::vector<GraphViewer::Edge *> getEdges();

    /**
     * @brief Remove edge.
     *
     * @param id Unique ID of edge to be removed
     */
    void removeEdge(id_t id);

private:
    void removeEdge_noLock(id_t id);

public:
    /**
     * @brief Set background image.
     *
     * @param path Filepath of new background
     */
    void setBackground(const std::string &path, const sf::Vector2f &position = sf::Vector2f(0, 0), const sf::Vector2f &scale = sf::Vector2f(1.0, 1.0), double alpha = 1.0);

    /**
     * @brief Clear background image.
     */
    void clearBackground();

    /**
     * @brief Join the window main thread.
     * 
     * The window main thread exits when the user presses the close button in
     * the window.
     */
    void join();

    /**
     * @brief Enable node drawing.
     * 
     * If set to false, does not draw nodes but may improve performance
     * significantly.
     * 
     * @param b True to draw nodes, false to not draw nodes
     */
    void setEnabledNodes(bool b = true);
    /**
     * @brief Enable edge drawing.
     * 
     * @param b Draw nodes if true, false otherwise
     */
    void setEnabledEdges(bool b = true);
    /**
     * @brief Enable node label drawing.
     * 
     * @param b Draw node labels if true, false otherwise
     */
    void setEnabledNodesText(bool b = true);
    /**
     * @brief Enable edge label drawing.
     * 
     * @param b Draw edge labels if true, false otherwise
     */
    void setEnabledEdgesText(bool b = true);

    /**
     * @brief Allow edges to be zipped.
     * 
     * Zipping is the act of collecting all vertices that make up a set of
     * objects, and group all those vertices into a single, large vertex array.
     * This has the advantage of being a lot faster to draw than drawing each
     * object separately; performance improves by about 20 times in large
     * graphs with many edges.
     * 
     * Please take close attention to the fact that, if edge zipping is enabled,
     * if you change an edge (or the position of a node that has edges connected
     * to it) you must afterwards call GraphViewer::setZipEdges(true) again, so
     * the zipped edges object is properly updated.
     * 
     * @param b True to zip edges, false if not.
     */
    void setZipEdges(bool b = false);

    /**
     * @brief Lock access to object.
     * 
     * This is important especially if you are editing parameters of nodes/edges
     * that are inside a GraphViewer instance for which createWindow(int, int)
     * was already called. This is because, once you run createWindow(int, int),
     * a thread is created to manage that window's events.
     * 
     */
    void lock();
    /**
     * @brief Unlock access to object.
     */
    void unlock();

private:
    static std::mutex createWindowMutex;

    static const sf::Font DEBUG_FONT;           ///< @brief Debug font.
    static const int DEBUG_FONT_SIZE = 14;      ///< @brief Debug font size, in pixels.
    bool debug_mode = false;                    ///< @brief True if debug mode is enabled, false otherwise.
    FPSMonitor fps_monitor = FPSMonitor(1000);  ///< @brief FPS monitor.
    sf::Text debug_text;                        ///< @brief Debug text to be displayed.

    static const sf::Font FONT;                 ///< @brief Font.
    static const int FONT_SIZE = 16;            ///< @brief Font size.

    float scale = 1.0;                          ///< @brief Scale (changed with scroll).
    /**
     * @brief Scale factor.
     * 
     * Scale is updated after a scroll of delta by calculating
     * 
     * @code{.cpp}
     * scale = scale * pow(SCALE_DELTA, -delta);
     * @endcode
     */
    static constexpr float SCALE_DELTA = 1.5;
    sf::Vector2f center;                        ///< @brief Coordinates of center of the window.

    sf::Texture background_texture;             ///< @brief Background texture (must be kept alive).
    sf::Sprite background_sprite;               ///< @brief Background sprite.
    sf::RenderWindow *window = nullptr;         ///< @brief Window.
    sf::View *view       = nullptr;             ///< @brief Default view, to draw the graph.
    sf::View *debug_view = nullptr;             ///< @brief Debug view, to draw debug information.
    std::thread *main_thread = nullptr;         ///< @brief Main thread.
    bool windowOpen = false;

    bool enabledNodes     = true;               ///< @brief Node drawing enabled.
    bool enabledNodesText = true;               ///< @brief Node text drawing enabled.
    bool enabledEdges     = true;               ///< @brief Edge drawing enabled.
    bool enabledEdgesText = true;               ///< @brief Edge text drawing enabled.

    /**
     * @brief Class to save zipped edges.
     * 
     * Only works properly with vertex arrays meant to be drawn as sf::Quads.
     */
    class ZipEdges {
    private:
        std::vector<sf::Vertex> vertices;       ///< @brief Vertices vector, the zipped version of several vertex arrays
    public:
        /**
         * @brief Append vertex array.
         * 
         * @param a Vertex array to append.
         */
        void append(const sf::VertexArray &a);
        /**
         * @brief Get vertex vector.
         * 
         * @return const std::vector<sf::Vertex>& Vertex vector to be drawn.
         */
        const std::vector<sf::Vertex>& getVertices() const;
    };
    bool zipEdges = false;                      ///< @brief Zip edges or not.
    ZipEdges zip;                               ///< @brief Zipped edges object.
    /**
     * @brief Update zip object.
     */
    void updateZip();

    /**
     * @brief Mutex protecting structures that are being drawn and that can
     * be updated by another thread at the same time.
     */
    std::mutex graphMutex;
    std::unordered_map<id_t, Node*> nodes;   ///< @brief Nodes map.
    std::unordered_map<id_t, Edge*> edges;   ///< @brief Edges map.

    /**
     * @brief Main entry point for event processing.
     * 
     * This function is the entry point for the window thread, and manages
     * events and drawing. 
     */
    void run();
    /**
     * @brief Draw graph and debug information.
     */
    void draw();
    /**
     * @brief Draw debug information; called by GraphViewer::draw().
     */
    void drawDebug();

    /**
     * @brief Called on window resize.
     */
    void onResize();
    /**
     * @brief Called on mouse scroll; updates scale/zoom.
     * 
     * @param delta Scroll delta (usually +1 or -1)
     */
    void onScroll(float delta);

    /**
     * @brief Recalculate views on window resize, or dragging inside window.
     */
    void recalculateView();

    /**
     * @brief Window width.
     */
    unsigned int width;

    /**
     * @brief Window height.
     */
    unsigned int height;
};

#include "lines.h"

#endif
