#include "studentwidget.h"
#include "ui_studentwidget.h"
#include"showcu.h"
#include"manager.h"
#include"clockthread.h"
#include"clockmanager.h"
#include<QString>
#include<QFile>
#include<QDebug>
#include<QPushButton>
#include"quanju.h"
#include<qtablewidget.h>
#include<QTimer>
#include"actmanager.h"
#include<QByteArray>
#include <QTextStream>
#include"mapchoose.h"
#include<QTextCodec>
#include<QDateTime>
#include<QDate>
#include<QTime>
#include<QDir>

studentWidget::studentWidget(QString Path,courseKu* c,homeKu* allhomework,groupActKu* groupact,QDialog *parent) :
    QDialog(parent),
    ui(new Ui::studentWidget)
{
    ui->setupUi(this);
    *cPath = Path;

    *base = *nowbase;

    //现在的时间
    now = new QTimer(this);
    now->setSingleShot(false);
    now->start(*base);

    connect(now,&QTimer::timeout,[=](){
        QString week = "";
        QString hour = QString::number(curHour,10);
        QString min = "";
        switch (curWeek) {
        case 1:
           week = "一";
            break;
        case 2:
           week = "二";
            break;
        case 3:
           week = "三";
            break;
        case 4:
           week = "四";
            break;
        case 5:
           week = "五";
            break;
        case 6:
           week = "六";
            break;
        default:
            week = "七";
            break;
        }
        if(curMin < 10)
            min = "0" + QString::number(curMin,10);
        else min = QString::number(curMin,10);
        ui->curWeek->setText(week);
        ui->curHour->setText(hour);
        ui->curMin->setText(min);
        ui->nowweek->setText(QString::number(allweek));
    });
    //实现按钮的加速和减速
    connect(ui->speedup,&QPushButton::clicked,[=](){
        if(*base > 299){
            *base -= 100;
            now->start(*base);
        }else if(*base > 99){
            *base -= 50;
            now->start(*base);
        }else if(*base > 29){
            *base -= 20;
            now->start(*base);
        }
    });
    connect(ui->speeddown,&QPushButton::clicked,[=](){
            *base += 50;
            now->start(*base);
    });
    //实现时间的启动与暂停
    connect(ui->stopbtn,&QPushButton::clicked,[=](){
        *nowbase = *base;
        *base = 0;
        now->start(*base);
    });
    connect(ui->startbtn,&QPushButton::clicked,[=](){
        *base = *nowbase;
        now->start(*base);
    });

    //准备学生自己的课程数组
    path = Path;
    QString mycu;
    int sizecu,place;

    allcourse = new int;
    *allcourse = 0;

    QFile file(path + "\\data_course.txt");
    file.open(QIODevice::ReadOnly);
        while(!file.atEnd()){
            mycu = file.readLine();
            sizecu = mycu.size();
            mycu.remove(sizecu-2,2);
            place = c->search(mycu,NUM);
            if(place != -1){
                cou[*allcourse] = c->num[place];
                *(allcourse) += 1;
            }
        }
        file.close();

    //课程界面
    //放置信息
    setcu(cou);

    //实现点击弹出课程详细信息
    connect(ui->cu_tableWidget,&QTableWidget::doubleClicked,[=](){

        QTableWidgetItem *p = ui->cu_tableWidget->currentItem();
        QString thezifuc = p->text();
        QString worked = "";
        int i = 0;
        while(thezifuc[i] != "\n"){
            worked += thezifuc[i];
            i++;
        }
        //通过名字寻找对应课程
        int place = 0;
        while(cou[place].cName != worked)
            place++;
        Course result = cou[place];
        ShowCu cuw(result,this);
        cuw.exec();
    });

    //实现管理按钮功能
    connect(ui->CmanagerBtn,&QPushButton::clicked,[=](){
        Manager manager(c,cou,this);
        manager.exec();

        setcu(cou);
    });

    //活动界面
    //显示当前活动
    activity = new actKu(Path,groupact);
    setact(activity);

    //实现管理按钮功能
    connect(ui->actmanbtn,&QPushButton::clicked,[=](){
        actmanager manageract(cou,activity,this);
        manageract.exec();
        setact(activity);
    });
    //实现排序功能
    connect(ui->sort,&QPushButton::clicked,[=](){
        sort(activity);
        setact(activity);
    });

    //闹钟界面
    //显示闹钟
    showclock(naozhong);

    //闹钟删除按钮
    connect(ui->clockdeletebtn,&QPushButton::clicked,[=](){
        int x = ui->time_tableWidget->currentRow();
        QString con = ui->time_tableWidget->item(x,2)->text();

        QFile op3("D:\\dataexample\\op.txt");
        op3.open(QIODevice::Append);
        QTextStream opp3(&op3);
        opp3.setCodec(QTextCodec::codecForName("utf-8"));
        QDateTime dateTime3(QDateTime::currentDateTime());
        QString oppp3 = dateTime3.toString("yyyy-MM-dd hh:mm:ss ddd");
        oppp3 += " ";
        oppp3 += "学生学号：" + *xuehao + "  删除闹钟：" + con;
        opp3 << oppp3 << endl;
        op3.close();

        for(int i = 0; i < *allclock; i++){
            if(naozhong->clock[i].Ccontent == con){
                for(; i <*allclock; i++)
                    naozhong->clock[i] = naozhong->clock[i+1];
                *(allclock) -= 1;
                break;
            }
        }
        showclock(naozhong);
    });

    //闹钟添加按钮
    connect(ui->clockplusbtn,&QPushButton::clicked,[=](){
        clockmanager cloman(activity,this);
        cloman.exec();
        showclock(naozhong);
    });

    //作业和资料
    connect(ui->ChomeworkBtn, &QPushButton::clicked, this, [=]()
    {
        homework hpage(cou, allhomework);
        hpage.exec();
    });
    connect(ui->CsourceBtn, &QPushButton::clicked, this, [=](){
        document dpage(c);
        dpage.exec();
    });

    //闹钟排序按钮
    connect(ui->timesort,&QPushButton::clicked,[=](){
        clocksort(naozhong);
        showclock(naozhong);
    });

    //导航界面
    connect(ui->mapButton,&QPushButton::clicked,[=](){
        QFile op("d:\\dataexample\\op.txt");
        op.open(QIODevice::Append);
        QTextStream opp(&op);
        QDateTime dateTime(QDateTime::currentDateTime());
        QString opr="操作时间：";
        QString timestr=dateTime.toString("yyyy-MM-dd hh:mm:ss ddd");
        QString oppr="学生学号：";
        opp <<opr<<timestr << " " << oppr << *xuehao <<"\n";
        QString hh="进入课程导航\n";
        opp << hh;
        op.close();
        opp.setCodec(QTextCodec::codecForName("utf-8"));
        mapchoose map(cou);
        map.exec();

    });

}

