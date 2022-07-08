#ifndef MAPCHOOSE_H
#define MAPCHOOSE_H

#include <QDialog>
#include"course.h"

namespace Ui {
class mapchoose;
}

class mapchoose : public QDialog
{
    Q_OBJECT

public:
    explicit mapchoose(Course a[],QWidget *parent = 0);
    ~mapchoose();

private:
    Ui::mapchoose *ui;
};

#endif // MAPCHOOSE_H
