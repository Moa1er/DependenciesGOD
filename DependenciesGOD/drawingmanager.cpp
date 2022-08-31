#include "drawingmanager.h"

DrawingManager::DrawingManager(Ui::MainWindow* ui)
{
    scene_ = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene_);
}

int DrawingManager::drawTree(DepenNode* tree, int yDrawing, int xDrawing){
    QString fileName = QFileInfo(tree->depenName_).fileName();
    const int widthTextNode = (new QGraphicsTextItem(fileName))->sceneBoundingRect().width();
    const int parentX = xDrawing;
    xDrawing -= widthTextNode/2;
    uiNodes_.push_back(new DepenNodeUi(scene_, tree, yDrawing, xDrawing));
    yDrawing += 100;
    const int paddingBetNodes = 20;
    const int paddingDrawing = 5;
    const int nbChildNode = tree->childDepen_.size();
    const int anchorPointXParent = uiNodes_[uiNodes_.size() - 1]->anchorPointX_;
    const int anchorPointYParent = uiNodes_[uiNodes_.size() - 1]->anchorPointY_;
    QVector<int> widthChildren;
    for(int i = 0; i < nbChildNode; i++){
        widthChildren.push_back(findWidthTree(tree->childDepen_[i]));
    }
    int widthNode = std::accumulate(widthChildren.begin(), widthChildren.end(), 0);
    widthNode += (nbChildNode - 1) * 20;
    if(widthNode < widthTextNode + 2*paddingDrawing){
        widthNode = widthTextNode + 2*paddingDrawing;
    }

    int lastXPos = -widthNode/2 + parentX;

    drawBounderiesTree(lastXPos, yDrawing, widthNode);

    for(int i = 0; i < nbChildNode; i++){
        //if there is only one child we position at same place as parent
        if(nbChildNode == 1){
            xDrawing = parentX;
        }else{
            xDrawing = lastXPos + widthChildren[i]/2;
        }

       lastXPos += widthChildren[i] + paddingBetNodes;

       drawTree(tree->childDepen_[i], yDrawing, xDrawing);
       //gets the x coordonate by getting the width/2 of the printed name
       const int anchorPointXChild = xDrawing;
       const int anchorPointYChild = yDrawing - paddingDrawing;
       drawLine(anchorPointXParent, anchorPointYParent,
                anchorPointXChild, anchorPointYChild, Qt::black);
    }

    return yDrawing;
}

int DrawingManager::findWidthTree(DepenNode* tree){
//    if(tree->depenName_.contains("minidump_format.h")){
//        bool test = false;
//    }
    const int paddingDrawing = 5;
    const int nbChildNode = tree->childDepen_.size();
    int sumWidth = 0;
    for(int i = 0; i < nbChildNode; i++){
        sumWidth += findWidthTree(tree->childDepen_[i]);
    }
    //checking in case we there is no child
    QString fileName = QFileInfo(tree->depenName_).fileName();
    int widthNodeInItself = (new QGraphicsTextItem(fileName))->sceneBoundingRect().width();
    if(sumWidth < widthNodeInItself){
        sumWidth = widthNodeInItself;
    }
    if(nbChildNode > 0){
        //adds the sum of padding beetween each node to general width of tree
        sumWidth += (nbChildNode - 1) * 20;
    }
    sumWidth += 2*paddingDrawing;
    return sumWidth;
}

//mainly a function for debug
float currentHue = 0.0;
void DrawingManager::drawBounderiesTree(int startX, int startY, int widthTree){
    QColor rdmColor = QColor::fromHslF(currentHue, 1.0, 0.5);
    currentHue += 0.618033988749895f;
    currentHue = std::fmod(currentHue, 1.0f);

    const int paddingOneLevel = 100;
    //first line left
    drawLine(startX, startY - paddingOneLevel, startX, startY, rdmColor);

    //add text for width debug
    QGraphicsTextItem *item = new QGraphicsTextItem(QString::number(widthTree));
    item->setPos(startX + 2, startY - paddingOneLevel/2);
    scene_->addItem(item);

    //second line right
    drawLine(startX + widthTree, startY - paddingOneLevel, startX + widthTree, startY, rdmColor);
}

void DrawingManager::drawLine(int x1, int y1, int x2, int y2, QColor color){
    QPen outlinePen(color);
    outlinePen.setWidth(2);
    scene_->addLine(x1, y1, x2, y2, outlinePen);
}
