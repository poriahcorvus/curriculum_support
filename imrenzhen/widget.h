#ifndef WIDGET_H
#define WIDGET_H

#include <QDialog>
#include"courseku.h"
#include"timethread.h"
#include"clockthread.h"
#include"homeku.h"
#include"groupactku.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    courseKu *a;
    homeKu *b;
    groupActKu *c;
    ~Widget();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
