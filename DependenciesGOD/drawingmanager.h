#ifndef DRAWINGMANAGER_H
#define DRAWINGMANAGER_H

#include "ui_mainwindow.h"

#include "depennode.h"

#include <QGraphicsScene>
#include <QGraphicsTextItem>

class DepenNodeUi
{
public:
    DepenNodeUi(QGraphicsScene* scene, DepenNode* node, int y, int x){
        QBrush blueBrush(node->colorOnGraph_);
        QPen outlinePen(Qt::black);
        outlinePen.setWidth(2);

        nameDepen_ = new QGraphicsTextItem(node->depenName_);
        nameDepen_->setPos(x, y);
        nameDepen_->setZValue(1);
        scene->addItem(nameDepen_);

        QRectF rectDim = nameDepen_->sceneBoundingRect();
        const int padding = 5;
        borderDepen_ = scene->addRect(rectDim.x() - padding, rectDim.y() - padding,
                                      rectDim.width() + (2*padding), rectDim.height() + (2*padding),
                                      outlinePen, blueBrush);
        borderDepen_->setZValue(0);
        y_ = y;
        x_ = x;
        anchorPointX_ = rectDim.x() + rectDim.width()/2;
        anchorPointY_ = rectDim.y() + padding + rectDim.height();

//        qDebug() << "Start point: ";
//        qDebug() << "X start: " << rectDim.x() - padding;
//        qDebug() << "Y start: " << rectDim.y() - padding;
//        qDebug() << "X final: " << rectDim.x() - padding + rectDim.width() + (2*padding);
//        qDebug() << "Y final: " << rectDim.y() - padding + rectDim.height() + (2*padding);
    };
public:
    int y_;
    int x_;
    int anchorPointX_;
    int anchorPointY_;
private:
    QGraphicsRectItem* borderDepen_;
    QGraphicsTextItem* nameDepen_;
};

class DrawingManager : public QWidget
{
//METHODS
public:
    DrawingManager(Ui::MainWindow* ui);
    void drawTree(DepenNode* tree, int yDrawing, int xDrawing);
//private:
    int findWidthTree(DepenNode* tree);
//ATTRIBUTES
private:
    QGraphicsScene *scene_;
    QVector<DepenNodeUi*> uiNodes_;
};

#endif // DRAWINGMANAGER_H