void studentWidget::setcu(Course* source){
    //清除表格
    ui->cu_tableWidget->clear();

    //先准备table表格
    QStringList row;
    row <<"8:00-8:45"<<"8:50-9:35"<<"9:50-10:35"<<"10:40-11:25"<<"11:30-12:15"<<"13:00-13:45"<<"13:50-14:35"<<"14:45-15:30"<<"15:40-16:25"<<"16:35-17:20"<<"17:25-18:10";
    ui->cu_tableWidget->setColumnCount(5);
    ui->cu_tableWidget->setHorizontalHeaderLabels(QStringList() <<"星期一"<<"星期二"<<"星期三"<<"星期四"<<"星期五");
    ui->cu_tableWidget->setRowCount(11);
    ui->cu_tableWidget->setVerticalHeaderLabels(row);
    ui->cu_tableWidget->horizontalHeader()->setMinimumSectionSize(100);
    ui->cu_tableWidget->verticalHeader()->setMinimumSectionSize(80);

    int xs,xe,y;
    bool ok;
    QString cutable = "";
    ui->cu_tableWidget->setWordWrap(true);

    for(int i = 0 ; i < *allcourse; i++){
        cutable += source[i].cName + "\n\n";
        cutable += where(source[i].cPlace.toInt(&ok,10)) + source[i].cDetail;

        switch(source[i].cWeek.toInt(&ok,10)){
        case 1:
            y = 0;
            break;
        case 2:
            y = 1;
            break;
        case 3:
            y = 2;
            break;
        case 4:
            y = 3;
            break;
        default:
            y = 4;
            break;
        }

        switch (source[i].cStartHour.toInt(&ok,10)){
        case 8:
            xs = 0;
            break;
        case 9:
            xs = 2;
            break;
        case 13:
            xs = 5;
            break;
        case 14:
            xs = 7;
            break;
        default:
            xs = 8;
            break;
        }

        switch (source[i].cEndHour.toInt(&ok,10)) {
        case 9:
            xe = 1;
            break;
        case 11:
            xe = 3;
            break;
        case 12:
            xe = 4;
            break;
        case 14:
            xe = 6;
            break;
        case 15:
            xe = 7;
            break;
        case 16:
            xe = 8;
            break;
        default:
            xe = 10;
            break;
        }

        for(int j = xs; j <= xe; j++){

            if(! ui->cu_tableWidget->item(j,y))
                ui->cu_tableWidget->setItem(j,y,new QTableWidgetItem(cutable));
            else{
                QString toset = ui->cu_tableWidget->item(j,y)->text();
                toset += "\n----------\n";
                toset += cutable;
                ui->cu_tableWidget->item(j,y)->setText(toset);
            }
        }
        cutable = "";
    }
    ui->cu_tableWidget->resizeRowsToContents();
    ui->cu_tableWidget->resizeColumnsToContents();
}

