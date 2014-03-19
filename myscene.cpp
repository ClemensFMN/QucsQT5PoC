#include "myscene.h"

MyScene::MyScene(QObject *parent) : QGraphicsScene(parent) {
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

/*!
* \brief MyScene::NameWire assigns a netID to a component's port (and all subsequently connected wires)
* \param c the component, n the port to which the global variable netNum shall be assigned as netID
* Starts with all wires leaving the component's port and assigns all (electrically) connected wires
* the value netNum.
*/
void MyScene::NameWire(Component *c, int n) {

    // get the wires from the component's port
    QVector<Wire*> wires = c->getWires(n);
    foreach(Wire* w, wires) {
        if(w->getName() == -1) {
            // haven't assigned an id to the wire yet
            w->setName(netNum);
            Component *nextComponent;
            int nextNode;
            // obtain the component at the other side of the wire
            if(w->getComponent1() == c) {
                nextComponent = w->getComponent2();
                nextNode = w->getNode2();
            }
            else {
                nextComponent = w->getComponent1();
                nextNode = w->getNode1();
            }
            // recursively start naming wires from the component at the other side
            NameWire(nextComponent, nextNode);
        }
        // already visited this node -> continue
    }

}

/*!
* \brief MyScene::exportScene exports a scene in the qucs netlist format (the properties are still missing)
* Triggered by the GUI, exports the netlist
*/
void MyScene::exportScene() {

    // this cleans out any netlist names we may have assigned to the wires so far
    foreach(Wire* w, wires) {
        w->setName(-1);
    }

    // all wires connected with ground get id = 0
    netNum = 0;
    // go over all components and if it's a gnd component, assign netID 0 to all connected wires
    foreach(Component* c, components) {
        if(c->getName() == "GND") {
            NameWire(c, 0);
        }
    }


    // the first net becomes the id 1
    netNum = 1;

    // go over all wires, check if they have already gotten a name; if not, trigger assignment on both sides of the wire
    foreach(Wire* w, wires) {
        if(w->getName() == -1) {
            // start traversal for one side of the wire
            NameWire(w->getComponent1(), w->getNode1());
            // and for the other...
            NameWire(w->getComponent2(), w->getNode2());
            netNum++;
        }
    }

    // iterate over all components & write the netlist to the console
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

    // wire
    if(mode == 1) {
        if(captureMode == true) {
            qDebug() << "capturemode = true";
            // find all items @ pos of mouse click
            QList<QGraphicsItem*> items = this->items(mouseEvent->scenePos());
            if(items.count() != 0) {
                Component *startComponent = qgraphicsitem_cast<Component*>(items.last());
                if(startComponent != 0) {
                    // if it's a node continue
                    int startPort = startComponent->getNearestPort(mouseEvent->scenePos());
                    captureMode = false;
                    // instantiate a "partial" wire - the startcomponent & startport are known
                    w = new Wire(startComponent, startPort);
                    // the partial wire is able to draw itself
                    addItem(w);
                }
            }
        }
        else { //capturemode = false

            qDebug() << "capturemode = false";

            if(mouseEvent->button() == Qt::LeftButton) {
                // add point to wire & redraw wire
                w->addSegment(mouseEvent->scenePos());
                w->update();
            }
            else {

                // find all items @ pos of mouse click
                QList<QGraphicsItem*> items = this->items(mouseEvent->scenePos());
                if(items.count() != 0) {
                    Component *endComponent = qgraphicsitem_cast<Component*>(items.last());
                    if(endComponent != 0) {
                        // if it's a node continue
                        int endPort = endComponent->getNearestPort(mouseEvent->scenePos());
                        captureMode = true;
                        // finalize "partial" wire
                        w->finalizeWire(endComponent, endPort);
                        w->update();
                        // and add it to the list of wires
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

        // find all items @ pos of mouse click
        QList<QGraphicsItem*> items = this->items(mouseEvent->scenePos());

        if(items.size() != 0) {

            // assume that there is only one component selected => last is enough
            // TODO The docs say that type must be implemented for the cast to work, but somehow it needs the
            // comparison with type as well -> not nice :-(
            Component *c = qgraphicsitem_cast<Component*>(items.last());
            if(c != 0 & c->type() == QGraphicsItem::UserType+2) {
                // remove the component both from the components array
                components.removeAll(c);
                // and from the scene
                this->removeItem(c);
            }

            Wire *w = qgraphicsitem_cast<Wire*>(items.last());
            // in case a wire shall be removed, things are a bit trickier
            if(w != 0 & w->type() == QGraphicsItem::UserType+1) {
                // delete the refernece in the component to the wire
                w->getComponent1()->removeWire(w->getNode1(), w);
                w->getComponent2()->removeWire(w->getNode2(), w);
                // remove the wire from the wires array
                wires.removeAll(w);
                // and finally from the scene
                this->removeItem(w);
            }
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
