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
        fileUsed_[key] = false;
    }

    for(auto const& [key, val] : filesManager_->files_){
        if(fileUsed_[key]){
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
    treeOfDepen_ = partialDepenNodes_["C:/Github/LabeoTechGithubs/AwakeQt/mainwindow.h"];
    regroupExternalDepen(treeOfDepen_);

//    treeOfDepen = tmpDepenNodes_["C:/Github/LabeoTechGithubs/AwakeQt/qt-breakpad/breakpad/src/processor/stackwalker_arm.h"];
//    regroupExternalDepen(treeOfDepen);

    foreach(DepenNode* node, partialDepenNodes_){
        regroupExternalDepen(node);
    }

    bool test = false;
}

void DepenManager::makeDepen(DepenNode* node){
    if(filesManager_->files_.find(node->depenName_) == filesManager_->files_.end()){
        return;
    }
    QStringList dependencies = filesManager_->files_[node->depenName_]->getDependencies();
    fileUsed_[node->depenName_] = true;
    const QStringList fileUsedKeys = fileUsed_.keys();
    for(int i = 0; i < dependencies.size(); i++){
        QVector<QString> pathfilesSameName;
        for(int j = 0; j < fileUsedKeys.size(); j++){
            //TODO bug where fileUsedKeys has some dependencies with no path..
            //this is a temporary fix:
            if(fileUsedKeys[j] == dependencies[i] || !fileUsedKeys[j].contains(filesManager_->dirWithFiles_)){
                continue;
            }
            QString fileNameFromFilePath = QFileInfo(fileUsedKeys[j]).fileName();
            QString fileNameFromDepen = QFileInfo(dependencies[i]).fileName();
            if(fileNameFromFilePath == fileNameFromDepen){
                pathfilesSameName.push_back(fileUsedKeys[j]);
            }
        }
        if(pathfilesSameName.size() > 1){
            bool test = false;
        }
        QString actualDepenFullPath = "";
        for(int j = 0; j < pathfilesSameName.size(); j++){
            if(pathfilesSameName[j].contains(dependencies[i])){
                actualDepenFullPath = pathfilesSameName[j];
                break;
            }
        }
        if(node->depenName_ == "C:/Github/ioi_acq/eventslistdialog.h"){
            QList<bool> test = filesManager_->files_[node->depenName_]->isExternDepen_;

        }
        if(fileUsedKeys.contains(actualDepenFullPath) && !filesManager_->files_[node->depenName_]->isExternDepen_[i]){
            if(fileUsed_[actualDepenFullPath]){
                //we get the node that has already been created
                //we add it as a depen to the node we are processing
                node->childDepen_.push_back(getNodeAlrdyProcessed(actualDepenFullPath));
                //If the node was in the map of the tmpDepenNodes then we erase it from the map
                if(partialDepenNodes_.contains(actualDepenFullPath)){
                    partialDepenNodes_.remove(actualDepenFullPath);
                }
                continue;
            }
            node->childDepen_.push_back(new DepenNode(actualDepenFullPath, false, QColor(COLOR_PROJECT_FILE)));
            makeDepen(node->childDepen_[node->childDepen_.size() - 1]);
        }else{
            node->childDepen_.push_back(new DepenNode(dependencies[i], true, QColor(COLOR_PROJECT_FILE)));
            makeDepen(node->childDepen_[node->childDepen_.size() - 1]);
        }
    }
}

void DepenManager::regroupExternalDepen(DepenNode* node){
    if(node == nullptr){
        return;
    }
    DepenNode* externDepenNode = nullptr;
    for(int i = 0; i < node->childDepen_.size(); i++){
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
    for(const QString& key : keys){
        node = findNode(nodeToFind, partialDepenNodes_[key]);
        if(node != nullptr){
            return node;
        }
    }
    return nullptr;
}

DepenNode* DepenManager::findNode(QString nodeToFind, DepenNode* rootNode){
    if(rootNode == nullptr || rootNode->isExternDepen_){
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
