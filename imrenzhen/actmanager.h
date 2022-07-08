#ifndef ACTMANAGER_H
#define ACTMANAGER_H

#include <QDialog>
#include"actku.h"
#include"course.h"

namespace Ui {
class actmanager;
}

class actmanager : public QDialog
{
    Q_OBJECT

public:
    explicit actmanager(Course* a,actKu* activity,QDialog *parent);
    void showact(actKu* activity);
    ~actmanager();

private:
    Ui::actmanager *ui;
};

#endif // ACTMANAGER_H
