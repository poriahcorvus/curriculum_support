#include "showcu.h"
#include "ui_showcu.h"
#include"quanju.h"

ShowCu::ShowCu(Course a,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowCu)
{
    ui->setupUi(this);
    bool ok;
    ui->Name->setText(a.cName);
    ui->Num->setText(a.cNum);
    ui->cGroup->setText(a.cGroup);
    ui->teacher->setText(a.teacher);

    QString result = where(a.cPlace.toInt(&ok,10));
    result += a.cDetail;
    ui->classPlace->setText(result);

    result = a.cStartHour + ":";
    if(a.cStartMin.toInt(&ok,10) < 10)
        result += "0" + a.cStartMin;
    else result += a.cStartMin;

    result += "-" + a.cEndHour + ":";
    if(a.cEndMin.toInt(&ok,10) < 10)
        result += "0" + a.cEndMin;
    else result += a.cEndMin;

    ui->classTime->setText(result);

    result = where(a.kPlace.toInt(&ok,10));
    result += a.kDetail;

    ui->kPlace->setText(result);
    result = a.kTime;
    ui->kTime->setText(result);
}

ShowCu::~ShowCu()
{
    delete ui;
}
