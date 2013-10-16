#include "component.h"


Component::Component(QString name) {
    _name = name;

    QPointF p1(-20, 0);
    _nodes.append(p1);
    QPointF p2(20, 0);
    _nodes.append(p2);

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF Component::boundingRect() const {
    return QRectF(-30, -20, 60, 40);
}

void Component::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    for(int i=0; i<_nodes.length(); i++) {
        painter->drawEllipse(_nodes.at(i), 5, 5);
    }

    painter->drawText(boundingRect(), _name);
}

void Component::addWire(int node, Wire *e) {
    if(node==0) {
        _wires1.append(e);
    } else {
        _wires2.append(e);
    }
    _wires[node].append(e);
}

QString Component::getName() {
    return _name;
}

QList<Wire *> Component::getWires1(){
    return _wires1;
}

QList<Wire *> Component::getWires2(){
    return _wires2;
}

QList<Wire*> Component::getWires(int node) {
    return _wires[node];
}

QList<QPointF> Component::getNodes() {
    return _nodes;
}

QVariant Component::itemChange(GraphicsItemChange change, const QVariant & value) {

    if(change == ItemPositionHasChanged) {
        foreach(Wire *edge, _wires1) {
            edge->adjust();
        }
        foreach(Wire *edge, _wires2) {
            edge->adjust();
        }
    }

    return QGraphicsItem::itemChange(change, value);
}
