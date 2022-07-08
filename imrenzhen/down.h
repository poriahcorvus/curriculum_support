#ifndef DOWN_H
#define DOWN_H

#include <QWidget>
#include <QDialog>
#include "course.h"
#include "courseku.h"

namespace Ui {
class down;
}

class down : public QDialog
{
    Q_OBJECT

public:
    explicit down(courseKu *c, QDialog *parent = 0);
    ~down();
    void sendslot();
signals:
    void downret();
public slots:

private:
    Ui::down *ui;
};

#endif // DOWN_H
