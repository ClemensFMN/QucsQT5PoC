#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtWidgets>
#include <QDebug>

#include "myscene.h"

class MyScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int getSelectedItem();

public slots:
    void toolActionsSlot(QAction*);
    void itemClicked(QListWidgetItem*);

private:
    MyScene *scene;
    QListWidget *itemList;

};

#endif // MAINWINDOW_H
