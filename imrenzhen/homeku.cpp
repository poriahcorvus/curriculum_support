#include "homeku.h"
#include<QFile>
#include"quanju.h"
#include<QDebug>

homeKu::homeKu()
{
    allwork = new int;
    *allwork = 0;
    bool ok;
    //准备数据
    QFile file("D:\\dataexample\\course\\allHome.txt");
    file.open(QIODevice::ReadOnly);

    while(!file.atEnd()){
        QString result = file.readLine();
        allhomo[*allwork].cu_num = cut(result);
        allhomo[*allwork].homo_name = cut(result);
        allhomo[*allwork].ddl_howweek = cut(result).toInt(&ok,10);
        allhomo[*allwork].ddl_week = cut(result).toInt(&ok,10);
        allhomo[*allwork].ddl_hour = cut(result).toInt(&ok,10);
        allhomo[*allwork].ddl_min = cut(result).toInt(&ok,10);
        allhomo[*allwork].homo_desc = cut(result);
        (*allwork) += 1;
    }

}

QList<Manahome> homeKu::thehomo(QString num){
    QList<Manahome> result;
    for(int i = 0; i < *allwork; i++){
        if(allhomo[i].cu_num == num)
            result << allhomo[i];
    }
    return result;
}

QString homeKu::cut(QString result){
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