void studentWidget::setact(actKu* activity){

    ui->act_tableWidget->setColumnCount(4);
    ui->act_tableWidget->setRowCount(activity->alltheact);
    ui->act_tableWidget->setHorizontalHeaderLabels(QStringList()<< "活动类型" <<"活动频率" <<"活动时间" <<"活动内容");

    int x = 0, y = 0;
    QString typ,fre,tim,con;
    for(int i = 0; i < 14; i++){
        int j = 0;
        while(activity->allact[i][j].content != ""){
                if(i < 7)
                  typ = "个人";
                  else typ = "集体";
                switch (activity->allact[i][j].frequen){
                    case 1:
                        fre = "仅一次";
                        break;
                    case 2:
                        fre = "每周一次";
                        break;
                    default:
                        fre = "每天一次";
                        break;
             }
                tim = "星期";
                switch (activity->allact[i][j].Aweek) {
                case 1:
                    tim += "一";
                    break;
                case 2:
                    tim += "二";
                    break;
                case 3:
                    tim += "三";
                    break;
                case 4:
                    tim += "四";
                    break;
                case 5:
                    tim += "五";
                    break;
                case 6:
                    tim += "六";
                    break;
                default:
                    tim += "七";
                    break;
                }
                tim += " " + QString::number(activity->allact[i][j].Ahour) + ":";
        if(activity->allact[i][j].Amin< 10)
                 tim += "0" + QString::number(activity->allact[i][j].Amin);
        else tim += QString::number(activity->allact[i][j].Amin);
                 con = activity->allact[i][j].content;
                 ui->act_tableWidget->setItem(x,y++,new QTableWidgetItem(typ));
                 ui->act_tableWidget->setItem(x,y++,new QTableWidgetItem(fre));
                 ui->act_tableWidget->setItem(x,y++,new QTableWidgetItem(tim));
                 ui->act_tableWidget->setItem(x,y++,new QTableWidgetItem(con));
                 x++;
                 y = 0;
            j++;
        }
    }

    ui->act_tableWidget->verticalHeader()->setMinimumSectionSize(70);
    ui->act_tableWidget->horizontalHeader()->setMinimumSectionSize(100);
    ui->act_tableWidget->resizeColumnsToContents();
    ui->act_tableWidget->resizeRowsToContents();
}

