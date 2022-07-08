#ifndef PLUSGROUPACT_H
#define PLUSGROUPACT_H

#include <QDialog>
#include"quanju.h"
#include"groupactku.h"

namespace Ui {
class plusgroupAct;
}

class plusgroupAct : public QDialog
{
    Q_OBJECT

public:
    explicit plusgroupAct(groupActKu* group,QWidget *parent = 0);
    ~plusgroupAct();

private:
    Ui::plusgroupAct *ui;
};

#endif // PLUSGROUPACT_H
