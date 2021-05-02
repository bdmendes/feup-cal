#include "graphviewer.h"

#include <cmath>
#include <iostream>

using namespace std;
using namespace sf;

GraphViewer::Edge::Edge(){
    text.setFont(GraphViewer::FONT);
    text.setCharacterSize(GraphViewer::FONT_SIZE);
    text.setFillColor(Color::Black);
}

GraphViewer::Edge::Edge(
    GraphViewer::id_t id,
    GraphViewer::Node &u,
    GraphViewer::Node &v,
    GraphViewer::Edge::EdgeType edge_type
):
    id(id),
    u(&u),
    v(&v),
    edge_type(edge_type)
{
    u.edges.insert(this);
    v.edges.insert(this);

    text.setFont(GraphViewer::FONT);
    text.setCharacterSize(GraphViewer::FONT_SIZE);
    text.setFillColor(Color::Black);
    
    update();
}

        GraphViewer::id_t           GraphViewer::Edge::getId        (                                       ) const { return id; }
        void                        GraphViewer::Edge::setFrom      (Node *u                                )       { this->u->edges.erase(this); this->u = u; this->u->edges.insert(this); update(); }
const   GraphViewer::Node*          GraphViewer::Edge::getFrom      (                                       ) const { return u; }
        void                        GraphViewer::Edge::setTo        (Node *v                                )       { this->v->edges.erase(this); this->v = v; this->v->edges.insert(this); update(); }
const   GraphViewer::Node*          GraphViewer::Edge::getTo        (                                       ) const { return v; }
        void                        GraphViewer::Edge::setEdgeType  (GraphViewer::Edge::EdgeType edge_type  )       { this->edge_type = edge_type; update(); }
        GraphViewer::Edge::EdgeType GraphViewer::Edge::getEdgeType  (                                       ) const { return edge_type; }
        void                        GraphViewer::Edge::setLabel     (const string &label                    )       { this->label = label; update(); }
const   string&                     GraphViewer::Edge::getLabel     (                                       ) const { return label; }
        void                        GraphViewer::Edge::setColor     (const Color &color                     )       { this->color = color; update(); }
const   Color&                      GraphViewer::Edge::getColor     (                                       ) const { return color; }
        void                        GraphViewer::Edge::setDashed    (bool dashed                            )       { this->dashed = dashed; update(); }
        bool                        GraphViewer::Edge::getDashed    (                                       ) const { return dashed; }
        void                        GraphViewer::Edge::setThickness (float thickness                        )       { this->thickness = thickness; update(); }
        float                       GraphViewer::Edge::getThickness (                                       ) const { return thickness; }
        void                        GraphViewer::Edge::setWeight    (float weight                           )       { delete this->weight; this->weight = new float(weight); update(); }
const   float*                      GraphViewer::Edge::getWeight    (                                       ) const { return weight; }
        void                        GraphViewer::Edge::setFlow      (float flow                             )       { delete this->flow; this->flow = new float(flow); update(); }
const   float*                      GraphViewer::Edge::getFlow      (                                       ) const { return flow; }
const   VertexArray*                GraphViewer::Edge::getShape     (                                       ) const { return shape; }
const   Text&                       GraphViewer::Edge::getText      (                                       ) const { return text; }

void GraphViewer::Edge::update(){
    delete shape;
    shape = nullptr;

    sf::Vector2f uPos = u->getPosition();
    sf::Vector2f vPos = v->getPosition();
    sf::Vector2f uvVec  = vPos - uPos;
    sf::Vector2f uvUVec = uvVec/(sqrt(uvVec.x*uvVec.x + uvVec.y*uvVec.y));
    uPos = uPos + uvUVec*(u->getSize()/2.0f);
    vPos = vPos - uvUVec*(v->getSize()/2.0f);

    shape = new LineShape(uPos, vPos, 0);
    if(edge_type == EdgeType::DIRECTED){
        ArrowHead arrow(uPos, vPos, getThickness());
        shape->append(arrow);
        uPos = arrow.getLineConnection();
    }
    if(!getDashed()){
        shape->append(FullLineShape(uPos, vPos, getThickness()));
    } else {
        shape->append(DashedLineShape(uPos, vPos, getThickness()));
    }
    shape->setFillColor(getColor());

    string label = getLabel();
    if(getWeight() != nullptr) label += (label == "" ? "" : " ")+string("w: ")+to_string(int(*getWeight()));
    if(getFlow  () != nullptr) label += (label == "" ? "" : " ")+string("f: ")+to_string(int(*getFlow  ()));
    text.setString(label);
    FloatRect bounds = text.getLocalBounds();
    text.setPosition((u->getPosition() + v->getPosition())/2.0f - Vector2f(bounds.width/2.0, 0.8*bounds.height));
}
