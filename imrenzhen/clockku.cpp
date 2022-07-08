#include "clockku.h"
#include"quanju.h"
#include<QString>
#include<QFile>
#include<QDebug>

clockKu::clockKu(QString path)
{
    QString Path = path + "\\data_clock.txt";
    QFile file(Path);
    file.open(QIODevice::ReadOnly);

    allclock = new int;
    *allclock = 0;

    bool ok;
    QString result;
    QString ss;
    while(!file.atEnd()){
        result = file.readLine();
        ss = cut(result);
        clock[*allclock].Cfrequen = ss.toInt(&ok,10);
        ss = cut(result);
        clock[*allclock].Cweek = ss.toInt(&ok,10);
        ss = cut(result);
        clock[*allclock].Chour = ss.toInt(&ok,10);
        ss = cut(result);
        clock[*allclock].Cmin = ss.toInt(&ok,10);
        clock[*allclock].Ccontent = cut(result);
        clock[*allclock].Cisuse = 0;
        *(allclock) += 1;
    }

    file.close();
}

QString clockKu::cut(QString result){
    QString s = "";
    static int m = 0;

    while(result[m] != " " && result[m] != "\r"){
        s += result[m];
        m++;
    }
    m++;

    if(result[m-1] == "\r")
        m = 0;
    return s;
}
