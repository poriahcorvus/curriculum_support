#ifndef MANAHOME_H
#define MANAHOME_H

#include <QObject>
#include<QString>

class Manahome
{
public:
    //课程编号
    QString cu_num;
    //作业名称
    QString homo_name;
    //截止时间
    int ddl_howweek;
    int ddl_week;
    int ddl_hour;
    int ddl_min;
    //备注
    QString homo_desc;

};

#endif // MANAHOME_H
