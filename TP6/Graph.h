/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include "MutablePriorityQueue.h"


template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
    T info;						// content of the vertex
    std::vector<Edge<T> > adj;		// outgoing edges

    double dist = 0;
    Vertex<T> *path = NULL;
    int queueIndex = 0; 		// required by MutablePriorityQueue

    bool visited = false;		// auxiliary field
    bool processing = false;	// auxiliary field

    void addEdge(Vertex<T> *dest, double w);

public:
    Vertex(T in);
    T getInfo() const;
    double getDist() const;
    Vertex *getPath() const;

    bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
    friend class Graph<T>;
    friend class MutablePriorityQueue<Vertex<T>>;
};


template <class T>
Vertex<T>::Vertex(T in): info(in) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
    adj.push_back(Edge<T>(d, w));
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
    return this->dist < vertex.dist; // necessary for priority queue
}

template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template <class T>
double Vertex<T>::getDist() const {
    return this->dist;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const {
    return this->path;
}

/********************** Edge  ****************************/

template <class T>
class Edge {
    Vertex<T> * dest;      // destination vertex
    double weight;         // edge weight
public:
    Edge(Vertex<T> *d, double w);
    friend class Graph<T>;
    friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


/*************************** Graph  **************************/

template <class T>
class Graph {
    std::vector<Vertex<T> *> vertexSet;    // vertex set

public:
    Vertex<T> *findVertex(const T &in) const;
    bool addVertex(const T &in);
    bool addEdge(const T &sourc, const T &dest, double w);
    int getNumVertex() const;
    std::vector<Vertex<T> *> getVertexSet() const;

    // Fp06 - single source
    void unweightedShortestPath(const T &s);    //TODO...
    void dijkstraShortestPath(const T &s);      //TODO...
    void bellmanFordShortestPath(const T &s);   //TODO...
    std::vector<T> getPath(const T &origin, const T &dest) const;   //TODO...

    // Fp06 - all pairs
    void floydWarshallShortestPath();   //TODO...
    std::vector<T> getfloydWarshallPath(const T &origin, const T &dest) const;   //TODO...

};

template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
    for (auto v : vertexSet)
        if (v->info == in)
            return v;
    return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
    if ( findVertex(in) != NULL)
        return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == NULL || v2 == NULL)
        return false;
    v1->addEdge(v2,w);
    return true;
}


/**************** Single Source Shortest Path algorithms ************/

template<class T>
void Graph<T>::unweightedShortestPath(const T &orig) {
    for (auto& v: vertexSet){
        v->dist = INF;
        v->path = nullptr;
    }

    auto startV = findVertex(orig);
    if (startV == NULL) return;

    startV->dist = 0;
    std::queue<Vertex<T>*> q;
    q.push(startV);
    while (!q.empty()){
        auto currV = q.front();
        q.pop();
        for (auto& e: currV->adj){
            auto destV = e.dest;
            if (destV->dist == INF){
                q.push(destV);
                destV->dist = currV->dist + 1;
                destV->path = currV;
            }
        }
    }
}


template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) {
    for (auto& v: vertexSet){
        v->dist = INF;
        v->path = nullptr;
    }

    auto startV = findVertex(origin);
    if (startV == NULL) return;

    /* Use mutable heap so that a queue change is O(log n) instead of O(n) */
    MutablePriorityQueue<Vertex<T>> q;
    startV->dist = 0;
    q.insert(startV); // key = dist(V) = 0

    while (!q.empty()){
        auto currV = q.extractMin(); // greedy; would not work with negative edges!
        for (auto& e: currV->adj){
            auto destV = e.dest;
            if (destV->dist > currV->dist + e.weight){
                bool alreadyQueued = destV->dist != INF;
                destV->dist = currV->dist + e.weight;
                destV->path = currV;
                if (alreadyQueued){
                    q.decreaseKey(destV);
                } else {
                    q.insert(destV);
                }
            }
        }
    }
}


