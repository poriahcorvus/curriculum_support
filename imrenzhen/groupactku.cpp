#include "groupactku.h"
#include<QString>
#include<QFile>
#include<QDebug>
#include"quanju.h"
groupActKu::groupActKu()
{   
    allgroupAct = new int;
    *allgroupAct = 0;
    //准备数组
    QFile file("D:\\dataexample\\course\\allgroupact.txt");
    file.open(QIODevice::ReadOnly);

   groupAct p;
   bool ok;

   while(!file.atEnd()){
       QString result = file.readLine();
       p.type = 2;
       p.classroom = cut(result).toInt(&ok,10);
       p.frequence = cut(result).toInt(&ok,10);
       p.week = cut(result).toInt(&ok,10);
       p.hour = cut(result).toInt(&ok,10);
       p.min = cut(result).toInt(&ok,10);
       p.content = cut(result);
       gAct[*allgroupAct] = p;
       (*allgroupAct) += 1;
   }
}

QString groupActKu::cut(QString result){

    QString s = "";
    static int m = 0;

    while(result[m] != " "  && result[m] != "\r"){
        s += result[m];
        m++;
    }
    m++;

    if(result[m-1] == "\r")
        m = 0;

    return s;
}

