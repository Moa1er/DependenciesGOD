#ifndef DEPENMANAGER_H
#define DEPENMANAGER_H

#include "depennode.h"

#include "filesmanager.h"

//useful here for mainwindow.h
#define COLOR_PROJECT_FILE "#FF00FF"
#define COLOR_EXTERNAL_FILE "#808000"

class DepenManager
{
//METHODS
public:
    DepenManager(FilesManager* fileManager);
    void buildTree();
    DepenNode* getNodeAlrdyProcessed(QString nodeToFind);
private:
    void makeDepen(DepenNode* node);
    void regroupExternalDepen(DepenNode* node);
    QString findDepenFullPath(QString depenName);
    DepenNode* findNode(QString nodeToFind, DepenNode* rootNode);

    //functions to print in terminal
    void printTree(DepenNode* nodeToPrint, QDebug &dbg,
                   int nbTab);
    void printTab(QDebug &dbg, int nbTab);
//ATTRIBUTES
public:
    //useful for debug
    DepenNode* treeOfDepen_;
    QHash<QString, DepenNode*> partialDepenNodes_;
private:
    FilesManager* filesManager_;
    QHash<QString, bool> fileProcessed_;
};

#endif // DEPENMANAGER_H
