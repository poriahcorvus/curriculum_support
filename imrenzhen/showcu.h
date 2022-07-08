#ifndef SHOWCU_H
#define SHOWCU_H

#include <QDialog>
#include "course.h"

namespace Ui {
class ShowCu;
}

class ShowCu : public QDialog
{
    Q_OBJECT

public:
    explicit ShowCu(Course a,QWidget *parent = 0);
    ~ShowCu();

private:
    Ui::ShowCu *ui;
};

#endif // SHOWCU_H
