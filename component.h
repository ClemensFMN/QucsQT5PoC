#ifndef COMPONENT_H
#define COMPONENT_H

#include <QtGui>
#include <QtWidgets>
#include <QDebug>
#include <QList>

#include "wire.h"

class Wire;

/*
 * A component which can be placed onto a schematic
 * currently, the component has two nodes (ports)
*/
class Component : public QGraphicsItem {

public:
    Component(QString);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void addWire(int, Wire *e);
    QString getName();
    QList<Wire*> getWires1();
    QList<Wire*> getWires2();
    QList<Wire*> getWires(int);
    QList<QPointF> getNodes();


protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);

private:
    QString _name;
    //the set of node positions
    QList<QPointF> _nodes;
    // a list for every node to keep track of whcih wires are connected to this node -> I know this is ugly
    QList<Wire*> _wires1, _wires2;
    // this would be the preferred way to do it (a list of a list of wires; one list for every node); but
    // I'm struggling with initializing this list
    QList<QList<Wire*> > _wires;
};

#endif // COMPONENT_H
