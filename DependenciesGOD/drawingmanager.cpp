#include "drawingmanager.h"

DrawingManager::DrawingManager(Ui::MainWindow* ui)
{
    scene_ = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene_);
}

void DrawingManager::drawTree(DepenNode* tree, int yDrawing, int xDrawing){
    uiNodes_.push_back(new DepenNodeUi(scene_, tree, yDrawing, xDrawing));
    yDrawing += 100;
    const int parentX = xDrawing;
    const int paddingBetNodes = 100;
    const int nbChildNode = tree->childDepen_.size();
    const int anchorPointXParent = uiNodes_[uiNodes_.size() - 1]->anchorPointX_;
    const int anchorPointYParent = uiNodes_[uiNodes_.size() - 1]->anchorPointY_;
    QVector<int> widthChildren;
    for(int i = 0; i < nbChildNode; i++){
        widthChildren.push_back(findWidthTree(tree->childDepen_[i]));
        if(tree->depenName_ == "level0"){
            qDebug() << "child: " << findWidthTree(tree->childDepen_[i]);
        }
    }
    int widthNode = std::accumulate(widthChildren.begin(), widthChildren.end(), 0);
    if(tree->depenName_ == "level0"){
        qDebug() << "sum_of_elems: " << widthNode;
    }

    int lastXPos = -widthNode/2 + xDrawing;
    for(int i = 0; i < nbChildNode; i++){
       xDrawing = lastXPos + widthChildren[i]/2;
       lastXPos += widthChildren[i];

       drawTree(tree->childDepen_[i], yDrawing, xDrawing);
       const int paddingDrawing = 5;
       //gets the x coordonate by getting the width/2 of the printed name
       const int anchorPointXChild = xDrawing + (new QGraphicsTextItem(tree->childDepen_[i]->depenName_))->sceneBoundingRect().width()/2;
       const int anchorPointYChild = yDrawing - paddingDrawing;
       QPen outlinePen(Qt::black);
       outlinePen.setWidth(2);
       scene_->addLine(anchorPointXParent, anchorPointYParent,
                       anchorPointXChild, anchorPointYChild, outlinePen);
    }
}

int DrawingManager::findWidthTree(DepenNode* tree){
    const int paddingBetNodes = 40;
    const int nbChildNode = tree->childDepen_.size();
    int sumWidth = 0;
    for(int i = 0; i < nbChildNode; i++){
        sumWidth += findWidthTree(tree->childDepen_[i]);
        sumWidth += (new QGraphicsTextItem(tree->childDepen_[i]->depenName_))->sceneBoundingRect().width();
        if(i > 0){
            sumWidth += paddingBetNodes;
        }
    }

    //checking in case we there is no child
    int widthNodeInItself = (new QGraphicsTextItem(tree->depenName_))->sceneBoundingRect().width();
    if(sumWidth < widthNodeInItself){
        sumWidth = widthNodeInItself + paddingBetNodes;
    }
    return sumWidth;
}
