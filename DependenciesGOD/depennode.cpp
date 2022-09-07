#include "depennode.h"

DepenNode::DepenNode(DepenNode* depenNode){
    if(depenNode == nullptr){
        return;
    }
    depenName_ = depenNode->depenName_;
    isExternDepen_ = depenNode->isExternDepen_;
    colorOnGraph_ = depenNode->isExternDepen_;
    //shallow copy instead of deed copy.. may be a problem
    childDepen_ = depenNode->childDepen_;
}

DepenNode::DepenNode(QString depenName, bool isExternDepen, QColor colorOnGraph)
{
    depenName_ = depenName;
    isExternDepen_ = isExternDepen;
    colorOnGraph_ = colorOnGraph;
}
