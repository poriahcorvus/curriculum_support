#include "mapchoose.h"
#include "ui_mapchoose.h"
#include<QComboBox>
#include"quanju.h"
#include"navdialog.h"
#include<QDebug>
#include<QTextCodec>
#include<QDateTime>
#include<QDate>
#include<QTime>
mapchoose::mapchoose(Course a[],QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mapchoose)
{
    ui->setupUi(this);

    //放置combobox
    //放置课程
    QMap<QString,int>CURCOU;
    bool ok;
    for(int i = 0; i < *allcourse ; i++)
        CURCOU.insert(a[i].cName,a[i].cPlace.toInt(&ok,10));
    foreach (const QString &str, CURCOU.keys())
        ui->course->addItem(str,CURCOU.value(str));

    //放置起点
    QFile startfile("D:\\dataexample\\skplacestart.txt");
    if(!startfile.open(QIODevice::ReadOnly))
        qDebug()<<startfile.errorString();
    QTextStream TS_startfile(&startfile);
    TS_startfile.setCodec("UTF-8");
    int nodenum;
    TS_startfile >> nodenum;
    for(int count_node=1;count_node<=nodenum;count_node++)
    {
        QString str;
        TS_startfile >> str;
        if(count_node!=1) ui->start->addItem(str);
    }
    //放置终点
    //自动显示到当前课程的地点
    QMap<QString,int>PLACE;
    int i = 0;
    while(i < 163){
        if(Whereplace[i] != "")
            PLACE.insert(Whereplace[i],i);
        i++;
    }

    foreach (const QString &str, PLACE.keys())
        ui->end->addItem(str,PLACE.value(str));


    connect(ui->course,&QComboBox::currentTextChanged,[=](){
        int place = CURCOU.value(ui->course->currentText());
        ui->end->setCurrentText(PLACE.key(place));
    });

    //放置交通方式
    //获取数据

    connect(ui->finish,&QPushButton::clicked,[=](){
        navdialog nav(ui->start->currentIndex()+1,CURCOU.value(ui->course->currentText()),ui->way->currentIndex());
        nav.exec();
        this->close();
        QFile op("d:\\dataexample\\op.txt");
        op.open(QIODevice::Append);
        QTextStream opp(&op);
        opp.setCodec(QTextCodec::codecForName("utf-8"));
        QString s_course=ui->course->currentText();
        QString s_place=ui->end->currentText();
        QString s_start=ui->start->currentText();
        QString s_Way=ui->way->currentText();
        QDateTime dateTime(QDateTime::currentDateTime());
        QString opr="操作时间：";
        QString timestr=dateTime.toString("yyyy-MM-dd hh:mm:ss ddd");
        QString oprr="学生学号：";
        opp <<opr<<timestr << oprr << *xuehao <<"\n";
        QString  aa= "进入导航界面";
        QString bb="用户选择课程为：";
        QString cc="用户选择课程的上课位置为：";
        QString dd="用户选择的起点为：";
        QString ee="用户选择的出行方式为：";
        opp << aa <<"\n";
        opp << bb << s_course <<"\n";
        opp << cc << s_place<<"\n";
        opp<< dd << s_start<<"\n";
        opp<< ee << s_Way<<"\n";
        op.close();
    });




}

mapchoose::~mapchoose()
{
    delete ui;
}
