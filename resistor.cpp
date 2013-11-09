#include "resistor.h"

Resistor::Resistor(QString name) : Component(name) {

    Resistor::NUM_NODES = 3;
    _wires = QVector<QVector<Wire*> > (Resistor::NUM_NODES);

    QPointF p1(-30, 0);
    _nodes.append(p1);
    QPointF p2(30, 0);
    _nodes.append(p2);

    Lines.append(new Line(-30,  0,-18,  0,QPen(Qt::darkBlue,2)));
    Lines.append(new Line(-18,  0,-15, -7,QPen(Qt::darkBlue,2)));
    Lines.append(new Line(-15, -7, -9,  7,QPen(Qt::darkBlue,2)));
    Lines.append(new Line( -9,  7, -3, -7,QPen(Qt::darkBlue,2)));
    Lines.append(new Line( -3, -7,  3,  7,QPen(Qt::darkBlue,2)));
    Lines.append(new Line(  3,  7,  9, -7,QPen(Qt::darkBlue,2)));
    Lines.append(new Line(  9, -7, 15,  7,QPen(Qt::darkBlue,2)));
    Lines.append(new Line( 15,  7, 18,  0,QPen(Qt::darkBlue,2)));
    Lines.append(new Line( 18,  0, 30,  0,QPen(Qt::darkBlue,2)));

}


QRectF Resistor::boundingRect() const {
    return QRectF(-40, -40, 80, 80);
}

