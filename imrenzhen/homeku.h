#ifndef HOMEKU_H
#define HOMEKU_H

#include <QObject>
#include "manahome.h"

class homeKu
{
public:

    //作业数组
    Manahome allhomo[100];

    //返回某课程编号的所有作业
    QList<Manahome> thehomo(QString num);
    QString cut(QString result);

    homeKu();
};

#endif // HOMEKU_H
