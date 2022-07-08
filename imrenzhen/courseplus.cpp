#include "courseplus.h"
#include "ui_courseplus.h"
#include <QComboBox>
#include <QMap>
#include"quanju.h"
#include<QDebug>
#include<QFile>
#include<QTextCodec>
#include<QDateTime>
#include<QDate>
#include<QTime>
#include<QString>
#include <QDir>


coursePlus::coursePlus(courseKu* thecourse,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::coursePlus)
{
    ui->setupUi(this);

    //放置combox
    ui->courseWeek->addItem("星期一");
    ui->courseWeek->addItem("星期二");
    ui->courseWeek->addItem("星期三");
    ui->courseWeek->addItem("星期四");
    ui->courseWeek->addItem("星期五");
    ui->courseWeek->addItem("星期六");
    ui->courseWeek->addItem("星期七");

    //上课和考试地点
    QMap<QString,int>CuPlace;
    CuPlace.insert("学生活动中心",19);
    CuPlace.insert("S1楼",31);
    CuPlace.insert("实验楼",33);
    CuPlace.insert("教学综合实验楼",34);
    CuPlace.insert("教四楼",92);
    CuPlace.insert("教三楼",100);
    CuPlace.insert("科研大楼西南门",121);
    CuPlace.insert("教一楼南门",144);
    CuPlace.insert("教二楼",149);
    CuPlace.insert("主楼西门",148);
    CuPlace.insert("科学会堂",159);
            ui->coursePlace->clear();
    foreach(const QString &str,CuPlace.keys())
            ui->coursePlace->addItem(str,CuPlace.value(str));

            ui->kPlace->clear();
    foreach(const QString &str,CuPlace.keys())
            ui->kPlace->addItem(str,CuPlace.value(str));

    //完成
    connect(ui->finish,&QPushButton::clicked,[=](){

        Course p;
        p.cName = ui->name->text();
        p.cNum = ui->num->text();
        p.teacher = ui->teacher->text();
        p.cGroup = ui->group->text();
        p.kTime = ui->ktime->text();
        p.cWeek = QString::number(ui->courseWeek->currentIndex() + 1);       
        p.cStartHour = QString::number(ui->start->time().hour());
        p.cStartMin = QString::number(ui->start->time().minute());
        p.cEndHour = QString::number(ui->end->time().hour());
        p.cEndMin = QString::number(ui->end->time().minute());
        QString pl = ui->coursePlace->currentText();

        p.cPlace = QString::number(CuPlace.value(pl));

        p.cDetail = ui->coursedetile->text();
        QString cl = ui->kPlace->currentText();

        p.kPlace = QString::number(CuPlace.value(cl));

        p.kDetail = ui->kPlaceDetile->text();
        p.cPopular = QString::number(qrand()%999);
        p.similar = 0;

        //添加
        int ii = *coursenumber;
        thecourse->all[ii] = p;
        (*coursenumber) += 1;

        QFile op("D:\\dataexample\\op.txt");
        op.open(QIODevice::Append);
        QTextStream opp(&op);
        opp.setCodec(QTextCodec::codecForName("utf-8"));
        QDateTime dateTime(QDateTime::currentDateTime());
        QString oppp = dateTime.toString("yyyy-MM-dd hh:mm:ss ddd");
        oppp += " ";
        oppp += "管理员端 添加课程 " + p.cName;
        opp << oppp << endl;
        op.close();

        int pplace = thecourse->Hash(p.cName);
        if(thecourse->name[pplace].cNum == "")
            thecourse->name[pplace] = p;
        else{
            while(thecourse->name[pplace].cNum != ""){
                pplace++;
                pplace = pplace%100;
            }
            thecourse->name[pplace] = p;
        }

        pplace = thecourse->Hash(p.cNum);
        if(thecourse->num[pplace].cNum == "")
            thecourse->num[pplace] = p;
        else{
            while(thecourse->num[pplace].cNum != ""){
                pplace++;
                pplace = pplace%100;
            }
            thecourse->num[pplace] = p;
        }

        //建立对应文件夹
        QString dir_str = "D:\\dataexample\\course\\" + p.cNum;
        QDir dir;
        if(!dir.exists(dir_str)){
            bool res = dir.mkpath(dir_str);
        }





        this->close();
    });


}

coursePlus::~coursePlus()
{
    delete ui;
}
