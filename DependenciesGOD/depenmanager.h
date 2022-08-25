#ifndef DEPENMANAGER_H
#define DEPENMANAGER_H

#include "depennode.h"

#include "filesmanager.h"

class DepenManager
{
public:
    DepenManager(FilesManager* fileManager);
    void buildTree();
public:
    DepenNode* treeOfDepen;
private:
    void makeDepen(DepenNode* node);
    void regroupExternalDepen(DepenNode* node);
    DepenNode* getNodeAlrdyProcessed(QString nodeToFind);
    DepenNode* findNode(QString nodeToFind, DepenNode* rootNode);

    //functions to print in terminal
    void printTree(DepenNode* nodeToPrint, QDebug &dbg,
                   int nbTab);
    void printTab(QDebug &dbg, int nbTab);
private:
    FilesManager* filesManager_;
    QHash<QString, bool> fileUsed;
    QHash<QString, DepenNode*> tmpDepenNodes_;
};

#endif // DEPENMANAGER_H
