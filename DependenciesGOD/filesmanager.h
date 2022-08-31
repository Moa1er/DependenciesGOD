#ifndef FILESMANAGER_H
#define FILESMANAGER_H

#include <QObject>
#include <QHash>
#include <QHashFunctions>

#include "filedata.h"

class FilesManager : public QObject
{
    Q_OBJECT
public:
    FilesManager();
    void addFilesAndInit(QString dirWithFiles);

public:
    std::unordered_map<QString, std::unique_ptr<FileData>> files_;
    //<nameOfFile, <vectorIfSameNameFile<File>>
//    std::unordered_map<QString, QVector<std::unique_ptr<FileData>>> files_;
    QString dirWithFiles_;

signals:
    void needTreeViewUpdate(QString fileName, QStringList& dependencies);

};

#endif // FILESMANAGER_H
