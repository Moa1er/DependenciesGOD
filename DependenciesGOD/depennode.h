#ifndef DEPENNODE_H
#define DEPENNODE_H

#include <QVector>
#include <QColor>

class DepenNode
{
public:
    DepenNode(QString depenName, QColor colorOnGraph);
    void setDepenName(QString newDepenName){depenName_ = newDepenName;};
public:
    QString depenName_;
    QVector<DepenNode*> childDepen_;
    QColor colorOnGraph_;
};

#endif // DEPENNODE_H
