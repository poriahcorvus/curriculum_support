#include "actmanager.h"
#include "ui_actmanager.h"
#include<QPushButton>
#include<QDebug>
#include"quanju.h"
#include"actplus.h"
#include<QString>
#include<QTextStream>
#include<QFile>
#include<QTextCodec>
#include<QDateTime>
#include<QDate>
#include<QTime>


actmanager::actmanager(Course* a,actKu* activity,QDialog *parent) :
    QDialog(parent),
    ui(new Ui::actmanager)
{
    ui->setupUi(this);

    //显示界面
    showact(activity);

    //添加按钮
    connect(ui->actplus,&QPushButton::clicked,[=](){
        actPlus plus(a,activity,this);
        plus.exec();
        showact(activity);
    });


    //删除按钮
    connect(ui->actdelete,&QPushButton::clicked,[=](){

        int x = ui->tableWidget->currentRow();
        QString typ = ui->tableWidget->item(x,0)->text();
        QString week = ui->tableWidget->item(x,2)->text().left(3);
        QString cont = ui->tableWidget->item(x,3)->text();

        int i,j,k = 0;

        if(typ == "个人")
            i = 0;
        else i = 7;

        if(week == "星期一")
            j = 0;
        else if(week == "星期二")
            j = 1;
        else if(week == "星期三")
            j = 2;
        else if(week == "星期四")
            j = 3;
        else if(week == "星期五")
            j = 4;
        else if(week == "星期六")
            j = 5;
        else j = 6;

        while(activity->allact[i+j][k].content != ""){
            if(activity->allact[i+j][k].content == cont){
                while(activity->allact[i+j][k].content != ""){
                    activity->allact[i+j][k] = activity->allact[i+j][k+1];
                    k++;
                }
                break;
            }
                k++;
        }

        (activity->alltheact)--;
        showact(activity);

        QFile op3("D:\\dataexample\\op.txt");
        op3.open(QIODevice::Append);
        QTextStream opp3(&op3);
        opp3.setCodec(QTextCodec::codecForName("utf-8"));
        QDateTime dateTime3(QDateTime::currentDateTime());
        QString oppp3 = dateTime3.toString("yyyy-MM-dd hh:mm:ss ddd");
        oppp3 += " ";
        oppp3 += "学生学号：" + *xuehao + "  删除活动：" + cont;
        opp3 << oppp3 << endl;
        op3.close();


        //同步删除闹钟
        int cc = 0;
        while(naozhong->clock[cc].Ccontent != ""){
            if(naozhong->clock[cc].Ccontent == cont){
              while(naozhong->clock[cc].Ccontent != "")
                  naozhong->clock[cc] = naozhong->clock[cc+1];
              *(allclock) -= 1;
            }
            cc++;
        }
    });

}

void actmanager::showact(actKu* activity){
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

actmanager::~actmanager()
{
    delete ui;
}
