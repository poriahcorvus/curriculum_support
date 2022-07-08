#ifndef COURSEKU_H
#define COURSEKU_H

#define NAME 1
#define NUM 2
#include"course.h"

class courseKu
{
public:

    //课程库数组
    Course num[100];
    Course name[100];
    Course all[50];
    Course mh[51];

    //在构造函数中要把这几个都准备好
    courseKu();

    //两个哈希函数 一个模糊搜索函数
    int Hash(QString tocu);
    void Muhu(QString tofd,Course* mh);
    QString cut(QString result);
    int search(QString tofd,int type);
    QString change(Course a);
    int Contain(const char* a,const char*b);
    int Strlen(const char* p);
    int howsimi(const char* foun,const char* cond);

};

#endif // COURSEKU_H
