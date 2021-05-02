#include "graphviewer.h"

#include <cmath>
#include <iostream>

using namespace std;
using namespace sf;

GraphViewer::Node::Node(){
    text.setFont(GraphViewer::FONT);
    text.setCharacterSize(GraphViewer::FONT_SIZE);
    text.setFillColor(Color::Black);
    update();
}
GraphViewer::Node::Node(GraphViewer::id_t id, const Vector2f &position):
    id(id),
    position(position)
{
    text.setFont         (GraphViewer::FONT     );
    text.setCharacterSize(GraphViewer::FONT_SIZE);
    text.setFillColor    (Color::Black          );
    update();
}

        GraphViewer::id_t   GraphViewer::Node::getId                (                           ) const { return id; }
        void                GraphViewer::Node::setPosition          (const Vector2f &position   )       { this->position = position; update(); }
const   Vector2f&           GraphViewer::Node::getPosition          (                           ) const { return position; }
        void                GraphViewer::Node::setSize              (float size                 )       { this->size = size; update(); }
        float               GraphViewer::Node::getSize              (                           ) const { return size; }
        void                GraphViewer::Node::setLabel             (const string &label        )       { text.setString(label); update(); }
        string              GraphViewer::Node::getLabel             (                           ) const { return text.getString(); }
        void                GraphViewer::Node::setColor             (const Color &color         )       { this->color = color; update(); }
const   Color&              GraphViewer::Node::getColor             (                           ) const { return color; }
        void                GraphViewer::Node::setIcon              (const string &path         )       { if(path == "") icon = Texture(); else icon.loadFromFile(path); isIcon = (path != ""); update(); }
const   Texture&            GraphViewer::Node::getIcon              (                           ) const { return icon; }
        bool                GraphViewer::Node::getIsIcon            (                           ) const { return isIcon; }
        void                GraphViewer::Node::setOutlineThickness  (float outlineThickness     )       { this->outlineThickness = outlineThickness; update(); }
        float               GraphViewer::Node::getOutlineThickness  (                           ) const { return outlineThickness; }
        void                GraphViewer::Node::setOutlineColor      (const Color &outlineColor  )       { this->outlineColor = outlineColor; update(); }
const   Color&              GraphViewer::Node::getOutlineColor      (                           ) const { return outlineColor; }
const   Shape*              GraphViewer::Node::getShape             (                           ) const { return shape; }
const   Text&               GraphViewer::Node::getText              (                           ) const { return text; }

void GraphViewer::Node::update(){
    delete shape;
    shape = nullptr;
    if(!getIsIcon()){
        CircleShape *newShape = new CircleShape(getSize()/2.0);
        newShape->setFillColor(getColor());
        newShape->setOutlineThickness(getOutlineThickness());
        newShape->setOutlineColor(getOutlineColor());
        shape = newShape;
    } else {
        RectangleShape *newShape = new RectangleShape(Vector2f(getSize(),getSize()));
        newShape->setTexture(&getIcon());
        shape = newShape;
    }
    shape->setOrigin(getSize()/2.0, getSize()/2.0);
    shape->setPosition(getPosition());    

    FloatRect bounds = text.getLocalBounds();
    text.setPosition(getPosition() - Vector2f(bounds.width/2.0, 0.8*bounds.height));

    for(Edge *e: edges){
        e->update();
    }
}
