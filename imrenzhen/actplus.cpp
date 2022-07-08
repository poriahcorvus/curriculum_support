#include "actplus.h"
#include "ui_actplus.h"
#include<QPushButton>
#include<QString>
#include<QDebug>
#include"quanju.h"
#include<QMessageBox>
#include<QString>
#include<QTextStream>
#include<QFile>
#include<QTextCodec>
#include<QDateTime>
#include<QDate>
#include<QTime>

actPlus::actPlus(Course* a,actKu* activity,QDialog *parent) :
    QDialog(parent),
    ui(new Ui::actPlus)
{
    ui->setupUi(this);

    ui->week->clear();
    QStringList plusweek;
    plusweek <<"星期一" <<"星期二"<<"星期三"<<"星期四"<<"星期五"<<"星期六"<<"星期七";
    ui->week->addItems(plusweek);
    toplus.type = 1;

    connect(ui->onlyonce,&QRadioButton::clicked,[=](){
        toplus.frequen = 1;
    });
    connect(ui->perweek,&QRadioButton::clicked,[=](){
        toplus.frequen = 2;
    });
    connect(ui->perday,&QRadioButton::clicked,[=](){
        toplus.frequen = 3;
    });


    connect(ui->finished,&QPushButton::clicked,[=](){

    toplus.Aweek = ui->week->currentIndex() + 1;
    toplus.Ahour = ui->timeEdit->time().hour();
    toplus.Amin = ui->timeEdit->time().minute();
    toplus.content = ui->lineEdit->text();

    QString chongtu = "";
    bool ok;
    int i;
    //时间冲突提醒
    //课程时间冲突
    for(i = 0; i < *allcourse; i++){
        if(a[i].cWeek.toInt(&ok,10) == toplus.Aweek)
               if(a[i].cEndHour.toInt(&ok,10) >= toplus.Ahour && a[i].cEndMin.toInt(&ok,10) > toplus.Amin && a[i].cStartHour.toInt(&ok,10) <= toplus.Ahour && a[i].cStartMin.toInt(&ok,10) < toplus.Amin)
                 chongtu += a[i].cName + " ";
    }

    //个人活动时间冲突
    i = 0;
    while(activity->allact[toplus.Aweek - 1][i].content != ""){
        if(activity->allact[toplus.Aweek - 1][i].Ahour == toplus.Ahour && activity->allact[toplus.Aweek - 1][i].Amin == toplus.Amin)
        chongtu += activity->allact[toplus.Aweek - 1][i].content + " ";
        i++;
    }

    //集体活动时间冲突
    i = 0;
    while(activity->allact[toplus.Aweek + 6][i].content != ""){
        if(activity->allact[toplus.Aweek + 6][i].Ahour == toplus.Ahour && activity->allact[toplus.Aweek + 6][i].Amin == toplus.Amin)
        chongtu += activity->allact[toplus.Aweek + 6][i].content + " ";
        i++;
    }

    for(i = 0; i < 14; i++){
        int j = 0;
        while(activity->allact[i][j].content != ""){
            if(activity->allact[i][j].frequen == 1 || activity->allact[i][j].frequen == 2){
                if(activity->allact[i][j].Aweek == toplus.Aweek && activity->allact[i][j].Ahour == toplus.Ahour && activity->allact[i][j].Amin == toplus.Amin)
                    chongtu += activity->allact[i][j].content;
            }else{
                if(activity->allact[i][j].Ahour == toplus.Ahour && activity->allact[i][j].Amin == toplus.Amin)
                    chongtu += activity->allact[i][j].content;
            }
            j++;
        }
    }

    if(chongtu != ""){
        if(QMessageBox::Ok == QMessageBox::information(this,"时间冲突","请注意：\n您当前添加的活动与" + chongtu +" 有时间冲突！",QMessageBox::Ok|QMessageBox::Close))
        {
            i = 0;
            while(activity->allact[toplus.Aweek-1][i].content != "")
                i++;
            activity->allact[toplus.Aweek-1][i] = toplus;
                activity->alltheact ++;

                QFile op3("D:\\dataexample\\op.txt");
                op3.open(QIODevice::Append);
                QTextStream opp3(&op3);
                opp3.setCodec(QTextCodec::codecForName("utf-8"));
                QDateTime dateTime3(QDateTime::currentDateTime());
                QString oppp3 = dateTime3.toString("yyyy-MM-dd hh:mm:ss ddd");
                oppp3 += " ";
                oppp3 += "学生学号：" + *xuehao + "  添加活动：" + toplus.content;
                opp3 << oppp3 << endl;
                op3.close();

        }
    }else{
        i = 0;
        while(activity->allact[toplus.Aweek-1][i].content != "")
            i++;
        activity->allact[toplus.Aweek-1][i] = toplus;
            activity->alltheact ++;

            QFile op3("D:\\dataexample\\op.txt");
            op3.open(QIODevice::Append);
            QTextStream opp3(&op3);
            opp3.setCodec(QTextCodec::codecForName("utf-8"));
            QDateTime dateTime3(QDateTime::currentDateTime());
            QString oppp3 = dateTime3.toString("yyyy-MM-dd hh:mm:ss ddd");
            oppp3 += " ";
            oppp3 += "学生学号：" + *xuehao + "  添加活动：" + toplus.content;
            opp3 << oppp3 << endl;
            op3.close();
    }

        this->close();
    });
}

actPlus::~actPlus()
{
    delete ui;
}
