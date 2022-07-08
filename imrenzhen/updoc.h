#ifndef UPDOC_H
#define UPDOC_H

#include <QWidget>
#include <QDialog>
#include "course.h"
#include "courseku.h"

namespace Ui {
class updoc;
}

class updoc : public QDialog
{
    Q_OBJECT

public:
    explicit updoc(courseKu *c, QDialog *parent = 0);
    ~updoc();
    void sendslot();
signals:
    void upret();
public slots:

private:
    Ui::updoc *ui;
};

#endif // UPDOC_H
