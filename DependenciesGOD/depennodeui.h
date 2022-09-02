#ifndef DEPENNODEUI_H
#define DEPENNODEUI_H

#include "depennode.h"

#include <QGraphicsScene>
#include <QGraphicsTextItem>

#include <QFileInfo>

class DepenNodeUi
{
public:
    DepenNodeUi(QGraphicsScene* scene, DepenNode* node, int y, int x){
        if(node->depenName_ == "foo.h\nstring\nvector\nstdint.h\nsys/types.h\ninttypes.h"){
            bool test = false;
        }
        QBrush blueBrush(node->colorOnGraph_);
        QPen outlinePen(Qt::black);
        outlinePen.setWidth(2);

        QString fileName = "";
        //if the node is external then it doesn't have a path so we take the whole name
        if(node->isExternDepen_){
            fileName = node->depenName_;
        }else{
            fileName = QFileInfo(node->depenName_).fileName();
        }
        nameDepen_ = new QGraphicsTextItem(fileName);
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

#endif // DEPENNODEUI_H
