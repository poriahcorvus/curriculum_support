#include "plusgroupact.h"
#include "ui_plusgroupact.h"
#include<QTextStream>
#include<QFile>
#include<QTextCodec>
#include<QDateTime>
#include<QDate>
#include<QTime>
#include<QString>
#include<QTextStream>

plusgroupAct::plusgroupAct(groupActKu* group,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::plusgroupAct)
{
    ui->setupUi(this);

    //放置combobox
    ui->week->addItem("星期一");
    ui->week->addItem("星期二");
    ui->week->addItem("星期三");
    ui->week->addItem("星期四");
    ui->week->addItem("星期五");
    ui->week->addItem("星期六");
    ui->week->addItem("星期七");

    ui->frequency->addItem("仅一次");
    ui->frequency->addItem("每周一次");
    ui->frequency->addItem("每天一次");

    ui->classroom->addItem("7");
    ui->classroom->addItem("8");

    connect(ui->finish,&QPushButton::clicked,[=](){

        groupAct p;
        p.type = 2;
        p.content = ui->content->text();
        p.classroom = ui->classroom->currentIndex() + 7;
        p.frequence = ui->frequency->currentIndex() + 1;
        p.week = ui->week->currentIndex() + 1;
        p.hour = ui->time->time().hour();
        p.min = ui->time->time().minute();

        QFile op3("D:\\dataexample\\op.txt");
        op3.open(QIODevice::Append);
        QTextStream opp3(&op3);
        opp3.setCodec(QTextCodec::codecForName("utf-8"));
        QDateTime dateTime3(QDateTime::currentDateTime());
        QString oppp3 = dateTime3.toString("yyyy-MM-dd hh:mm:ss ddd");
        oppp3 += " ";
        oppp3 += "管理员端 添加集体活动 班级为：" + QString::number(p.classroom) + " 活动内容为: " + p.content;
        opp3 << oppp3 << endl;
        op3.close();

        //添加
        group->gAct[*allgroupAct] = p;
        (*allgroupAct) += 1;

        this->close();

    });

}

plusgroupAct::~plusgroupAct()
{
    delete ui;
}
