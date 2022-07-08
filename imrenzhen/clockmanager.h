#ifndef CLOCKMANAGER_H
#define CLOCKMANAGER_H

#include <QDialog>
#include"actku.h"

namespace Ui {
class clockmanager;
}

class clockmanager : public QDialog
{
    Q_OBJECT

public:
    explicit clockmanager(actKu* activity,QDialog *parent = 0);
    void setact(actKu* activity);
    QString cut(QString tocut);
    ~clockmanager();

private:
    Ui::clockmanager *ui;
};

#endif // CLOCKMANAGER_H
