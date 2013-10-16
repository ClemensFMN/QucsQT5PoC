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
    QString getName();
    void setName(QString s);
    Component* getComponent1();
    Component* getComponent2();
    int getNode1();
    int getNode2();

private:
    Component *Component1, *Component2;
    int node1, node2;
    QLineF line;
    QString _name;
};

#endif // WIRE_H
