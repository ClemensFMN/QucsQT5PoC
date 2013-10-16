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
    QGraphicsLineItem *line;
    int mode;
    QString itemtype;
    int numComponents;
    Component *startComponent;
    QPointF startPoint;
    QList<Component*> components;
    QList<Wire*> wires;
};

#endif // MYSCENE_H
