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
        if(key.contains(".cpp")){
            continue;
        }
        fileUsed[key] = false;
    }

    for(auto const& [key, val] : filesManager_->files_){
        if(fileUsed[key] || key.contains(".cpp")){
            continue;
        }
        tmpDepenNodes_[key] = new DepenNode(key, QColor(COLOR_PROJECT_FILE));
        makeDepen(tmpDepenNodes_[key]);
    }

    //Prints the resulting tree
//    QStringList keys = tmpDepenNodes_.keys();
//    for(const QString& key : keys){
//        QDebug dbg(QtDebugMsg);
//        printTree(tmpDepenNodes_[key], dbg, 0);
//    }

    //TODO remove later
//    treeOfDepen = tmpDepenNodes_["mainwindow.h"];
//    regroupExternalDepen(treeOfDepen);

    foreach(DepenNode* node, tmpDepenNodes_){
        regroupExternalDepen(node);
    }

    bool test = false;
}

void DepenManager::makeDepen(DepenNode* node){
    if(filesManager_->files_.find(node->depenName_) == filesManager_->files_.end()){
        return;
    }
    QStringList dependencies = filesManager_->files_[node->depenName_]->getDependencies();
    fileUsed[node->depenName_] = true;
    for(int i = 0; i < dependencies.size(); i++){
        if(fileUsed[dependencies[i]]){
            //we get the node that has already been created
            //we add it as a depen to the node we are processing
            node->childDepen_.push_back(getNodeAlrdyProcessed(dependencies[i]));
            //If the node was in the map of the tmpDepenNodes then we erase it from the map
            if(tmpDepenNodes_.contains(dependencies[i])){
                tmpDepenNodes_.remove(dependencies[i]);
            }
            continue;
        }
        node->childDepen_.push_back(new DepenNode(dependencies[i], QColor(COLOR_PROJECT_FILE)));
        makeDepen(node->childDepen_[node->childDepen_.size() - 1]);
    }
}

void DepenManager::regroupExternalDepen(DepenNode* node){
    DepenNode* externDepenNode = nullptr;
    if(node->depenName_ == "ios_exception_minidump_generator.h"){
        bool test = false;
    }
    for(int i = 0; i < node->childDepen_.size(); i++){
//        if(node->childDepen_[i]->depenName_ == "ios_exception_minidump_generator.h"){
//            bool test = false;
//        }
        QString fileName = QFileInfo(node->childDepen_[i]->depenName_).fileName();
        bool dependencyByLocalPath = false;
        if(fileName != node->childDepen_[i]->depenName_){
            dependencyByLocalPath = true;
        }
        auto fileDataIt = filesManager_->files_.find(fileName);
        if(fileDataIt != filesManager_->files_.end() && fileDataIt->second->getFilePath().contains(node->childDepen_[i]->depenName_)){
//            if(dependencyByLocalPath)
//                if(!fileDataIt->second->getFilePath().contains(node->childDepen_[i]->depenName_)){
//                    regroupExternalDepen(node->childDepen_[i]);
//                    continue;
//                }
//            }else{
                regroupExternalDepen(node->childDepen_[i]);
                continue;
//            }
        }
        if(externDepenNode == nullptr){
            externDepenNode = new DepenNode(node->childDepen_[i]->depenName_, COLOR_EXTERNAL_FILE);
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
    QStringList keys = tmpDepenNodes_.keys();
    DepenNode* node = nullptr;
    for(const QString& key : keys){
        node = findNode(nodeToFind, tmpDepenNodes_[key]);
        if(node != nullptr){
            return node;
        }
    }
    return nullptr;
}

DepenNode* DepenManager::findNode(QString nodeToFind, DepenNode* rootNode){
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
