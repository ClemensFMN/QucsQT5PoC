#ifndef COMPONENT_H
#define COMPONENT_H

#include <QtGui>
#include <QtWidgets>
#include <QDebug>
#include <QList>

#include "wire.h"

class Wire;


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
    QList<QPointF> _nodes;
    QList<Wire*> _wires1, _wires2;
    QList<QList<Wire*> > _wires;
};

#endif // COMPONENT_H
