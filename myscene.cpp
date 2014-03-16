#include "myscene.h"

MyScene::MyScene(QObject *parent) : QGraphicsScene(parent) {
    line = 0;
    mode = 0;
    numComponents = 0;
    netNum = 0;
    captureMode = true;
    //this->setItemIndexMethod(QGraphicsScene::NoIndex);
}

void MyScene::setMode(int mode) {
    MyScene::mode = mode;
}

void MyScene::setDrawingItem(QString itemtype) {
    this->itemtype = itemtype;
}


void MyScene::NameWire(Component *c, int n) {

    // get the wires from the component's node
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

    // all wires connected with ground get id = 0
    netNum = 0;
    // let's go over all components and if it's a gnd component, assign net id 0 to all connected wires
    foreach(Component* c, components) {
        if(c->getName() == "GND") {
            NameWire(c, 0);
        }
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

    qDebug() << "Export Netlist";
    for(int i=0; i<components.size(); i++) {
        Component *c = components[i];
        if(c->getName() != "GND") {
            int nodes = c->getNumNodes();
            QString temp = c->getName();

            for(int i=0; i<nodes; i++) {
                int num = c->getWires(i).at(0)->getName();
                temp = temp + " " + QString("%1").arg(num);
            }

            qDebug() << temp;
        }
    }
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {

    // place item
    if(mode == 2) {
        Component *n;
        if(itemtype == "Resistor") {
            numComponents++;
            QString name = QString("Resistor %0").arg(numComponents);
            n = new Resistor(name);
            components.append(n);
            addItem(n);
            n->setPos(mouseEvent->scenePos());
        }
        if(itemtype == "Capacitor") {
            numComponents++;
            QString name = QString("Capacitor %0").arg(numComponents);
            n = new Capacitor(name);
            components.append(n);
            addItem(n);
            n->setPos(mouseEvent->scenePos());
        }
        if(itemtype == "Ground") {
            numComponents++;
            QString name = QString("GND").arg(numComponents);
            n = new Ground(name);
            components.append(n);
            addItem(n);
            n->setPos(mouseEvent->scenePos());
        }
    }

    // line
    if(mode == 1) {
        if(captureMode == true) {
            qDebug() << "capturemode = true";
            // find all items @ pos of mouse click
            QList<QGraphicsItem*> items = this->items(mouseEvent->scenePos());
            if(items.count() != 0) {
                Component *n = qgraphicsitem_cast<Component*>(items.last());
                if(n != 0) {
                    // if it's a node continue
                    Component *startComponent = n;
                    QPointF startPoint = n->getNearestPort(mouseEvent->scenePos());
                    // Since the end point component isn't known yet, let's draw a line instead - as some kind of visual feedback...
                    line = new QGraphicsLineItem(QLineF(startComponent->mapToScene(startPoint), startComponent->mapToScene(startPoint)));
                    line->setPen(QPen(Qt::red, 2));
                    //addItem(line);
                    captureMode = false;
                    // we instantiate a "partial" wire - the start point is known
                    w = new Wire(startComponent, startComponent->getNodes().indexOf(startPoint));
                    // we expect the partial wire to be able to draw itself
                    addItem(w);
                }
            }
        }
        else { //capturemode = false

            qDebug() << "capturemode = false";

            if(mouseEvent->button() == Qt::LeftButton) {
                line = new QGraphicsLineItem(QLineF(line->line().p2(), mouseEvent->scenePos()));
                line->setPen(QPen(Qt::red, 2));
                //addItem(line);
                // add point to wire & redraw wire
                w->addSegment(mouseEvent->scenePos());
                w->update();
            }
            else {

                // find all items @ pos of mouse click
                QList<QGraphicsItem*> items = this->items(mouseEvent->scenePos());
                if(items.count() != 0) {
                    Component *n = qgraphicsitem_cast<Component*>(items.last());
                    if(n != 0) {
                        // if it's a node continue
                        Component *endComponent = n;
                        QPointF endPoint = n->getNearestPort(mouseEvent->scenePos());
                        // Since the end point component isn't known yet, let's draw a line instead - as some kind of visual feedback...
                        line = new QGraphicsLineItem(QLineF(line->line().p2(), endComponent->mapToScene(endPoint)));
                        line->setPen(QPen(Qt::red, 2));
                        //addItem(line);
                        captureMode = true;
                        // finalize "partial" wire
                        w->finalizeWire(endComponent, endComponent->getNodes().indexOf(endPoint));
                        w->update();
                        wires.append(w);
                    }
                }
            }
        }
    }
    // select
    if(mode == 0) {
        QGraphicsScene::mousePressEvent(mouseEvent);
    }

    // delete
    if(mode == 3) {
        qDebug() << "remove";
        QList<QGraphicsItem*> items = this->selectedItems();

        qDebug() << items;

        Component *c = qgraphicsitem_cast<Component*>(items.last());
        if(c != 0) {
            components.removeAll(c);
            this->removeItem(c);
        }

        Wire *w = qgraphicsitem_cast<Wire*>(items.last());
        if(w != 0) {
            // remove wire refernce in components
            w->getComponent1()->removeWire(w->getNode1(), w);
            w->getComponent2()->removeWire(w->getNode2(), w);
            wires.removeAll(w);
            this->removeItem(w);
        }

        QGraphicsScene::mousePressEvent(mouseEvent);
    }

}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
