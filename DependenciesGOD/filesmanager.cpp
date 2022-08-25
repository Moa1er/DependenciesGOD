#include "filesmanager.h"

#include <QDebug>
#include <QDir>

FilesManager::FilesManager(){}

void FilesManager::addFilesAndInit(QString dirWithFiles){
    QDir rootDir(dirWithFiles);
    QStringList filesList = rootDir.entryList(
                QStringList() << "*.hpp" << "*.cpp" << "*.h" << "*.c",
                QDir::Files | QDir::NoDotAndDotDot);
    foreach(QString fileName, filesList){
        QString filePath = rootDir.absolutePath() + "/" + fileName;
        files_[fileName] = std::make_unique<FileData>(filePath);
        emit needTreeViewUpdate(fileName, files_[fileName]->getDependencies());
    }
}
