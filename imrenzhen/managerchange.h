#ifndef MANAGERCHANGE_H
#define MANAGERCHANGE_H

#include <QDialog>
#include"courseku.h"
#include"homeku.h"
#include"plushome.h"
#include"groupactku.h"

namespace Ui {
class managerchange;
}

class managerchange : public QDialog
{
    Q_OBJECT

public:

    courseKu* course;
    homeKu* hwork;
    groupActKu* groupa;

    explicit managerchange(courseKu* a,homeKu* homework,groupActKu* groupact,QWidget *parent = 0);
    void showNowCourse(courseKu* thecourse);
    void showhomo(homeKu* homo,courseKu* thecourse);
    void showgroupact(groupActKu* groupact);
    void soort(courseKu* tosort,int left,int right,int type);
    void hosort(homeKu* work,int left,int right);
    int isbiger(Course a,Course b);
    bool DeleteFile(QString &strPath);
    ~managerchange();

private:
    Ui::managerchange *ui;
};

#endif // MANAGERCHANGE_H
