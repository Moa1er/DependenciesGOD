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

signals:
    void needTreeViewUpdate(QString fileName, QStringList& dependencies);
};

#endif // FILESMANAGER_H
