#ifndef RESISTOR_H
#define RESISTOR_H

#include <QtGui>
#include <QtWidgets>
#include <QDebug>
#include <QVector>

#include "component.h"

class Resistor : public Component {

public:
    Resistor(QString);
    QRectF boundingRect() const;
};

#endif // RESISTOR_H
