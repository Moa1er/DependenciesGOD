#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QString>
#include <QScreen>
#include <QPainter>
#include <QImageWriter>
#include <QDir>
#include <QFileDialog>
#include <QMimeData>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , filesManager()
    , depenManager(&filesManager)
{
    ui->setupUi(this);
    treeViewElements.setHorizontalHeaderLabels ({"Dependencies"});
    connect(
        &filesManager, &FilesManager::needTreeViewUpdate,
        this, &MainWindow::updateTreeView);

///////////////////////////////////////////////////////////////////////////////////
//PART FOR TESTING
///////////////////////////////////////////////////////////////////////////////////
//    QString dirWithFiles = "C:/ProjectsQt/DepenciesGODFilesTest";
////    QString dirWithFiles2 = QFileDialog::getExistingDirectory(this, tr("Select Directory"));
//    QString dirWithFiles3 = "C:/Github/LabeoTechGithubs/AwakeQt";
//    QString dirWithFiles4 = "C:/Github/DependenciesGOD/DependenciesGOD";
//    filesManager.dirWithFiles_ = dirWithFiles3;
//    filesManager.addFilesAndInit(dirWithFiles3);
//    depenManager.buildTree();

//    // Create actions and menus
//    createActions();
//    createMenus();
//    drawingManager = new DrawingManager(ui);
////    drawingManager->drawTree(depenManager.treeOfDepen, 0, 0);

////    DepenNode* testFindWidth;
////    testFindWidth = new DepenNode("level0", COLOR_PROJECT_FILE);
////    testFindWidth->childDepen_.push_back(new DepenNode("level1_0", COLOR_PROJECT_FILE));
////    testFindWidth->childDepen_.push_back(new DepenNode("level1_1", COLOR_PROJECT_FILE));
////    testFindWidth->childDepen_.push_back(new DepenNode("level1_2", COLOR_PROJECT_FILE));
////    testFindWidth->childDepen_[0]->childDepen_.push_back(new DepenNode("level2_0_0", COLOR_PROJECT_FILE));
////    testFindWidth->childDepen_[0]->childDepen_.push_back(new DepenNode("level2_0_1", COLOR_PROJECT_FILE));
////    testFindWidth->childDepen_[1]->childDepen_.push_back(new DepenNode("level2_1_0", COLOR_PROJECT_FILE));
////    testFindWidth->childDepen_[1]->childDepen_.push_back(new DepenNode("level2_1_1", COLOR_PROJECT_FILE));
////    testFindWidth->childDepen_[2]->childDepen_.push_back(new DepenNode("level2_2_0", COLOR_PROJECT_FILE));
////    testFindWidth->childDepen_[2]->childDepen_.push_back(new DepenNode("level2_2_1", COLOR_PROJECT_FILE));
////    testFindWidth->childDepen_[0]->childDepen_[0]->childDepen_.push_back(new DepenNode("level3_0_0_0", COLOR_PROJECT_FILE));
////    testFindWidth->childDepen_[0]->childDepen_[0]->childDepen_.push_back(new DepenNode("level3_0_0_1", COLOR_PROJECT_FILE));
////    testFindWidth->childDepen_[0]->childDepen_[1]->childDepen_.push_back(new DepenNode("level3_0_1_0", COLOR_PROJECT_FILE));
////    testFindWidth->childDepen_[0]->childDepen_[1]->childDepen_.push_back(new DepenNode("level3_0_1_1", COLOR_PROJECT_FILE));
////    testFindWidth->childDepen_[1]->childDepen_[1]->childDepen_.push_back(new DepenNode("level3_1_1_0", COLOR_PROJECT_FILE));
////    testFindWidth->childDepen_[1]->childDepen_[1]->childDepen_.push_back(new DepenNode("level3_1_1_1\nblballb\nfew", COLOR_PROJECT_FILE));
////    qDebug() << "Width tree is: " << drawingManager->findWidthTree(testFindWidth);

////    drawingManager->drawTree(testFindWidth, 400, 0);

//    int lastGraphY = 0;
//    foreach(DepenNode* node, depenManager.tmpDepenNodes_){
//        drawingManager->drawTree(node, lastGraphY, 0);
//        lastGraphY += 1000;
//    }

//    setAcceptDrops(true);
//    ui->centralwidget->setAcceptDrops(true);

///////////////////////////////////////////////////////////////////////////////////
//PART FOR RELEASE
///////////////////////////////////////////////////////////////////////////////////
    createMenus();
    //enabling drop of folder on app
    setAcceptDrops(true);

    //useful to create menu on right click to show graph
    ui->treeView_tw->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView_tw, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(onCustomContextMenu(const QPoint &)));
}