template<class T>
void Graph<T>::bellmanFordShortestPath(const T &orig) {
    for (auto& v: vertexSet){
        v->dist = INF;
        v->path = nullptr;
    }

    auto startV = findVertex(orig);
    if (startV == NULL) return;
    startV->dist = 0;

    for (int i = 1; i <= vertexSet.size() - 1; i++){ // longest path is v-1 edges long
        bool changed = false;
        for (auto& v : vertexSet){
            for (auto& e : v->adj){
                auto destV = e.dest;
                if (destV->dist > v->dist + e.weight){
                    changed = true;
                    destV->dist = v->dist + e.weight;
                    destV->path = v;
                }
            }
        }
        if (!changed) break;
    }

    /* Fail if negative cycles are found */
    for (auto& v : vertexSet){
        for (auto& e : v->adj){
            auto destV = e.dest;
            if (v->dist + e.weight < destV->dist){
                throw std::logic_error("There are cycles of negative weight");
            }
        }
    }
}


template<class T>
std::vector<T> Graph<T>::getPath(const T &origin, const T &dest) const{
    std::vector<T> res;

    /* Start at the end and traverse back. path(v) is always the best */
    auto currV = findVertex(dest);
    if (currV == nullptr) return {};
    while (currV != nullptr){
        res.push_back(currV->info);
        currV = currV->path;
    }
    std::reverse(res.begin(), res.end());
    return res;
}



/**************** All Pairs Shortest Path  ***************/

struct pairHash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};

template <class T>
static std::unordered_map<std::pair<int,int>,std::vector<Vertex<T>*>, pairHash> floydPath;

template<class T>
void Graph<T>::floydWarshallShortestPath() {
    int dist[50][50];
    if (vertexSet.size() >= 49) throw std::logic_error("No sufficient space in dp table");

    /* Set length 1 paths */
    for (int i = 0; i < 50; i++){
        for (int j = 0; j < 50; j++){
            dist[i][j] = 10e5;
        }
    }
    for (int i = 1; i <= vertexSet.size(); i++){
        for (int j = 1; j <= vertexSet.size(); j++){
            for (auto& e: vertexSet.at(i-1)->adj){
                if (e.dest->info == vertexSet.at(j-1)->info){
                    floydPath<T>[std::make_pair(i, j)] = {};
                    dist[i][j] = e.weight;
                    break;
                }
            }
        }
    }

    /* Add middle men to paths */
    for (int k = 1; k <= vertexSet.size(); k++){
        auto middleMan = vertexSet.at(k - 1);
        for (int i = 1; i <= vertexSet.size(); i++){
            for (int j = 1; j <= vertexSet.size(); j++){
                if (dist[i][k] + dist[k][j] < dist[i][j]){
                    dist[i][j] = dist[i][k] + dist[k][j];
                    auto& fullPath = floydPath<T>[std::make_pair(i, j)];
                    auto pathBeforeMiddleMan = floydPath<T>[std::make_pair(i, k)];
                    auto pathAfterMiddleMan = floydPath<T>[std::make_pair(k,j)];
                    fullPath = pathBeforeMiddleMan;
                    fullPath.push_back(middleMan);
                    fullPath.insert(fullPath.end(), pathAfterMiddleMan.begin(),
                                    pathAfterMiddleMan.end());
                }
            }
        }
    }
}

template<class T>
std::vector<T> Graph<T>::getfloydWarshallPath(const T &orig, const T &dest) const{
    std::vector<T> res;

    /* Find integer correspondence */
    int i = 0, j = 0;
    for (int a = 1; a <= vertexSet.size(); a++){
        if (vertexSet.at(a-1)->info == orig){
            i = a;
        } else if (vertexSet.at(a-1)->info == dest){
            j = a;
        }
    }
    if (i == 0 || j == 0) return {};

    res.push_back(orig);
    for (auto& v: floydPath<T>[std::make_pair(i, j)]){
        res.push_back(v->info);
    }
    res.push_back(dest);
    return res;
}


#endif /* GRAPH_H_ */
