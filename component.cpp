#include "component.h"


Component::Component(QString name) {
    _name = name;
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF Component::boundingRect() const {
    return QRectF(0, 0, 0, 0);
}

void Component::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    for(int i=0; i<_nodes.size(); i++) {
        painter->drawEllipse(_nodes.at(i), 5, 5);
    }

    foreach (Line *l, Lines) {
      QPen pen(l->style);
      pen.setCosmetic(true); // do not scale thickness
      painter->setPen(pen);
      painter->drawLine(l->x1, l->y1, l->x2, l->y2);
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

int Component::getNumNodes() {
    return _nodes.size();
}

QPointF Component::getNearestPort(QPointF s) {
    QPointF startPoint = this->mapFromScene(s);
    // the maximum a click can be off is the diagonal of the bounding box
    QRectF bb = this->boundingRect();
    qreal dist = pow(bb.height(), 2)+  pow(bb.width(), 2);
    for(int i=0; i<_nodes.size(); i++) {
        QPointF temp = _nodes.at(i);
        qreal temp_dist = pow((temp - startPoint).x(), 2) + pow((temp - startPoint).y(), 2);
        if(temp_dist < dist) {
            startPoint = _nodes.at(i);
            dist = temp_dist;
        }
    }
    return startPoint;
}


QVariant Component::itemChange(GraphicsItemChange change, const QVariant & value) {
    if(change == ItemPositionHasChanged || change==QGraphicsItem::ItemPositionChange) {
        for(int i=0; i<Component::NUM_NODES; i++) {
            foreach(Wire *edge, _wires[i]) {
                edge->adjust();
            }
        }
    }

    return QGraphicsItem::itemChange(change, value);
}
