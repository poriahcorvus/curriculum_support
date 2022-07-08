#ifndef COURSECHANGE_H
#define COURSECHANGE_H

#include <QDialog>

namespace Ui {
class courseChange;
}

class courseChange : public QDialog
{
    Q_OBJECT

public:
    explicit courseChange(QWidget *parent = 0);
    ~courseChange();

private:
    Ui::courseChange *ui;
};

#endif // COURSECHANGE_H
