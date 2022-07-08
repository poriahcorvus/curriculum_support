#ifndef MANAGER_H
#define MANAGER_H

#define SIMI 1
#define POPU 2
#define TIME 3
#include <QDialog>
#include"course.h"
#include"courseku.h"

namespace Ui {
class Manager;
}

class Manager : public QDialog
{
    Q_OBJECT

public:

    //当前课程总数
    int cur;

    Course sedcourse[50];
    explicit Manager(courseKu* a,Course* b,QWidget *parent = 0);
    void setcu(Course* source);
    void showresult(Course* result);
    void cclear(Course* source);
    void sort(Course* tosort,int left,int right,int type);
    int isbiger(Course a,Course b);
    void showall(courseKu* a);
    bool DeleteFile(QString &strPath);
    ~Manager();

private:
    Ui::Manager *ui;
};

#endif // MANAGER_H
