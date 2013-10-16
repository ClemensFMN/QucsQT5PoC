#include "wire.h"

Wire::Wire(Component *c1, int n1, Component *c2, int n2) {
    Component1 = c1;
    node1 = n1;
    Component2 = c2;
    node2 = n2;

    qDebug() << "c1:" << Component1->getName() << "Node:" << node1;
    qDebug() << "c2:" << Component2->getName() << "Node:" << node2;

    c1->addWire(n1, this);
    c2->addWire(n2, this);

    _name = "";

    qDebug() << "wire created";
}

QRectF Wire::boundingRect() const {

    qreal deltaX = qAbs(Component1->x() - Component2->x());
    qreal deltaY = qAbs(Component1->y() - Component2->y());

    qreal posX = qMin(Component1->x(), Component2->x());
    qreal posY = qMin(Component1->y(), Component2->y());

    return QRectF(posX, posY, deltaX, deltaY);
}



void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    qDebug() << "wire paint" << Component1->getName() << Component2->getName();
    qDebug() << Component1->getNodes() << Component2->getNodes();
    qDebug() << node1 << "..." << node2;
    qDebug() << Component1->getNodes().at(node1);
    qDebug() << Component2->getNodes().at(node2);

    QLineF line(mapFromItem(Component1, Component1->getNodes().at(node1)), mapFromItem(Component2, Component2->getNodes().at(node2)));
    painter->setPen(QPen(Qt::blue, 2, Qt::SolidLine));
    painter->drawLine(line);

}

QString Wire::getName() {
    return _name; //Node1->getName() + Node2->getName();
}

void Wire::setName(QString s) {
    _name = s;
}

void Wire::adjust() {
    update();
}

Component* Wire::getNode1() {
    return Component1;
}

Component* Wire::getNode2() {
    return Component2;
}


