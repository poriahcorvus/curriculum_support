#ifndef COURSE_H
#define COURSE_H

#include<QString>

class Course
{
public:

    QString cName,cNum,cGroup;
    QString cPlace;
    QString cDetail;
    QString cPopular;

    QString kPlace;
    QString kTime;
    QString kDetail;

    QString cWeek;
    QString cStartHour;
    QString cEndHour;
    QString cStartMin;
    QString cEndMin;
    QString teacher;

    int similar;
};

#endif // COURSE_H
