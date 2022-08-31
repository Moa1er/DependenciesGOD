#include "depennode.h"

DepenNode::DepenNode(QString depenName, bool isExternDepen, QColor colorOnGraph)
{
    depenName_ = depenName;
    isExternDepen_ = isExternDepen;
    colorOnGraph_ = colorOnGraph;
}
