#ifndef STUDENTWIDGET_H
#define STUDENTWIDGET_H

#include <QDialog>
#include"course.h"
#include"courseku.h"
#include"myactivity.h"
#include"actku.h"
#include"clockku.h"
#include"timethread.h"
#include"clockthread.h"
#include"homeku.h"
#include"groupactku.h"
#include"homework.h"
#include"document.h"

namespace Ui {
class studentWidget;
}

class studentWidget : public QDialog
{
    Q_OBJECT

public:
    //学生课程数组

    Course cou[20];
    QString path;
    actKu* activity;
    QTimer *now;

    explicit studentWidget(QString name,courseKu* c,homeKu* b,groupActKu* act,QDialog *parent = 0);
    void sort(actKu* activity);
    void showclock(clockKu* clock);
    void setcu(Course* source);
    void setact(actKu* activity);
    void clocksort(clockKu* naozhong);
    ~studentWidget();

private:
    Ui::studentWidget *ui;
};

#endif // STUDENTWIDGET_H
