#include "msgbox.h"
#include"quanju.h"
#include<QDebug>

msgBox::msgBox(QObject *parent):QObject (parent)
{

}

void msgBox::readyShow()
{
    this->moveToThread(qApp->thread());
    QTimer::singleShot(0,this,SLOT(onShow()));
}

msgBox::msgBox(const QString &title, const QString &msg,const int type):title(title),msg(msg),type(type)
{

}

void msgBox::show(const QString &title, const QString &msg,const int type)
{
    QEventLoop eventLoop;
    auto messageBox = new msgBox(title,msg,type);
    connect(messageBox,SIGNAL(destroyed()),&eventLoop,SLOT(quit()));
    messageBox->readyShow();
    eventLoop.exec();
}

void msgBox::onShow()
{
    switch (type)
    {
    case 1:
    {
        QMessageBox::information(NULL,title,msg,QString::fromLocal8Bit("ok"));
             *base = *nowbase;
    }
        break;
    case 2:
        QMessageBox::critical(NULL,title,msg,QString::fromLocal8Bit("ok"));
        break;
    case 3:
        QMessageBox::question(NULL,title,msg,QString::fromLocal8Bit("ok"));
        break;
    }


    this->deleteLater();
}
