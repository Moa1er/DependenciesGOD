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
    if(filePath_.contains("gflags.h")){
        bool few = false;
    }
    QFile file(filePath_);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "fileNotOpen in FileData::findDependencies()";
        return;
    }
    QString fileName = QFileInfo(filePath_).fileName();
    QString lineRead = "";

    //TODO maybe some processing to not read all the lines of the file ?
    while(!file.atEnd()){
        if(!lineRead.contains("#include") || (!lineRead.contains("<") && !lineRead.contains("\""))){
            lineRead = file.readLine();
            continue;
        }

        QString dependency = extractDependencyFromStr(lineRead);
        if(dependency != "" && fileName != dependency){
            fileDependencies_.append(dependency);
        }else{
            isExternDepen_.removeLast();
        }
        lineRead = file.readLine();
    }
    file.close();
}

QString FileData::extractDependencyFromStr(QString lineRead){
    int idxInclude = lineRead.indexOf("#include");
    //9 is the number of char in "#include"
    lineRead.remove(0, idxInclude + 8);
    int idxStartDepen = lineRead.indexOf("<");
    //checking if < is in the string
    if(idxStartDepen == -1){
        //checking if " is in the string
        idxStartDepen = lineRead.indexOf("\"");
        if(idxStartDepen == -1){
            isExternDepen_.append(true);
            return "Dependency not readable";
        }else{
            isExternDepen_.append(false);
        }
    }else{
        isExternDepen_.append(true);
    }
    int idxEndDepen = -1;
    if(isExternDepen_.last()){
        idxEndDepen = lineRead.indexOf(">", idxStartDepen + 1);
   }else{
        idxEndDepen = lineRead.indexOf("\"", idxStartDepen + 1);
    }
    lineRead = lineRead.mid(idxStartDepen + 1, idxEndDepen - idxStartDepen - 1);

    return lineRead;
}
