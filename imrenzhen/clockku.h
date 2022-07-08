#ifndef CLOCKKU_H
#define CLOCKKU_H

#include"myclock.h"

class clockKu
{
public:

    myClock clock[51];
    clockKu(QString path);

    QString cut(QString result);
};

#endif // CLOCKKU_H
