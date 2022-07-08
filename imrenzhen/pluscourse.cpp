#include "pluscourse.h"
#include "ui_pluscourse.h"

pluscourse::pluscourse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pluscourse)
{
    ui->setupUi(this);
}

pluscourse::~pluscourse()
{
    delete ui;
}
