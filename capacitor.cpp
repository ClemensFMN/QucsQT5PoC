#include "capacitor.h"

Capacitor::Capacitor(QString name) : Component(name) {

    Capacitor::NUM_NODES = 2;
    _wires = QVector<QVector<Wire*> > (Capacitor::NUM_NODES);

    QPointF p1(-20, 0);
    _nodes.append(p1);
    QPointF p2(20, 0);
    _nodes.append(p2);

}

QRectF Capacitor::boundingRect() const {
    return QRectF(-30, -20, 60, 40);
}


