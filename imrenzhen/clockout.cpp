#include "clockout.h"
#include "ui_clockout.h"


clockOUt::clockOUt(QString toset,QDialog *parent) :
    QDialog(parent),
    ui(new Ui::clockOUt)
{
    ui->setupUi(this);

    ui->text->setText(toset);
}

clockOUt::~clockOUt()
{
    delete ui;
}
