#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "filesmanager.h"
#include "drawingmanager.h"
#include "depenmanager.h"

#include <QMainWindow>
#include <QStandardItemModel>
#include <QDragEnterEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    // Will tie user actions to functions
    void createMenus();

    //override functions for drop folder on app
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    //main function that creates everything
    //the tree, the relations beetween, the files map etc etc
    void processProjectFolder(QString folderPath);
private:
    Ui::MainWindow *ui;
    FilesManager filesManager;
    DepenManager depenManager;
    QStandardItemModel treeViewElements;
    QWindow* window;
    // What we'll draw on
    DrawingManager *drawingManager;
    // The menu widgets
    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *contextMenuTreeView;

private slots:
    void updateTreeView(QString fileName, QStringList& dependencies);
    // Opens the Save dialog and saves
    void saveFile();
    void showHelp();
    void onCustomContextMenu(const QPoint &point);
    void showGraph(QString fileName);
};
#endif // MAINWINDOW_H
