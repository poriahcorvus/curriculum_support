/*
author : LNY && poria && LYF
*/

#include"quanju.h"
#include "widget.h"
#include"timethread.h"
#include"clockthread.h"
#include <QApplication>
#include<QObject>
#include<QDebug>
#include <QTextCodec>

/*
闹钟频率（1仅一次 2每周一次 3每天一次） 闹钟开始提醒星期 闹钟小时 闹钟分钟 闹钟内容
活动类型（1个人 2集体） 活动频率 活动时间（星期 小时 分 活动内容
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(code);

    timeThread s;
    s.start();

    clockThread ss;
    ss.start();

    Widget w;
    w.show();

    return a.exec();
}
