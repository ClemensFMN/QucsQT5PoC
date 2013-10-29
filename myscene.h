#ifndef MYSCENE_H
#define MYSCENE_H

#include <QtGui>
#include <QtWidgets>
#include <QDebug>

#include "component.h"
#include "mainwindow.h"


class MyScene : public QGraphicsScene
{
    Q_OBJECT

public:
    MyScene(QObject *parent = 0);
    void setMode(int mode);
    void setDrawingItem(QString itemtype);

public slots:
    void exportScene();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);


private:
    // a "temporary" line to connect the startComponent with the curent mouse position while drawing a wire
    QGraphicsLineItem *line;
    int mode;
    QString itemtype;
    int numComponents;
    Component *startComponent;
    QPointF startPoint;
    // all components within the current schematic
    QList<Component*> components;
    // all wires...
    QList<Wire*> wires;
    void NameWire(Component*, int);
    int netNum;
};

#endif // MYSCENE_H
