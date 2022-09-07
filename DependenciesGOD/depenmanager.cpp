#include "depenmanager.h"

#include "filesmanager.h"

#include <QDebug>
#include <QFileInfo>

DepenManager::DepenManager(FilesManager* filesManager)
{
    filesManager_ = filesManager;

    //TREE EXAMPLE
//    treeOfDepen = new DepenNode("level0");
//    treeOfDepen->childDepen_.push_back(new DepenNode("level1_1"));
//    treeOfDepen->childDepen_.push_back(new DepenNode("level1_2"));
//    treeOfDepen->childDepen_[0]->childDepen_.push_back(new DepenNode("level2_0_1"));
//    treeOfDepen->childDepen_[0]->childDepen_.push_back(new DepenNode("level2_0_2"));
//    treeOfDepen->childDepen_[1]->childDepen_.push_back(new DepenNode("level2_1_1"));
//    treeOfDepen->childDepen_[0]->childDepen_[0]->childDepen_.push_back(new DepenNode("level3_0_1"));
//    QDebug dbg(QtDebugMsg);
//    printTree(treeOfDepen, dbg, 0);
}

void DepenManager::buildTree(){
    if(filesManager_->files_.size() <= 0){
        return;
    }

    //we fill up the map fileUsed to not have undefined values
    //TODO kind of a waste of time maybe try to do that differently
    for(auto const& [key, val] : filesManager_->files_){
        fileProcessed_[key] = false;
    }

    for(auto const& [key, val] : filesManager_->files_){
        if(fileProcessed_[key]){
            continue;
        }
        partialDepenNodes_[key] = new DepenNode(key, false, QColor(COLOR_PROJECT_FILE));
        makeDepen(partialDepenNodes_[key]);
    }

    //Prints the resulting tree
//    QStringList keys = tmpDepenNodes_.keys();
//    for(const QString& key : keys){
//        QDebug dbg(QtDebugMsg);
//        printTree(tmpDepenNodes_[key], dbg, 0);
//    }

    //TODO remove later
//    treeOfDepen_ = partialDepenNodes_["C:/Github/LabeoTechGithubs/AwakeQt/mainwindow.h"];
//    regroupExternalDepen(treeOfDepen_);

//    treeOfDepen = tmpDepenNodes_["C:/Github/LabeoTechGithubs/AwakeQt/qt-breakpad/breakpad/src/processor/stackwalker_arm.h"];
//    regroupExternalDepen(treeOfDepen);

    foreach(DepenNode* node, partialDepenNodes_){
        regroupExternalDepen(node);
    }

    bool test = false;
}

void DepenManager::makeDepen(DepenNode* node){
    if(node == nullptr){
        qDebug() << "node nullptr in DepenManager::makeDepen";
        return;
    }
    //if node externalDepen then we don't want to process the depen
    if(filesManager_->files_.find(node->depenName_) == filesManager_->files_.end()){
        return;
    }
    if(node->depenName_ == "C:/github/AwakeQt/qt-breakpad/breakpad/src/third_party/curl/curl.h"){
        qDebug() << "curl processed";
    }
    fileProcessed_[node->depenName_] = true;
    QStringList dependencies = filesManager_->files_[node->depenName_]->getDependencies();
    const QStringList fileUsedKeys = fileProcessed_.keys();

    for(int i = 0; i < dependencies.size(); i++){
        QString depenFullPath = findDepenFullPath(dependencies[i]);
        if(node->depenName_ == "C:/github/AwakeQt/qt-breakpad/breakpad/src/third_party/curl/mprintf.h"){
            QList<bool> test = filesManager_->files_[node->depenName_]->isExternDepen_;
        }
        //if the path is contained in the usedMap and the dependency isn't extern to the project
        if(fileUsedKeys.contains(depenFullPath) && !filesManager_->files_[node->depenName_]->isExternDepen_[i]){
            //if the file has already been processed
            if(fileProcessed_[depenFullPath]){
                //we get the node that has already been created
                //we add it as a depen to the node we are processing
                node->childDepen_.push_back(getNodeAlrdyProcessed(depenFullPath));
                //If the node was in the map of the tmpDepenNodes then we erase it from the map
                if(partialDepenNodes_.contains(depenFullPath)){
                    partialDepenNodes_.remove(depenFullPath);
                }
            }else{
                node->childDepen_.push_back(new DepenNode(depenFullPath, false, QColor(COLOR_PROJECT_FILE)));
                makeDepen(node->childDepen_[node->childDepen_.size() - 1]);
            }
        }else{
            node->childDepen_.push_back(new DepenNode(dependencies[i], true, QColor(COLOR_PROJECT_FILE)));
        }
    }
    bool end = false;
}

