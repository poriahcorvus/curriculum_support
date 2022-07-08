#ifndef TIMETHREAD_H
#define TIMETHREAD_H

#include<QThread>

class timeThread:public QThread
{
public:
    void run();
};

#endif // TIMETHREAD_H
