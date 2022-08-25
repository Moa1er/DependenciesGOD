#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

#include "filesmanager.h"
#include "scribblearea.h"
#include "drawingmanager.h"
#include "depenmanager.h"

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
    void createActions();
    void createMenus();
    // Opens the Save dialog and saves
    bool saveFile(const QByteArray &fileFormat);
private:
    Ui::MainWindow *ui;
    FilesManager filesManager;
    DepenManager depenManager;
    QStandardItemModel treeViewElements;
    QWindow* window;
    // What we'll draw on option 1
    ScribbleArea *scribbleArea;
    // What we'll draw on option 2
    DrawingManager *drawingManager;
    // The menu widgets
    QMenu *fileMenu;
    QMenu *saveAsMenu;
    // Actions tied to specific file formats
    QList<QAction *> saveAsActs;

private slots:
    void updateTreeView(QString fileName, QStringList& dependencies);
    void save();
};
#endif // MAINWINDOW_H
