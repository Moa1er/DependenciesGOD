#ifndef FILEDATA_H
#define FILEDATA_H

#include <QObject>

class FileData : QObject
{
    Q_OBJECT
//METHODS
public:
     FileData(QString filePath);
     QStringList& getDependencies(){return fileDependencies_;}
     QString getFilePath(){return filePath_;}
private:
     void findDependencies();
     QString extractDependencyFromStr(QString lineRead);
//ATTRIBUTES
public:
    QList<bool> isExternDepen_;
private:
    QString filePath_;
    QStringList fileDependencies_;
};

#endif // FILEDATA_H