void studentWidget::sort(actKu* activity){
    myActivity ii;
    for(int i = 0; i < 14; i++){
        int j = 0,ss;
        while(activity->allact[i][j].content != ""){
            for(int q = 0; q < j; q++){
                if(activity->allact[i][q].frequen > activity->allact[i][j].frequen){
                 ii = activity->allact[i][j];
                 for(ss = j; ss > q;ss--)
                     activity->allact[i][ss] = activity->allact[i][ss-1];
                 activity->allact[i][ss] = ii;
                 break;
                }
            }
            j++;
        }

        j = 1;
        int x = 0;
        while(activity->allact[i][j+1].content != ""){
            if(activity->allact[i][j].frequen == activity->allact[i][j-1].frequen){
              for(int k = x; k < j; k++){
                     if(activity->allact[i][k].Aweek > activity->allact[i][j].Aweek){
                         ii = activity->allact[i][j];
                         for(ss = j; ss > k; ss--)
                             activity->allact[i][ss] = activity->allact[i][ss-1];
                         activity->allact[i][ss] = ii;
                     }else if(activity->allact[i][k].Aweek == activity->allact[i][j].Aweek){
                         if(activity->allact[i][k].Ahour > activity->allact[i][j].Ahour){
                             ii = activity->allact[i][j];
                             for(ss = j; ss > k; ss--)
                                 activity->allact[i][ss] = activity->allact[i][ss-1];
                             activity->allact[i][ss] = ii;
                         }else if(activity->allact[i][k].Ahour == activity->allact[i][j].Ahour){
                         if(activity->allact[i][k].Amin > activity->allact[i][j].Amin){
                             ii = activity->allact[i][j];
                             for(ss = j; ss > k; ss--)
                                 activity->allact[i][ss] = activity->allact[i][ss-1];
                             activity->allact[i][ss] = ii;
                            }
                         }
                      }
                   }
                }else x = j;
        j++;
    }
  }
}

void studentWidget::showclock(clockKu* clock){

    ui->time_tableWidget->setColumnCount(3);
    ui->time_tableWidget->setRowCount(*allclock);
    ui->time_tableWidget->setHorizontalHeaderLabels(QStringList()<< "闹钟频率" << "闹钟时间" << "闹钟内容");

    for(int i = 0; i < *allclock; i++){
        int y = 0;
        QString fre,tim,con;
        switch (clock->clock[i].Cfrequen){
            case 1:
                fre = "仅一次";
                break;
            case 2:
                fre = "每周一次";
                break;
            default:
                fre = "每天一次";
                break;
     }
        tim = "星期";
        switch (clock->clock[i].Cweek) {
        case 1:
            tim += "一";
            break;
        case 2:
            tim += "二";
            break;
        case 3:
            tim += "三";
            break;
        case 4:
            tim += "四";
            break;
        case 5:
            tim += "五";
            break;
        case 6:
            tim += "六";
            break;
        default:
            tim += "七";
            break;
        }
        tim += " " + QString::number(clock->clock[i].Chour) + ":";
        if(clock->clock[i].Cmin< 10)
             tim += "0" + QString::number(clock->clock[i].Cmin);
        else tim += QString::number(clock->clock[i].Cmin);
             con = clock->clock[i].Ccontent;

        ui->time_tableWidget->setItem(i,y++,new QTableWidgetItem(fre));
        ui->time_tableWidget->setItem(i,y++,new QTableWidgetItem(tim));
        ui->time_tableWidget->setItem(i,y++,new QTableWidgetItem(con));

        y = 0;
    }

    ui->time_tableWidget->verticalHeader()->setMinimumSectionSize(70);
    ui->time_tableWidget->horizontalHeader()->setMinimumSectionSize(100);
    ui->time_tableWidget->resizeColumnsToContents();
    ui->time_tableWidget->resizeRowsToContents();
}