QString DepenManager::findDepenFullPath(QString depenName){
    const QStringList fileUsedKeys = fileProcessed_.keys();
    QVector<QString> pathfilesSameName;
    //we get all the paths that contains the ONLY fileName of the dependency
    for(int j = 0; j < fileUsedKeys.size(); j++){
        //TODO bug where fileUsedKeys has some dependencies with no path..
        //this is a temporary fix:
        if(fileUsedKeys[j] == depenName || !fileUsedKeys[j].contains(filesManager_->dirWithFiles_)){
            continue;
        }
        QString fileNameFromFilePath = QFileInfo(fileUsedKeys[j]).fileName();
        QString fileNameFromDepen = QFileInfo(depenName).fileName();
        if(fileNameFromFilePath == fileNameFromDepen){
            pathfilesSameName.push_back(fileUsedKeys[j]);
        }
    }
    if(pathfilesSameName.size() > 1){
        bool test = false;
    }

    QString actualDepenFullPath = "";
    //we get the supposedly unique path that contains the path AND the fileName of the dependency
    for(int j = 0; j < pathfilesSameName.size(); j++){
        if(pathfilesSameName[j].contains(depenName)){
            actualDepenFullPath = pathfilesSameName[j];
            break;
        }
    }
    return actualDepenFullPath;
}

void DepenManager::regroupExternalDepen(DepenNode* node){
    if(node == nullptr){
        qDebug() << "node nullptr in DepenManager::regroupExternalDepen";
        return;
    }

    DepenNode* externDepenNode = nullptr;
    for(int i = 0; i < node->childDepen_.size(); i++){
        //TODO mmmmm yeah sometimes it is nullptr for no apparent reason
        if(node->childDepen_[i] == nullptr){
            qDebug() << "node CHILDREN nullptr in DepenManager::regroupExternalDepen";
            continue;
        }
        if(!node->childDepen_[i]->isExternDepen_){
            regroupExternalDepen(node->childDepen_[i]);
            continue;
        }
        if(externDepenNode == nullptr){
            externDepenNode = new DepenNode(node->childDepen_[i]->depenName_, true, COLOR_EXTERNAL_FILE);
        }else{
            externDepenNode->setDepenName(externDepenNode->depenName_ + "\n" + node->childDepen_[i]->depenName_);
        }
        node->childDepen_.removeAt(i);
        i--;
    }

    if(externDepenNode != nullptr){
        node->childDepen_.push_back(externDepenNode);
    }
}

DepenNode* DepenManager::getNodeAlrdyProcessed(QString nodeToFind){
    QStringList keys = partialDepenNodes_.keys();
    DepenNode* node = nullptr;
    if(nodeToFind == "C:/github/AwakeQt/qt-breakpad/breakpad/src/third_party/curl/multi.h"){
        qDebug() << "Separator\n";
    }
    for(const QString& key : keys){
        node = findNode(nodeToFind, partialDepenNodes_[key]);
        if(node != nullptr){
            if(nodeToFind == "C:/github/AwakeQt/qt-breakpad/breakpad/src/third_party/curl/multi.h"){
                qDebug() << "key is: " << key;
            }
            return node;
        }
    }
    return nullptr;
}

DepenNode* DepenManager::findNode(QString nodeToFind, DepenNode* rootNode){
    if(rootNode == nullptr){
        qDebug() << "node nullptr in DepenManager::findNode";
        return nullptr;
    }

    if(rootNode->isExternDepen_){
        return nullptr;
    }

    if(rootNode->depenName_ == nodeToFind){
        return rootNode;
    }
    DepenNode* node = nullptr;
    for(DepenNode* depenNode : rootNode->childDepen_){
        node = findNode(nodeToFind, depenNode);
        if(node != nullptr){
            return node;
        }
    }
    return nullptr;
}

void DepenManager::printTree(DepenNode* nodeToPrint, QDebug &dbg, int nbTab){
    printTab(dbg, nbTab);
    dbg << "Depen name: " << nodeToPrint->depenName_ << Qt::endl;
    for(int i = 0; i < nodeToPrint->childDepen_.size(); i++){
       printTree(nodeToPrint->childDepen_[i], dbg, nbTab + 1);
    }
}

void DepenManager::printTab(QDebug &dbg, int nbTab){
    for(int i = 0; i < nbTab; i++){
        dbg << "    ";
    }
}
