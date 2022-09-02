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
    //while we didn't find any include we continue to read until the end
//    while(!lineRead.contains("#include") && !file.atEnd()){
//        lineRead = file.readLine();
//    }
    //if line contains "(" it means we are in a decleration of function
    //or processing of some sort
    //This condition means that if we have includes after a "(" it will not find it..
    //TODO maybe find something else
    while(/*!lineRead.contains("(") &&*/ !file.atEnd()){
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
//    //Gets rid of #include and spaces
//    lineRead.remove("#include").remove(" ");
//    //Gets rid of "\n" and "\r"
//    lineRead.remove("\n").remove("\r");
//    if(lineRead.size() < 1){
//        return "";
//    }

//    //Gets read of the "" or the <> around the dependency
//    if(lineRead[0] == '<'){
//        isExternDepen_.append(true);
//    }else if(lineRead[0] == '\"'){
//        isExternDepen_.append(false);
//    }else{
//        isExternDepen_.append(false);
//        return "";
//    }
//    lineRead.remove("\"").remove("<").remove(">");

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
