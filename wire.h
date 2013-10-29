#ifndef WIRE_H
#define WIRE_H

#include "component.h"

class Component;

class Wire : public QGraphicsItem {

public:
    Wire(Component *, int, Component *, int);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void adjust();
    int getName();
    void setName(int i);
    Component* getComponent1();
    Component* getComponent2();
    int getNode1();
    int getNode2();

private:
    // the components the wire connects
    Component *Component1, *Component2;
    // and the nodes (of the components) the wire connects
    int node1, node2;
    QLineF line;
    int _name;
};

#endif // WIRE_H
