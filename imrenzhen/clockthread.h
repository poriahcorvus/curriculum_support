#ifndef CLOCKTHREAD_H
#define CLOCKTHREAD_H

#include<QThread>

class clockThread:public QThread
{
public:
    void run();
};

#endif // CLOCKTHREAD_H
