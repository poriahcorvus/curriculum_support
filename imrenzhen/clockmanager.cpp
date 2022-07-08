#include "clockmanager.h"
#include "ui_clockmanager.h"
#include"myclock.h"
#include"quanju.h"
#include<QDebug>
#include<QString>
#include<QTextStream>
#include<QFile>
#include<QTextCodec>
#include<QDateTime>
#include<QDate>
#include<QTime>

clockmanager::clockmanager(actKu* activity,QDialog *parent) :
    QDialog(parent),
    ui(new Ui::clockmanager)
{
    ui->setupUi(this);
    setact(activity);

    connect(ui->finished,&QPushButton::clicked,[=](){
        int x = ui->tableWidget->currentRow();
        QString fre,tim,con;
        fre = ui->tableWidget->item(x,1)->text();
        con = ui->tableWidget->item(x,3)->text();
        tim = ui->tableWidget->item(x,2)->text();

        myClock tmp;
        if(fre == "仅一次")
            tmp.Cfrequen = 1;
        else if(fre == "每周一次")
            tmp.Cfrequen = 2;
        else tmp.Cfrequen = 3;
        tmp.Ccontent = con;
        tmp.Cisuse = 0;

        QString re = cut(tim);
        if(re == "星期一"){
            tmp.Cweek = 1;
        }else if(re == "星期二"){
            tmp.Cweek = 2;
        }else if(re == "星期三"){
            tmp.Cweek = 3;
        }else if(re == "星期四"){
            tmp.Cweek = 4;
        }else if(re == "星期五"){
            tmp.Cweek = 5;
        }else if(re == "星期六"){
            tmp.Cweek = 6;
        }else  tmp.Cweek = 7;

        tmp.Chour = ui->timeEdit->time().hour();
        tmp.Cmin = ui->timeEdit->time().minute();

        QFile op3("D:\\dataexample\\op.txt");
        op3.open(QIODevice::Append);
        QTextStream opp3(&op3);
        opp3.setCodec(QTextCodec::codecForName("utf-8"));
        QDateTime dateTime3(QDateTime::currentDateTime());
        QString oppp3 = dateTime3.toString("yyyy-MM-dd hh:mm:ss ddd");
        oppp3 += " ";
        oppp3 += "学生学号：" + *xuehao + "  添加闹钟：" + con;
        opp3 << oppp3 << endl;
        op3.close();

        naozhong->clock[*allclock] = tmp;
        *(allclock) += 1;

        this->close();
    });

}

QString clockmanager::cut(QString tocut){

    QString s = "";
    static int m = 0;

    while(tocut[m] != " "){
        s += tocut[m];
        m++;
    }
        m = 0;
    return s;
}

void clockmanager::setact(actKu* activity){

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(activity->alltheact);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<< "活动类型" <<"活动频率" <<"活动时间" <<"活动内容");

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

                 ui->tableWidget->setItem(x,y++,new QTableWidgetItem(typ));
                 ui->tableWidget->setItem(x,y++,new QTableWidgetItem(fre));
                 ui->tableWidget->setItem(x,y++,new QTableWidgetItem(tim));
                 ui->tableWidget->setItem(x,y++,new QTableWidgetItem(con));
                 x++;
                 y = 0;
            j++;
        }
    }

    ui->tableWidget->verticalHeader()->setMinimumSectionSize(70);
    ui->tableWidget->horizontalHeader()->setMinimumSectionSize(100);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
}

clockmanager::~clockmanager()
{
    delete ui;
}
