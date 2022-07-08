#ifndef CLOCKOUT_H
#define CLOCKOUT_H

#include <QDialog>
#include<QString>

namespace Ui {
class clockOUt;
}

class clockOUt : public QDialog
{
    Q_OBJECT

public:
    explicit clockOUt(QDialog *parent = 0);
    ~clockOUt();

private:
    Ui::clockOUt *ui;
};

#endif // CLOCKOUT_H
