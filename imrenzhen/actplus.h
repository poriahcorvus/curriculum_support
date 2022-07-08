#ifndef ACTPLUS_H
#define ACTPLUS_H

#include <QDialog>
#include"actku.h"
#include"course.h"

namespace Ui {
class actPlus;
}

class actPlus : public QDialog
{
    Q_OBJECT

public:
    myActivity toplus;
    explicit actPlus(Course* a,actKu* activity,QDialog *parent = 0);
    ~actPlus();

private:
    Ui::actPlus *ui;
};

#endif // ACTPLUS_H
