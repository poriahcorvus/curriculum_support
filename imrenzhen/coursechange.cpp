#include "coursechange.h"
#include "ui_coursechange.h"

courseChange::courseChange(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::courseChange)
{
    ui->setupUi(this);
}

courseChange::~courseChange()
{
    delete ui;
}
