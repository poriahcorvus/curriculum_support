#ifndef PLUSCOURSE_H
#define PLUSCOURSE_H

#include <QDialog>

namespace Ui {
class pluscourse;
}

class pluscourse : public QDialog
{
    Q_OBJECT

public:
    explicit pluscourse(QWidget *parent = 0);
    ~pluscourse();

private:
    Ui::pluscourse *ui;
};

#endif // PLUSCOURSE_H
