#include "wire.h"

/*!
* \brief Wire::Wire creates a new wire between two component's ports.
*/
Wire::Wire(Component *c1, int n1, Component *c2, int n2) {
    Component1 = c1;
    node1 = n1;
    Component2 = c2;
    node2 = n2;

    c1->addWire(n1, this);
    c2->addWire(n2, this);

    _name = -1;

    qDebug() << "wire created";
}

/*!
* \brief Wire::Wire creates a new wire between two component's ports.
* This constructor creates a "partial" wire, containing only of a startcomponent and a startport number.
*/
Wire::Wire(Component *c1, int n1) {
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
    Component1 = c1;
    node1 = n1;

    c1->addWire(n1, this);

    Component2 = 0;

    _name = -1;

    qDebug() << "wire started";
}

/*!
* \brief Wire::finalizeWire Finalizes a "partial" wire
*/
void Wire::finalizeWire(Component *c2, int n2) {
    Component2 = c2;
    node2 = n2;

    c2->addWire(n2, this);

    _name = -1;

    qDebug() << "wire finalized";

    qDebug() << "c1:" << Component1->getName() << "Node:" << node1;
    qDebug() << "c2:" << Component2->getName() << "Node:" << node2;

    this->update();

}

void Wire::addSegment(QPointF p) {
    points.append(p);
    qDebug() << "add segment";
}

QRectF Wire::boundingRect() const {

    /*
    Go over all segment points the wire consists (including the start- & endpoints)
    and find the min & max x & y values => this gives the bounding box...
    */

    // start with Component1
    qreal minX = mapFromItem(Component1, Component1->getNodes().at(node1)).x();
    qreal minY = mapFromItem(Component1, Component1->getNodes().at(node1)).y();
    qreal maxX = minX;
    qreal maxY = minY;


    for(int i=0; i<points.size(); i++) {
        minX = qMin(minX, points.at(i).x());
        minY = qMin(minY, points.at(i).y());
        maxX = qMax(maxX, points.at(i).x());
        maxY = qMax(maxY, points.at(i).y());
    }

    // when there is a second component connected, consider it in the min / max games as well...
    if(Component2 != 0) {
        // the minimum x / y values of the 2 components
        minX = qMin(minX, mapFromItem(Component2, Component2->getNodes().at(node2)).x());
        minY = qMin(minY, mapFromItem(Component2, Component2->getNodes().at(node2)).y());

        // the maximum x / y values of the 2 components
        maxX = qMax(maxX, mapFromItem(Component2, Component2->getNodes().at(node2)).x());
        maxY = qMax(maxY, mapFromItem(Component2, Component2->getNodes().at(node2)).y());
    }

    return QRectF(minX-2, minY-2, maxX-minX+4, maxY-minY+4);
}



void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(Qt::blue, 2, Qt::SolidLine));

    if(points.size() == 0) {
        // No points between Component1 & Component2
        if(Component2 != 0) {
            // if the second component is there; i.e. the wire has been finalized, draw a line
            // between the 2 components
            // need to convert from each component's coordinate system to the wire's coordinate system
            QLineF line(mapFromItem(Component1, Component1->getNodes().at(node1)), mapFromItem(Component2, Component2->getNodes().at(node2)));
            painter->drawLine(line);
        }
    }
    else {
        // in this case, there are points between Component1 and Component2
        // draw from Component1 to first point
        QLineF line1(mapFromItem(Component1, Component1->getNodes().at(node1)), points.at(0));
        painter->drawLine(line1);

        // connect the points
        QLineF l;
        for(int i=0; i<points.size()-1;i++) {
            l.setP1(points.at(i));
            l.setP2(points.at(i+1));
            painter->drawLine(l);
        }
        // if the 2nd component is there, connect the last point with the component
        if(Component2 != NULL) {
            QLineF lineL(points.at(points.size()-1), mapFromItem(Component2, Component2->getNodes().at(node2)));
            painter->drawLine(lineL);
        }
    }
}

int Wire::getName() {
    return _name; //Node1->getName() + Node2->getName();
}

void Wire::setName(int i) {
    _name = i;
}

void Wire::adjust() {
    update();
}

Component* Wire::getComponent1() {
    return Component1;
}

Component* Wire::getComponent2() {
    return Component2;
}

int Wire::getNode1() {
    return node1;
}

int Wire::getNode2() {
    return node2;
}
