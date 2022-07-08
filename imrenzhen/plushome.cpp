#include "plushome.h"
#include "ui_plushome.h"
#include"quanju.h"
#include<QTextStream>
#include<QFile>
#include<QTextCodec>
#include<QDateTime>
#include<QDate>
#include<QTime>
#include<QString>

Plushome::Plushome(courseKu* course,homeKu* nowhome,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Plushome)
{
    ui->setupUi(this);

    //放置combobox
    QMap<QString,QString>COU;
    for(int i = 0; i < *coursenumber; i++)
        COU.insert(course->all[i].cName,course->all[i].cNum);
    ui->num->clear();
    foreach(const QString &str, COU.keys())
        ui->num->addItem(str,COU.value(str));


    for(int i = 1; i < 17; i++)
        ui->howweek->addItem(QString::number(i));

    ui->week->addItem("星期一");
    ui->week->addItem("星期二");
    ui->week->addItem("星期三");
    ui->week->addItem("星期四");
    ui->week->addItem("星期五");
    ui->week->addItem("星期六");
    ui->week->addItem("星期七");

    connect(ui->finish,&QPushButton::clicked,[=](){

        Manahome p;
        p.cu_num = COU.value(ui->num->currentText());

        p.homo_name = ui->name->text();
        p.ddl_howweek = ui->howweek->currentIndex() + 1;

        p.ddl_week = ui->week->currentIndex() + 1;
        p.ddl_hour = ui->time->time().hour();
        p.ddl_min = ui->time->time().minute();
        p.homo_desc = ui->desc->text();        

        QFile op2("D:\\dataexample\\op.txt");
        op2.open(QIODevice::Append);
        QTextStream opp2(&op2);
        opp2.setCodec(QTextCodec::codecForName("utf-8"));
        QDateTime dateTime2(QDateTime::currentDateTime());
        QString oppp2 = dateTime2.toString("yyyy-MM-dd hh:mm:ss ddd");
        oppp2 += " ";
        oppp2 += "管理员端 添加作业 课程编号为：" + p.cu_num + " 作业名称为:" + p.homo_name;
        opp2 << oppp2 << endl;
        op2.close();


        nowhome->allhomo[*allwork] = p;
        (*allwork) += 1;

        this->close();
    });

}

Plushome::~Plushome()
{
    delete ui;
}
