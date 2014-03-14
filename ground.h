#ifndef GROUND_H
#define GROUND_H

#include <QtGui>
#include <QtWidgets>
#include <QDebug>
#include <QVector>

#include "component.h"


class Ground : public Component {
public:
    Ground(QString);
    QRectF boundingRect() const;
};

#endif // GROUND_H
