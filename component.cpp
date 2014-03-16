#include "component.h"


Component::Component(QString name) {
    _name = name;
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF Component::boundingRect() const {
    return QRectF(0, 0, 0, 0);
}

void Component::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {

    //draw the ports
    for(int i=0; i<_nodes.size(); i++) {
        painter->drawEllipse(_nodes.at(i), 5, 5);
    }

    // draw the lines
    foreach (Line *l, Lines) {
      QPen pen(l->style);
      pen.setCosmetic(true); // do not scale thickness
      painter->setPen(pen);
      painter->drawLine(l->x1, l->y1, l->x2, l->y2);
    }

    //draw the boundingbox (debugging purposes)
    painter->drawText(boundingRect(), _name);
    painter->setPen(QPen(Qt::green, 2, Qt::SolidLine));
    painter->drawRect(boundingRect());
}

/*!
* \brief Component::addWire adds a wire to a specific port
*/
void Component::addWire(int node, Wire *e) {
    _wires[node].append(e);
}

/*!
* \brief Component::removeWire removes a specific wire from a specific port
*/
void Component::removeWire(int node, Wire *e) {
    for(int i=0; i<_wires[node].size(); i++) {
        if(_wires[node].at(i) == e) {
            _wires[node].remove(i);
        }
    }
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

/*!
* \brief Component::getNearestPort finds the component port closest to a point
* \param s the point for which the closest port is searched
* \return the closest port (index)
* when drawing a wire, the user will click "near" the desired start/end port - this function finds the closest port
* wrt the click position and returns the corresponding port index.
*/
int Component::getNearestPort(QPointF s) {
    QPointF startPoint = this->mapFromScene(s);
    int minPort;
    // the maximum a click can be off is the diagonal of the bounding box
    QRectF bb = this->boundingRect();
    qreal dist = (pow(bb.height(), 2) + pow(bb.width(), 2))/2;
    for(int i=0; i<_nodes.size(); i++) {
        QPointF temp = _nodes.at(i);
        qreal temp_dist = pow((temp.x() - startPoint.x()), 2) + pow((temp.y() - startPoint.y()), 2);
        if(temp_dist < dist) {
            minPort = i;
            dist = temp_dist;
        }
    }
    return minPort;
}

/*!
* \brief Component::itemChange is called when the componet "changes"; check on position changes; implement some kind of snap function & update connected wires
*/
QVariant Component::itemChange(GraphicsItemChange change, const QVariant & value) {
    if(change == ItemPositionHasChanged || change==QGraphicsItem::ItemPositionChange) {

        QPointF newPos = value.toPointF();
        int tx = (int) newPos.x();
        tx = tx / 20;
        newPos.setX(20 * tx);
        int ty = (int) newPos.y();
        ty = ty / 20;
        newPos.setY(20 * ty);

        for(int i=0; i<Component::NUM_NODES; i++) {
            foreach(Wire *edge, _wires[i]) {
                edge->adjust();
            }
        }
        return newPos;
    }
    return QGraphicsItem::itemChange(change, value);
}
