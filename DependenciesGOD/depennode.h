#ifndef DEPENNODE_H
#define DEPENNODE_H

#include <QVector>
#include <QColor>

class DepenNode
{
public:
    DepenNode(DepenNode* depenNode);
    DepenNode(QString depenName, bool isExternDepen, QColor colorOnGraph);
    void setDepenName(QString newDepenName){depenName_ = newDepenName;};
public:
    QString depenName_;
    QVector<DepenNode*> childDepen_;
    QColor colorOnGraph_;
    bool isExternDepen_;
};

#endif // DEPENNODE_H
