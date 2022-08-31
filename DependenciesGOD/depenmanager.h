#ifndef DEPENMANAGER_H
#define DEPENMANAGER_H

#include "depennode.h"

#include "filesmanager.h"

//useful here for mainwindow.h
#define COLOR_PROJECT_FILE "#FF00FF"
#define COLOR_EXTERNAL_FILE "#808000"

class DepenManager
{
public:
    DepenManager(FilesManager* fileManager);
    void buildTree();
public:
    DepenNode* treeOfDepen;
    QHash<QString, DepenNode*> tmpDepenNodes_;
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
};

#endif // DEPENMANAGER_H
