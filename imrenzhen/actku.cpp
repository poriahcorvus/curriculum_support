#include "actku.h"
#include<QFile>
#include<QDebug>
#include"quanju.h"
actKu::actKu(QString Path,groupActKu* groupact)
{
    QString actpath = Path + "\\data_act.txt";
    QFile actfile(actpath);
    actfile.open(QIODevice::ReadOnly);
    bool ok;
    alltheact = 0;
    QString theact;
    myActivity act;
    QString tocut;

    //初始化
    while(!actfile.atEnd()){        
        theact = actfile.readLine();
        tocut = cut(theact);
        act.type = tocut.toInt(&ok,10);
        tocut = cut(theact);
        act.frequen = tocut.toInt(&ok,10);
        tocut = cut(theact);
        act.Aweek = tocut.toInt(&ok,10);
        tocut = cut(theact);
        act.Ahour = tocut.toInt(&ok,10);
        tocut = cut(theact);
        act.Amin = tocut.toInt(&ok,10);
        tocut = cut(theact);
        act.content = tocut;
        alltheact ++;

        int place;
        int y;
        if(act.type == 1){
           place = 0 + act.Aweek -1;
           for(y = 0; allact[place][y].content != ""; y++){
           }
            allact[place][y] = act;
        }else{
           place = 7 + act.Aweek -1;
           for(y = 0; allact[place][y].content != ""; y++){
            }
            allact[place][y] = act;
        }
    }

    //添加集体活动
    int theclass = Path.mid(23,1).toInt(&ok,10);
    for(int i = 0; i < *allgroupAct; i++){
        if(groupact->gAct[i].classroom == theclass){
            myActivity p;
            p.type = 2;
            p.frequen = groupact->gAct[i].frequence;
            p.content = groupact->gAct[i].content;
            p.Aweek = groupact->gAct[i].week;
            p.Ahour = groupact->gAct[i].hour;
            p.Amin = groupact->gAct[i].min;

            int x = groupact->gAct[i].week + 6;
            int j = 0;
            while(allact[x][j].content != "")
                j++;
            allact[x][j] = p;
         alltheact ++;
        }
    }

}
QString actKu::cut(QString tocut){

    QString s = "";
    static int m = 0;
    while(tocut[m] != " " && tocut[m] != "\r"){
        s += tocut[m];
        m++;
    }
    m++;

    if(tocut[m-1] == "\r")
        m = 0;

    return s;
}
