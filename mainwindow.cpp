#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    QDockWidget *leftDock = new QDockWidget("Item Palette", this);
    leftDock->setAllowedAreas(Qt::LeftDockWidgetArea);

    itemList = new QListWidget();
    itemList->setFixedWidth(150);


    QListWidgetItem *item1 = new QListWidgetItem("Resistor");
    QListWidgetItem *item2 = new QListWidgetItem("Capacitor");
    QListWidgetItem *item3 = new QListWidgetItem("Ground");

    itemList->addItem(item1);
    itemList->addItem(item2);
    itemList->addItem(item3);

    leftDock->setWidget(itemList);

    this->addDockWidget(Qt::LeftDockWidgetArea, leftDock);

    scene = new MyScene();
    scene->setSceneRect(0,0,1000,1000);

    QGraphicsView *view = new QGraphicsView(scene);
    this->setCentralWidget(view);

    QActionGroup *toolActions = new QActionGroup(this);
    toolActions->setExclusive(true);

    QAction *wireAction = new QAction("Wire", this);
    QAction *selectAction = new QAction("Select", this);
    QAction *itemAction = new QAction("Place Component", this);
    QAction *delItemAction = new QAction("Delete Item", this);
    wireAction->setActionGroup(toolActions);
    wireAction->setCheckable(true);
    selectAction->setActionGroup(toolActions);
    selectAction->setCheckable(true);
    itemAction->setActionGroup(toolActions);
    itemAction->setCheckable(true);
    delItemAction->setActionGroup(toolActions);
    delItemAction->setCheckable(true);


    QAction *exportAction = new QAction("Export", this);

    QToolBar *toolBar = new QToolBar("Actions", this);
    toolBar->addAction(wireAction);
    toolBar->addAction(selectAction);
    toolBar->addAction(itemAction);
    toolBar->addAction(delItemAction);
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

    if(a->text() == "Wire") {
        scene->setMode(1);
    }

    if(a->text() == "Place Component") {
        scene->setMode(2);
        //initialization for "Place Item"
        int selected = itemList->currentRow();
        if(selected == 0) {
            scene->setDrawingItem("Resistor");
        }
        if(selected == 1) {
            scene->setDrawingItem("Capacitor");
        }
        if(selected == 2) {
            scene->setDrawingItem("Ground");
        }
    }

    if(a->text() == "Delete Item") {
        scene->setMode(3);
    }

}

void MainWindow::itemClicked(QListWidgetItem *listitem) {
        scene->setDrawingItem(listitem->text());
}
