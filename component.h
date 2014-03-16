#ifndef COMPONENT_H
#define COMPONENT_H

#include <QtGui>
#include <QtWidgets>
#include <QDebug>
#include <QVector>
#include <QList>

#include "wire.h"

class Wire;


struct Line {
  Line(int _x1, int _y1, int _x2, int _y2, QPen _style)
       : x1(_x1), y1(_y1), x2(_x2), y2(_y2), style(_style) {};
  int   x1, y1, x2, y2;
  QPen  style;
};

/*
 * A component which can be placed onto a schematic
*/
class Component : public QGraphicsItem {

public:
    Component(QString);

    QRectF boundingRect() const;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
    void addWire(int, Wire *e);
    void removeWire(int, Wire *e);
    QString getName();
    QVector<Wire*> getWires(int);
    QVector<QPointF> getNodes();
    int getNearestPort(QPointF);
    int getNumNodes();


protected:
    QVariant itemChange(GraphicsItemChange , const QVariant & );

protected:
    QString _name;
    //the set of node positions
    QVector<QPointF> _nodes;
    QVector<QVector<Wire*> > _wires;
    int NUM_NODES;
    QList<Line*> Lines;
};

#endif // COMPONENT_H
