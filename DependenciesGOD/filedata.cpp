#include "filedata.h"

#include <QFile>
#include <QDebug>
#include <QFileInfo>

FileData::FileData(QString filePath)
{
    filePath_ = filePath;
    findDependencies();
}

void FileData::findDependencies(){
    QFile file(filePath_);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "fileNotOpen in FileData::findDependencies()";
        return;
    }
    QString fileName = QFileInfo(filePath_).fileName();
    QString lineRead = "";
    //while we didn't find any include we continue to read until the end
    while(!lineRead.contains("#include") && !file.atEnd()){
        lineRead = file.readLine();
    }
    //if line contains "(" it means we are in a decleration of function
    //or processing of some sort
    //This condition means that if we have includes after a "(" it will not find it..
    //TODO maybe find something else
    while(!lineRead.contains("(") && !file.atEnd()){
        if(!lineRead.contains("#include")){
            lineRead = file.readLine();
            continue;
        }

        QString dependency = extractDependencyFromStr(lineRead);
        if(dependency != "" && fileName != dependency){
            if(dependency.contains("client_info.h")){
                bool test = true;
            }
            fileDependencies_.append(dependency);
        }
        lineRead = file.readLine();
    }
    file.close();
}

QString FileData::extractDependencyFromStr(QString lineRead){
    //Gets rid of #include and spaces
    lineRead.remove("#include").remove(" ");
    //Gets rid of "\n" and "\r"
    lineRead.remove("\n").remove("\r");
    if(lineRead.size() < 1){
        return "";
    }

    //Gets read of the "" or the <> around the dependency
    lineRead.remove(0, 1);
    lineRead.remove(lineRead.size()-1, 1);

    return lineRead;
}
