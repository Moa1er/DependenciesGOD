#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QString>
#include <QScreen>
#include <QPainter>
#include <QImageWriter>
#include <QDir>
#include <QFileDialog>

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

    QString dirWithFiles = "C:/ProjectsQt/DepenciesGODFilesTest";
//    QString dirWithFiles2 = QFileDialog::getExistingDirectory(this, tr("Select Directory"));
    QString dirWithFiles3 = "C:/Github/LabeoTechGithubs/AwakeQt";
    QString dirWithFiles4 = "C:/Github/DependenciesGOD/DependenciesGOD";
    filesManager.addFilesAndInit(dirWithFiles3);
    depenManager.buildTree();

//    scribbleArea = new ScribbleArea(ui->widget);
//    scribbleArea->drawRectangle(0, 0, 200, 200);

    // Create actions and menus
    createActions();
    createMenus();
    drawingManager = new DrawingManager(ui);
//    drawingManager->drawTree(depenManager.treeOfDepen, 0, 0);

//    DepenNode* testFindWidth;
//    testFindWidth = new DepenNode("level0", COLOR_PROJECT_FILE);
//    testFindWidth->childDepen_.push_back(new DepenNode("level1_0", COLOR_PROJECT_FILE));
//    testFindWidth->childDepen_.push_back(new DepenNode("level1_1", COLOR_PROJECT_FILE));
//    testFindWidth->childDepen_.push_back(new DepenNode("level1_2", COLOR_PROJECT_FILE));
//    testFindWidth->childDepen_[0]->childDepen_.push_back(new DepenNode("level2_0_0", COLOR_PROJECT_FILE));
//    testFindWidth->childDepen_[0]->childDepen_.push_back(new DepenNode("level2_0_1", COLOR_PROJECT_FILE));
//    testFindWidth->childDepen_[1]->childDepen_.push_back(new DepenNode("level2_1_0", COLOR_PROJECT_FILE));
//    testFindWidth->childDepen_[1]->childDepen_.push_back(new DepenNode("level2_1_1", COLOR_PROJECT_FILE));
//    testFindWidth->childDepen_[2]->childDepen_.push_back(new DepenNode("level2_2_0", COLOR_PROJECT_FILE));
//    testFindWidth->childDepen_[2]->childDepen_.push_back(new DepenNode("level2_2_1", COLOR_PROJECT_FILE));
//    testFindWidth->childDepen_[0]->childDepen_[0]->childDepen_.push_back(new DepenNode("level3_0_0_0", COLOR_PROJECT_FILE));
//    testFindWidth->childDepen_[0]->childDepen_[0]->childDepen_.push_back(new DepenNode("level3_0_0_1", COLOR_PROJECT_FILE));
//    testFindWidth->childDepen_[0]->childDepen_[1]->childDepen_.push_back(new DepenNode("level3_0_1_0", COLOR_PROJECT_FILE));
//    testFindWidth->childDepen_[0]->childDepen_[1]->childDepen_.push_back(new DepenNode("level3_0_1_1", COLOR_PROJECT_FILE));
//    testFindWidth->childDepen_[1]->childDepen_[1]->childDepen_.push_back(new DepenNode("level3_1_1_0", COLOR_PROJECT_FILE));
//    testFindWidth->childDepen_[1]->childDepen_[1]->childDepen_.push_back(new DepenNode("level3_1_1_1\nblballb\nfew", COLOR_PROJECT_FILE));
//    qDebug() << "Width tree is: " << drawingManager->findWidthTree(testFindWidth);

//    drawingManager->drawTree(testFindWidth, 400, 0);

    int lastGraphY = 0;
    foreach(DepenNode* node, depenManager.tmpDepenNodes_){
        drawingManager->drawTree(node, lastGraphY, 0);
        lastGraphY += 500;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
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
}

// Called when the user clicks Save As in the menu
void MainWindow::save()
{
    // A QAction represents the action of the user clicking
    QAction *action = qobject_cast<QAction *>(sender());

    // Stores the array of bytes of the users data
    QByteArray fileFormat = action->data().toByteArray();

    // Pass it to be saved
    saveFile(fileFormat);
}

// Define menu actions that call functions
void MainWindow::createActions()
{
    // Get a list of the supported file formats
    // QImageWriter is used to write images to files
    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        // Create an action for each file format
        QAction *action = new QAction(text, this);

        // Set an action for each file format
        action->setData(format);

        // When clicked call MainWindow::save()
        connect(action, SIGNAL(triggered()), this, SLOT(save()));

        // Attach each file format option menu item to Save As
        saveAsActs.append(action);
    }
}

// Create the menubar
void MainWindow::createMenus()
{
    // Create Save As option and the list of file types
    saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach (QAction *action, saveAsActs)
        saveAsMenu->addAction(action);

    // Attach all actions to File
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addMenu(saveAsMenu);


    // Add menu items to the menubar
    menuBar()->addMenu(fileMenu);
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    // Define path, name and default file type
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    // Get selected file from dialog
    // Add the proper file formats and extensions
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));

    // If no file do nothing
    if (fileName.isEmpty()) {
        return false;
    } else {

        // Call for the file to be saved
        return scribbleArea->saveImage(fileName, fileFormat.constData());
    }
}
