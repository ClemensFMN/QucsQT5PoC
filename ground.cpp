#include "ground.h"

Ground::Ground(QString name) : Component(name) {

    Ground::NUM_NODES = 1;
    _wires = QVector<QVector<Wire*> > (Ground::NUM_NODES);

    QPointF p1(0, 0);
    _nodes.append(p1);

    Lines.append(new Line( 0, 0, 20, 20 ,QPen(Qt::darkBlue,4)));

}

QRectF Ground::boundingRect() const {
    return QRectF(-40, -30, 80, 60);
}
