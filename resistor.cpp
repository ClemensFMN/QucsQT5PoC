#include "resistor.h"

Resistor::Resistor(QString name) : Component(name) {

    Resistor::NUM_NODES = 2;
    _wires = QVector<QVector<Wire*> > (Resistor::NUM_NODES);

    QPointF p1(-20, 0);
    _nodes.append(p1);
    QPointF p2(20, 0);
    _nodes.append(p2);

}


QRectF Resistor::boundingRect() const {
    return QRectF(-30, -20, 60, 40);
}