void MainWindow::onCustomContextMenu(const QPoint &point){
    qDebug() << "point is: " << point;
    QModelIndex index = ui->treeView_tw->indexAt(point);
    if (index.isValid()) {
        QString fileName = treeViewElements.itemFromIndex(index)->text();
        contextMenuTreeView->clear();
        contextMenuTreeView->addAction("Show graph", this, [this, fileName] {showGraph(fileName);});
        contextMenuTreeView->exec(ui->treeView_tw->viewport()->mapToGlobal(point));
    }
}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::dragEnterEvent(QDragEnterEvent *event){
    if(event->mimeData()->hasUrls())
        event->accept();
}

void MainWindow::dropEvent(QDropEvent *event){
    QList<QUrl> urls = event->mimeData()->urls();
    QString folderPath = urls[0].toLocalFile();
    processProjectFolder(folderPath);
}

void MainWindow::processProjectFolder(QString folderPath){
    filesManager.dirWithFiles_ = folderPath;
    filesManager.addFilesAndInit(folderPath);
    depenManager.buildTree();
    QStringList nodeKeys = depenManager.partialDepenNodes_.keys();
    if(nodeKeys.size() > 0){
        drawingManager = new DrawingManager(ui);
        drawingManager->drawTree(depenManager.partialDepenNodes_[nodeKeys[0]], 0, 0);
    }
}

void MainWindow::showGraph(QString fileName){
    QVector<QString> filePathPossible;
    for(auto const& [key, val] : filesManager.files_){
        if(key.contains(fileName)){
            filePathPossible.push_back(key);
        }
    }
    DepenNode* node = depenManager.getNodeAlrdyProcessed(filePathPossible[0]);
    drawingManager = new DrawingManager(ui);
    drawingManager->drawTree(node, 0, 0);
}

void MainWindow::updateTreeView(QString fileName, QStringList& dependencies){
    QStandardItem* rootItem = new QStandardItem(fileName);
    //used to make the items non-editable
    rootItem->setFlags(rootItem->flags() & ~Qt::ItemIsEditable);
    treeViewElements.appendRow(rootItem);
    for(int i = 0; i < dependencies.size(); i++){
        QStandardItem* childItem = new QStandardItem(dependencies[i]);
        //used to make the items non-editable
        childItem->setFlags(childItem->flags() & ~Qt::ItemIsEditable);
        rootItem->appendRow(childItem);
    }
    ui->treeView_tw->setModel(&treeViewElements);
    contextMenuTreeView = new QMenu(ui->treeView_tw);
}

// Create the menubar
void MainWindow::createMenus(){
    // Create Open Project action
    QAction *openProjectAction = new QAction(tr("&Open Project"), this);
    connect(openProjectAction, &QAction::triggered,
            this, [this]{processProjectFolder(
                    QFileDialog::getExistingDirectory(this, tr("Select Directory")));});
    // Create Save As action
    QAction *saveAsAction = new QAction(tr("&Save current graph as"), this);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    // Attach all actions to File menu option
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openProjectAction);
    fileMenu->addAction(saveAsAction);


    //create action for Help menu option

    QAction *howUseAppAction = new QAction(tr("&How to use this app ?"), this);
    connect(howUseAppAction, SIGNAL(triggered()), this, SLOT(showHelp()));

    // Attach all actions to Help menu option
    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(howUseAppAction);

    // Add menu items to the menubar
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(helpMenu);
}

void MainWindow::saveFile(){
    QString fileName = QFileDialog::getSaveFileName(this, "Save image", QCoreApplication::applicationDirPath(), "BMP Files (*.bmp);;JPEG (*.JPEG);;PNG (*.png)" );
    if (!fileName.isNull()){
        QPixmap pixMap = this->ui->graphicsView->grab();
        pixMap.save(fileName, nullptr, 100);
    }
}

void MainWindow::showHelp(){
    QMessageBox msg;
    msg.setText("To use this application:\n"
                "1 - Drag and drop the folder of your c/c++ projet anywhere on the app or go into File->Open Project and choose the folder of your projet.\n"
                "2 - On your left you will have a tree of each .h/.hpp or .c/.cpp in your project. Right click on one of them and click on 'Show graph'\n"
                "3 - A graph will appear on your screen. If you want to save it to a jpeg/bmp/png file go into File->Save current graph as");
    msg.exec();
}
