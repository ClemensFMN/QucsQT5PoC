#include "capacitor.h"

Capacitor::Capacitor(QString name) : Component(name) {

    Capacitor::NUM_NODES = 2;
    _wires = QVector<QVector<Wire*> > (Capacitor::NUM_NODES);

    QPointF p1(-30, 0);
    _nodes.append(p1);
    QPointF p2(30, 0);
    _nodes.append(p2);

    Lines.append(new Line( -4,-11, -4, 11,QPen(Qt::darkBlue,4)));
    Lines.append(new Line(  4,-11,  4, 11,QPen(Qt::darkBlue,4)));

    Lines.append(new Line(-30,  0, -4,  0,QPen(Qt::darkBlue,2)));
    Lines.append(new Line(  4,  0, 30,  0,QPen(Qt::darkBlue,2)));

}

QRectF Capacitor::boundingRect() const {
    return QRectF(-40, -30, 80, 60);
}


