#include "myscene.h"

MyScene::MyScene(QObject *parent) : QGraphicsScene(parent) {
    line = 0;
    mode = 0;
    numComponents = 0;

    this->setItemIndexMethod(QGraphicsScene::NoIndex);
}

void MyScene::setMode(int mode) {
    MyScene::mode = mode;
}

void MyScene::setDrawingItem(QString itemtype) {
    this->itemtype = itemtype;
}

void MyScene::exportScene() {

    //qDebug() << wires.at(0)->getNode1()->getName() << wires.at(0)->getNode2()->getName();

    //qDebug() << wires.at(0)->getNode1();

    /*
    for(int i=0; i<components.count(); i++) {
        Component *c = components.at(i);
        qDebug() << c->getName();
        QList<Wire*> wires = c->getWires();
        for(int j=0; j<wires.length(); j++) {
            Wire *w = wires.at(j);
            qDebug() << w->getName();
        }
    }
*/
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {

    // place item
    if(mode == 2) {
        Component *n;
        if(itemtype == "Item 1") {
            numComponents++;
            QString name = QString("Node %0").arg(numComponents);
            n = new Component(name);
            components.append(n);
            addItem(n);
            n->setPos(mouseEvent->scenePos());
        }
    }

    // line
    if(mode == 1) {
        // find all items @ pos of mouse click
        QList<QGraphicsItem*> items = this->items(mouseEvent->scenePos());
        if(items.count() != 0) {
            Component *n = qgraphicsitem_cast<Component*>(items.last());
            if(n != 0) {
                // if it's a node continue
                startComponent = n;
                startPoint = n->mapFromScene(mouseEvent->scenePos());
                QList<QPointF> nnodes = n->getNodes();
                qreal dist = 500.0;
                for(int i=0; i<nnodes.length(); i++) {
                    QPointF temp = nnodes.at(i);
                    qreal temp_dist = (temp - startPoint).manhattanLength();
                    if(temp_dist < dist) {
                        startPoint = nnodes.at(i);
                        dist = temp_dist;
                    }
                }
                //qDebug() << "startPoint" << startPoint;
                line = new QGraphicsLineItem(QLineF(startComponent->mapToScene(startPoint), startComponent->mapToScene(startPoint)));
                line->setPen(QPen(Qt::red, 2));
                addItem(line);
            }
        }
    }
    // select
    if(mode == 0) {
        QGraphicsScene::mousePressEvent(mouseEvent);
    }
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {

    if(mode == 1) {
        // update the line accordingly
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    }

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {

    // create an edge & make it connect the startnode with the endnode
    if(mode == 1) {

        QList<QGraphicsItem*> items = this->items(mouseEvent->scenePos());

        if(items.count() != 0) {
            Component *n = qgraphicsitem_cast<Component*>(items.last());
            if(n != 0) {
                // if it's a node continue
                Component *endComponent = n;
                QPointF endPoint = n->mapFromScene(mouseEvent->scenePos());
                QList<QPointF> nnodes = n->getNodes();
                qreal dist = 500.0;
                for(int i=0; i<nnodes.length(); i++) {
                    QPointF temp = nnodes.at(i);
                    qreal temp_dist = (temp - endPoint).manhattanLength();
                    if(temp_dist < dist) {
                        endPoint = nnodes.at(i);
                        dist = temp_dist;
                    }
                }
                //qDebug() << "startindex" << startComponent->getNodes().indexOf(startPoint);
                //qDebug() << "endindex" << endComponent->getNodes().indexOf(endPoint);
                Wire *e = new Wire(startComponent, startComponent->getNodes().indexOf(startPoint), endComponent, endComponent->getNodes().indexOf(endPoint));
                wires.append(e);
                addItem(e);
                removeItem(line);
            }
        }
    }

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
