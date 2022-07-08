#ifndef PLUSHOME_H
#define PLUSHOME_H

#include <QDialog>
#include"homeku.h"
#include"courseku.h"

namespace Ui {
class Plushome;
}

class Plushome : public QDialog
{
    Q_OBJECT

public:
    explicit Plushome(courseKu* course,homeKu* nowhome,QWidget *parent = 0);
    ~Plushome();

private:
    Ui::Plushome *ui;
};

#endif // PLUSHOME_H
