#ifndef COMPONENT_H
#define COMPONENT_H

#include <QtGui>
#include <QtWidgets>
#include <QDebug>
#include <QVector>

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
    QVector<Wire*> getWires(int);
    QVector<QPointF> getNodes();


protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);

protected:
    QString _name;
    //the set of node positions
    QVector<QPointF> _nodes;
    QVector<QVector<Wire*> > _wires;
    int NUM_NODES;
};

#endif // COMPONENT_H
