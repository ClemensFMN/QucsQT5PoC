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
        if(line == 0) {
            qDebug() << "line = 0";
            // find all items @ pos of mouse click
            QList<QGraphicsItem*> items = this->items(mouseEvent->scenePos());
            if(items.count() != 0) {
                Component *n = qgraphicsitem_cast<Component*>(items.last());
                if(n != 0) {
                    // if it's a node continue
                    startComponent = n;
                    startPoint = n->getNearestPort(mouseEvent->scenePos());
                    //qDebug() << "startPoint" << startPoint;
                    // Since the end point component isn't known yet, let's draw a line instead - as some kind of visual feedback...
                    line = new QGraphicsLineItem(QLineF(startComponent->mapToScene(startPoint), startComponent->mapToScene(startPoint)));
                    line->setPen(QPen(Qt::red, 2));
                    //addItem(line);
                    // we would need to instantiate a "partial" wire - the start point is known

                }
            }
        }
        else {
            qDebug() << "line != 0";
            QGraphicsLineItem *newLine = new QGraphicsLineItem(QLineF(line->line().p1(), mouseEvent->scenePos()));
            newLine->setPen(QPen(Qt::red, 2));
            addItem(newLine);

            if(mouseEvent->button()==Qt::LeftButton) {
                //here we add additional points to the "partial" wire...
            }
            if(mouseEvent->button()==Qt::RightButton) {
                //here we finalize the "partial" wire...
                line = 0;
            }
        }
    }
    // select
    if(mode == 0) {
        QGraphicsScene::mousePressEvent(mouseEvent);
    }
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {

    /*qDebug() << "mousemove" << mode;

    if(mode == 1) {
        qDebug() << "mousemove";
        // update the line accordingly
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    }
*/
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {

    // create an edge & make it connect the startnode with the endnode
    /*
    if(mode == 1) {

        QList<QGraphicsItem*> items = this->items(mouseEvent->scenePos());

        if(items.count() != 0) {
            Component *n = qgraphicsitem_cast<Component*>(items.last());
            if(n != 0) {
                // if it's a node continue
                Component *endComponent = n;
                QPointF endPoint = n->getNearestPort(mouseEvent->scenePos());
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

    if(mode == 1) {
        if(line != 0) {
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
            line->setPen(QPen(Qt::red, 2));
            addItem(line);
        }
    }
*/
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
