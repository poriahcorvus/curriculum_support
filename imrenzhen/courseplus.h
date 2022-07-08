#ifndef COURSEPLUS_H
#define COURSEPLUS_H

#include <QDialog>
#include"courseku.h"

namespace Ui {
class coursePlus;
}

class coursePlus : public QDialog
{
    Q_OBJECT

public:
    explicit coursePlus(courseKu* thecourse,QWidget *parent = 0);
    ~coursePlus();

private:
    Ui::coursePlus *ui;
};

#endif // COURSEPLUS_H
