#include "myscene.h"

MyScene::MyScene(QObject *parent) : QGraphicsScene(parent) {
    line = 0;
    mode = 0;
    numComponents = 0;
    netNum = 0;
    //this->setItemIndexMethod(QGraphicsScene::NoIndex);
}

void MyScene::setMode(int mode) {
    MyScene::mode = mode;
}

void MyScene::setDrawingItem(QString itemtype) {
    this->itemtype = itemtype;
}


void MyScene::NameWire(Component *c, int n) {

    // get the wires ffrom the component's node
    QVector<Wire*> wires = c->getWires(n);
    foreach(Wire* w, wires) {
        if(w->getName() == -1) {
            // if we haven't assigned it an id do it now
            w->setName(netNum);
            Component *nextComponent;
            int nextNode;
            if(w->getComponent1() == c) {
                nextComponent = w->getComponent2();
                nextNode = w->getNode2();
            }
            else {
                nextComponent = w->getComponent1();
                nextNode = w->getNode1();
            }
            // after we have obtained the next component's node, we start all over again
            NameWire(nextComponent, nextNode);
        }
        // already visited this node -> continue
    }

}

void MyScene::exportScene() {

    // this cleans out any netlist names we may have assigned to the wires so far
    foreach(Wire* w, wires) {
        w->setName(-1);
    }

    // the first net becomes the id 1
    netNum = 1;

    // go over all wires, check if they have already gotten a name; if not, start a depth-first search & assign an id
    foreach(Wire* w, wires) {
        if(w->getName() == -1) {
            // start traversal for one side of the wire
            NameWire(w->getComponent1(), w->getNode1());
            // and for the other...
            NameWire(w->getComponent2(), w->getNode2());
            netNum++;
        }
    }

    qDebug() << "List Wires";

    foreach(Wire* w, wires) {
        qDebug() << w->getComponent1()->getName() << w->getNode1() << w->getName() << w->getComponent2()->getName() << w->getNode2();
    }


    qDebug() << "Export Netlist";

    for(int i=0; i<components.size(); i++) {
        Component *c = components[i];
        qDebug() << c->getName() << c->getWires(0).at(0)->getName() << c->getWires(1).at(0)->getName();
    }

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
                // and find the closest node to the mouse click
                QVector<QPointF> nnodes = n->getNodes();
                qreal dist = 500.0;
                for(int i=0; i<nnodes.size(); i++) {
                    QPointF temp = nnodes.at(i);
                    qreal temp_dist = (temp - startPoint).manhattanLength();
                    if(temp_dist < dist) {
                        startPoint = nnodes.at(i);
                        dist = temp_dist;
                    }
                }
                //qDebug() << "startPoint" << startPoint;
                // Since the end point component isn't known yet, let's draw a line instead - as some kind of visual feedback...
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
                // and find the closest node to the mouse click -- TODO: factor out into a separate method
                QVector<QPointF> nnodes = n->getNodes();
                qreal dist = 500.0;
                for(int i=0; i<nnodes.size(); i++) {
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
                // having the wire in place, we can remove the temporary line...
                removeItem(line);
            }
        }
    }

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
