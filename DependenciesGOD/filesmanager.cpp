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
        files_[filePath] = std::make_unique<FileData>(filePath);
        emit needTreeViewUpdate(fileName, files_[filePath]->getDependencies());
    }
    QStringList folderList = rootDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QString folder, folderList){
        QString folderPath = rootDir.absolutePath() + "/" + folder;
        addFilesAndInit(folderPath);
    }
}
