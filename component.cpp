#include "component.h"


Component::Component(QString name) {
    _name = name;

    QPointF p1(-20, 0);
    _nodes.append(p1);
    QPointF p2(20, 0);
    _nodes.append(p2);

    _wires = QVector<QVector<Wire*> > (Component::NUM_NODES);

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF Component::boundingRect() const {
    return QRectF(-30, -20, 60, 40);
}

void Component::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    for(int i=0; i<_nodes.size(); i++) {
        painter->drawEllipse(_nodes.at(i), 5, 5);
    }

    painter->drawText(boundingRect(), _name);
    painter->setPen(QPen(Qt::green, 2, Qt::SolidLine));
    painter->drawRect(boundingRect());
}

void Component::addWire(int node, Wire *e) {
    _wires[node].append(e);
}

QString Component::getName() {
    return _name;
}

QVector<Wire*> Component::getWires(int node) {
    return _wires[node];
}

QVector<QPointF> Component::getNodes() {
    return _nodes;
}

QVariant Component::itemChange(GraphicsItemChange change, const QVariant & value) {
    if(change == ItemPositionHasChanged) {
        for(int i=0; i<Component::NUM_NODES; i++) {
            foreach(Wire *edge, _wires[i]) {
                edge->adjust();
            }
        }
    }

    return QGraphicsItem::itemChange(change, value);
}
