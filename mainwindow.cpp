#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    QDockWidget *leftDock = new QDockWidget("Item Palette", this);
    leftDock->setAllowedAreas(Qt::LeftDockWidgetArea);

    itemList = new QListWidget();
    itemList->setFixedWidth(150);


    QListWidgetItem *item1 = new QListWidgetItem("Resistor");
    QListWidgetItem *item2 = new QListWidgetItem("Capacitor");

    itemList->addItem(item1);
    itemList->addItem(item2);

    leftDock->setWidget(itemList);

    this->addDockWidget(Qt::LeftDockWidgetArea, leftDock);

    scene = new MyScene();
    scene->setSceneRect(0,0,1000,1000);

    QGraphicsView *view = new QGraphicsView(scene);
    this->setCentralWidget(view);

    QActionGroup *toolActions = new QActionGroup(this);
    toolActions->setExclusive(true);

    QAction *lineAction = new QAction("Line", this);
    QAction *selectAction = new QAction("Select", this);
    QAction *itemAction = new QAction("Place Item", this);
    lineAction->setActionGroup(toolActions);
    lineAction->setCheckable(true);
    selectAction->setActionGroup(toolActions);
    selectAction->setCheckable(true);
    itemAction->setActionGroup(toolActions);
    itemAction->setCheckable(true);

    QAction *exportAction = new QAction("Export", this);

    QToolBar *toolBar = new QToolBar("Actions", this);
    toolBar->addAction(lineAction);
    toolBar->addAction(selectAction);
    toolBar->addAction(itemAction);
    toolBar->addAction(exportAction);
    this->addToolBar(toolBar);

    connect(toolActions, SIGNAL(triggered(QAction*)), this, SLOT(toolActionsSlot(QAction*)));
    connect(exportAction, SIGNAL(triggered()), scene, SLOT(exportScene()));
    connect(itemList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(itemClicked(QListWidgetItem*)));

}

MainWindow::~MainWindow() {
}

int MainWindow::getSelectedItem() {
    return 10;
}

void MainWindow::toolActionsSlot(QAction *a) {

    if(a->text() == "Select") {
        scene->setMode(0);
    }

    if(a->text() == "Line") {
        scene->setMode(1);
    }

    if(a->text() == "Place Item") {
        scene->setMode(2);
        //initialization for "Place Item"
        int selected = itemList->currentRow();
        if(selected == 0) {
            scene->setDrawingItem("Resistor");
        }
        if(selected == 1) {
            scene->setDrawingItem("Capacitor");
        }
    }

}

void MainWindow::itemClicked(QListWidgetItem *listitem) {
        scene->setDrawingItem(listitem->text());
}
