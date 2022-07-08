#ifndef ACTKU_H
#define ACTKU_H

#include<QString>
#include"myactivity.h"
#include"actsuoyin.h"
#include"groupactku.h"

class actKu
{
public:

    //个人在前 群体在后
    int alltheact;
    myActivity allact[14][11];

    actKu(QString path,groupActKu* groupact);
    QString cut(QString tocut);
    void actsort(myActivity* acti);
};

#endif // ACTKU_H
