#ifndef CAPACITOR_H
#define CAPACITOR_H

#include <QtGui>
#include <QtWidgets>
#include <QDebug>
#include <QVector>

#include "component.h"


class Capacitor : public Component {

public:
    Capacitor(QString);
    QRectF boundingRect() const;
};

#endif // CAPACITOR_H