void studentWidget::clocksort(clockKu* naozhong){

    myClock ii;

        int j = 0,ss;
        for(j = 0; j < *allclock; j++){
            for(int q = 0; q < j; q++){
                if(naozhong->clock[q].Cfrequen > naozhong->clock[j].Cfrequen){
                    ii = naozhong->clock[j];
                    for(ss = j; ss > q; ss--)
                        naozhong->clock[ss] = naozhong->clock[ss-1];
                    naozhong->clock[ss] = ii;
                    break;
                }
            }
        }

     j = 1;
     int x = 0;
     for(j = 1;j < *allclock ;j++){
         if(naozhong->clock[j-1].Cfrequen == naozhong->clock[j].Cfrequen){
           for(int k = x; k < j; k++){
                  if(naozhong->clock[k].Cweek > naozhong->clock[j].Cweek){
                      ii = naozhong->clock[j];
                      for(ss = j; ss > k; ss--)
                          naozhong->clock[ss] = naozhong->clock[ss-1];
                      naozhong->clock[ss] = ii;
                  }else if(naozhong->clock[k].Cweek == naozhong->clock[j].Cweek){
                      if(naozhong->clock[k].Chour > naozhong->clock[j].Chour){
                          ii = naozhong->clock[j];
                          for(ss = j; ss > k; ss--)
                              naozhong->clock[ss] = naozhong->clock[ss-1];
                          naozhong->clock[ss] = ii;
                      }else if(naozhong->clock[k].Chour == naozhong->clock[j].Chour){
                      if(naozhong->clock[k].Cmin > naozhong->clock[j].Cmin){
                          ii = naozhong->clock[j];
                          for(ss = j; ss > k; ss--)
                              naozhong->clock[ss] = naozhong->clock[ss-1];
                          naozhong->clock[ss] = ii;
                         }
                      }
                   }
                }
             }else x = j;
   }
}

studentWidget::~studentWidget()
{
    delete ui;

    //写回数组
    QString toput;

    //写回课程数组
    QFile file(path + "\\data_course.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream txtoutput(&file);
    txtoutput.setCodec("utf-8");
    int i = 0;
    for(int i = 0; i < *allcourse; i++)
        txtoutput << cou[i].cNum << endl;
    file.close();

    //写回活动数组
    QFile actfile(path + "\\data_act.txt");
    actfile.open(QIODevice::WriteOnly | QIODevice::Text);

   QTextStream acttxtoutput(&actfile);
   acttxtoutput.setCodec("utf-8");
    for(i = 0; i < 7; i++){
        int j = 0;
        while(activity->allact[i][j].content != ""){
           toput = "";
           toput += QString::number(activity->allact[i][j].type) + " ";
           toput += QString::number(activity->allact[i][j].frequen) + " ";
           toput += QString::number(activity->allact[i][j].Aweek) + " ";
           toput += QString::number(activity->allact[i][j].Ahour) + " ";
           toput += QString::number(activity->allact[i][j].Amin) + " ";
           toput += activity->allact[i][j].content;
           acttxtoutput << toput << endl;
            j++;
        }

    }
    actfile.close();

    //写回闹钟数组
    QFile clockfile(path + "\\data_clock.txt");
    clockfile.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream clockput(&clockfile);
    clockput.setCodec("utf-8");
    for(i = 0; i < *allclock; i++){
        toput = "";
        toput += QString::number(naozhong->clock[i].Cfrequen) + " ";
        toput += QString::number(naozhong->clock[i].Cweek) + " ";
        toput += QString::number(naozhong->clock[i].Chour) + " ";
        toput += QString::number(naozhong->clock[i].Cmin) + " ";
        toput += naozhong->clock[i].Ccontent;
        clockput << toput << endl;
    }
    clockfile.close();

    delete now;
    delete activity;
    delete allcourse;
}

