#ifndef HOMEWORK_H
#define HOMEWORK_H

#include <QDialog>
#include "course.h"
#include "courseku.h"
#include"homeku.h"

namespace Ui {
class homework;
}

class homework : public QDialog
{
    Q_OBJECT

public:
    explicit homework(Course cou[], homeKu *allhomework, QDialog *parent = 0);
    //homework(Course cou[], QWidget *parent = 0);
    ~homework();
    //void setCou(Course c[]);
    void sendslot();
    void showtable(Course cou[], QVector<QString>tnumber, QVector<QString>tname, QVector<QString>tddl, QVector<QString>hnumber, QVector<QString>hname, QVector<QString>htime);
    void showhad(Course cou[], QVector<QString>hnumber, QVector<QString>hname, QVector<QString>htime);
    void showto(Course cou[], QVector<QString>tnumber, QVector<QString>tname, QVector<QString>tddl);
    void sortnumber(QVector<QString>&tnumber, QVector<QString>&tname, QVector<QString>&ttime);
signals:
    void hret();
public slots:
/*
private slots:
    void getData(Course*);
*/

private:
    Ui::homework *ui;
    //Course cou[20];
};

#endif // HOMEWORK_H
