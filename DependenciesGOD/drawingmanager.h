#ifndef DRAWINGMANAGER_H
#define DRAWINGMANAGER_H

#include "ui_mainwindow.h"

#include "depennode.h"
#include "depennodeui.h"

#include <QGraphicsScene>


class DrawingManager : public QWidget
{
//METHODS
public:
    DrawingManager(Ui::MainWindow* ui);
    int drawTree(DepenNode* tree, int yDrawing, int xDrawing);
    int findWidthTree(DepenNode* tree);
private:
    void drawBounderiesTree(int startX, int startY, int widthTree);
    void drawLine(int x1, int y1, int x2, int y2, QColor color);
//ATTRIBUTES
private:
    QGraphicsScene *scene_;
    QVector<DepenNodeUi*> uiNodes_;
};

#endif // DRAWINGMANAGER_H
