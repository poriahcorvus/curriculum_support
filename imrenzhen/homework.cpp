#include "homework.h"
#include "ui_homework.h"
#include "dealfile.h"
#include "quanju.h"
#include "course.h"
#include "courseku.h"
#include "homeku.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVector>
#include <QTextCodec>
#include <QComboBox>


/*
homework::homework(QWidget *parent):
    QWidget(parent),
    ui(new Ui::homework)
{
    ui->setupUi(this);
}
*/

homework::homework(Course cou[], homeKu *allhomework, QDialog *parent) :
    QDialog(parent),
    ui(new Ui::homework)
{
    ui->setupUi(this);
    this->resize(QSize(1000, 800));

    //connect(ui->homeback, &QPushButton::clicked, this, &homework::sendslot);//返回
    QString catalog = "";
    QString homepage = (*cPath);
    QString cpp = homepage + "\\data_course.txt";
/*
    QFile coufile(cpp);
    coufile.open(QIODevice::ReadOnly);
    int coun = 0;
    QString counum = "";
    int cousize, couplace;
    courseKu ck;
    Course cou[20];
    while(!coufile.atEnd()){
        counum = coufile.readLine();
        cousize = counum.size();
        counum.remove(cousize-2, 2);
        couplace = ck.search(counum, NUM);
        if(couplace != -1){
            cou[coun] = ck.num[couplace];
            coun += 1;
        }
    }
    coufile.close();
    */

    //显示所有作业及状态
    //未提交作业在前，已提交作业在后
    QVector<QString>tnumber;
    QVector<QString>tname;
    QVector<QString>tddl;

    QVector<QString>hnumber;
    QVector<QString>hname;
    QVector<QString>hfilename;
    QVector<QString>htime;

    HadUp(homepage, hnumber, hname, hfilename, htime);
    ToBeUp(allhomework, cou, tnumber, tname, tddl, hnumber, hname);
/*
    QString hometext = "";
    for(int i = 0; i < tnumber.size(); i ++){

        hometext += tname[i];
        hometext += "\t";
        QString coname = NumtoName(cou, tnumber[i]);
        hometext += coname;
        hometext += "\t";
        hometext += tddl[i];
        hometext += "\n";

    }

    for(int i = 0; i < hnumber.size(); i ++){
        hometext += hname[i];
        hometext += "\t";
        QString coname = NumtoName(cou, hnumber[i]);
        hometext += coname;
        hometext += "\t";
        hometext += "已提交\n";
    }
    ui->homelist->setText(hometext);
    ui->homelist->setFont(QFont("宋体", 10));
*/
    showtable(cou, tnumber, tname,tddl, hnumber, hname, htime);

    int len = (*allcourse);//已选课程数组长度


    //查询
    ui->cn->addItem("全部课程");
    for(int i = 0; i < len; i ++){
        ui->cn->addItem(cou[i].cName);
        ui->cn->setFont(QFont("宋体", 10));
    }
    ui->cn->setCurrentIndex(-1);

    ui->cs->addItem("全部状态");
    ui->cs->addItem("已提交");
    ui->cs->addItem("未提交");
    ui->cs->setCurrentIndex(-1);

    connect(ui->search, &QPushButton::clicked, this, [=]()
    {
        QString ccn = ui->cn->currentText();
        QString ccs = ui->cs->currentText();

        QVector<QString>tnumber3;
        QVector<QString>tname3;
        QVector<QString>tddl3;

        QVector<QString>hnumber3;
        QVector<QString>hname3;
        QVector<QString>hfilename3;
        QVector<QString>htime3;

        HadUp(homepage, hnumber3, hname3, hfilename3, htime3);
        ToBeUp(allhomework, cou, tnumber3, tname3, tddl3, hnumber3, hname3);


        if(ccn == "全部课程"){//所有课
            if(ccs == "全部状态"){
                showtable(cou, tnumber3, tname3, tddl3, hnumber3, hname3, htime3);
            }
            else if(ccs == "已提交"){
                showhad(cou, hnumber3, hname3, htime3);
            }
            else if(ccs == "未提交"){
                showto(cou, tnumber3, tname3, tddl3);
            }
        }
        else{//某门课
            QString ccm = "";
            for(int i = 0; i < (*allcourse); i ++){
                if(ccn == cou[i].cName){
                    ccm = cou[i].cNum;
                    break;
                }
            }

            QVector<QString>tnumber4;
            QVector<QString>tname4;
            QVector<QString>tddl4;

            QVector<QString>hnumber4;
            QVector<QString>hname4;
            QVector<QString>hfilename4;
            QVector<QString>htime4;

            //qDebug() << hnumber3.size();
            //qDebug() << tnumber3.size();

            for(int i = 0; i < hnumber3.size(); i ++){
                if(ccm == hnumber3[i]){
                    hnumber4.push_back(hnumber3[i]);
                    hname4.push_back(hname3[i]);
                    htime4.push_back(htime3[i]);
                }
            }

            for(int i = 0; i < tnumber3.size(); i ++){
                if(ccm == tnumber3[i]){
                    tnumber4.push_back(tnumber3[i]);
                    tname4.push_back(tname3[i]);
                    tddl4.push_back(tddl3[i]);
                }
            }

            if(ccs == "全部状态"){
                showtable(cou, tnumber4, tname4, tddl4, hnumber4, hname4, htime4);
            }
            else if(ccs == "已提交"){
                showhad(cou, hnumber4, hname4, htime4);
            }
            else if (ccs == "未提交"){
                showto(cou, tnumber4, tname4, tddl4);
            }
        }

        QFile op("d:\\dataexample\\op.txt");
        op.open(QIODevice::Append);
        QTextStream opp(&op);
        opp.setCodec(QTextCodec::codecForName("utf-8"));
        QString gg = GetTime();
        QString hh = " 作业查询";
        QString oppr="学生学号：";
        opp << gg << " " << oppr << *xuehao << hh << " " << ccn << " " << ccs << "\n";
        op.close();
    });

    //排序
    ui->sorttype->addItem("课程编号");
    ui->sorttype->setCurrentIndex(-1);

    connect(ui->sort, &QPushButton::clicked, this, [=]()
    {
       QString st = ui->sorttype->currentText();

       QVector<QString>tnumber3;
       QVector<QString>tname3;
       QVector<QString>tddl3;

       QVector<QString>hnumber3;
       QVector<QString>hname3;
       QVector<QString>hfilename3;
       QVector<QString>htime3;

       HadUp(homepage, hnumber3, hname3, hfilename3, htime3);
       ToBeUp(allhomework, cou, tnumber3, tname3, tddl3, hnumber3, hname3);

       QVector<QString>nnumber;
       QVector<QString>nname;
       QVector<QString>ntime;

       for(int i = 0; i < hnumber3.size(); i ++){
           nnumber.push_back(hnumber3[i]);
           nname.push_back(hname3[i]);
           ntime.push_back("已提交");
       }
       for(int i = 0; i < tnumber3.size(); i ++){
           nnumber.push_back(tnumber3[i]);
           nname.push_back(tname3[i]);
           ntime.push_back(tddl3[i]);
       }

       if(st == "课程编号"){
            sortnumber(nnumber, nname, ntime);
            showto(cou, nnumber, nname, ntime);

            QFile op("d:\\dataexample\\op.txt");
            op.open(QIODevice::Append);
            QTextStream opp(&op);
            opp.setCodec(QTextCodec::codecForName("utf-8"));
            QString gg = GetTime();
            QString hh = " 作业排序";
            QString oppr="学生学号：";
            opp << gg << " " << oppr << *xuehao << hh << "\n";
            op.close();
       }
    });

    //选择课程


    for(int i = 0; i < len; i ++){
        ui->coursename->addItem(cou[i].cName);
        ui->coursename->setFont(QFont("宋体", 10));
    }
    ui->coursename->setCurrentIndex(-1);



    //选择作业
    connect(ui->coursename, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [=]()
    {
        ui->homename->clear();
        QString cur = ui->coursename->currentText();
        QString curnm = NametoNum(cou, cur);
        for(int i = 0; i < tnumber.size(); i ++){
            if(curnm == tnumber[i])
                ui->homename->addItem(tname[i]);
        }
        for(int i = 0; i < hnumber.size(); i ++){
            if(curnm == hnumber[i])
                ui->homename->addItem(hname[i]);
        }
    });

    /*
    for(int i = 0; i < tnumber.size(); i ++){
        ui->homename->addItem(tname[i]);
    }
    for(int i = 0; i < hnumber.size(); i ++){
        ui->homename->addItem(hname[i]);

    }
    */

    ui->homename->setFont(QFont("宋体", 10));
    ui->homename->setCurrentIndex(-1);


    //选择文件
    connect(ui->hselect, &QPushButton::clicked, this,[=]()
    {
        QString bfilename = "";
        QString homefile = "";
        homefile = QFileDialog::getOpenFileName(this, "Open File", "d:\\PoriaHua");
        if(!homefile.isEmpty()){
            QFileInfo info(homefile);
            QString s = info.fileName();
            ui->hpath->setFont(QFont("宋体", 10));
            ui->hpath->setText(s);
            bfilename = s;
        }
        //qDebug() << homefile;
        connect(ui->homeup, &QPushButton::clicked, this,[=]()
        {

            QString tnm = ui->coursename->currentText();
            QString bnumber = NametoNum(cou, tnm);
            QString bname = ui->homename->currentText();

            QString c1 = bnumber;
            QString cat = "";
            cat = homepage;
            cat += "\\";
            cat += c1;

            double bb = SimilarityMax("d:\\dataexample\\student", homefile);
            int aa = compress_hom(cat, homefile);
            qDebug() << aa;
            if(aa == -1){
                QMessageBox::critical(this, "错误信息", "上传失败！", QMessageBox::Close);

                QFile op("d:\\dataexample\\op.txt");
                op.open(QIODevice::Append);
                QTextStream opp(&op);
                opp.setCodec(QTextCodec::codecForName("utf-8"));
                QString gg = GetTime();
                QString hh = " 作业提交失败";
                QString oppr="学生学号：";
                opp << gg << " " << oppr << *xuehao << hh << "\n";
                op.close();
            }
            else if(aa == 1){
                QString tt1 = QString::number(allweek);
                QString tt2 = "";
                if(curWeek == 1)
                    tt2 = "星期一";
                else if(curWeek == 2)
                    tt2 = "星期二";
                else if(curWeek == 3)
                    tt2 ="星期三";
                else if(curWeek == 4)
                    tt2 ="星期四";
                else if(curWeek == 5)
                    tt2 ="星期五";
                else if(curWeek == 6)
                    tt2 ="星期六";
                else if(curWeek == 7)
                    tt2 ="星期日";
                QString tt3 = QString::number(curHour);
                QString tt4 = QString::number(curMin);
                QString btime = "第";
                btime += tt1;
                btime += "周";
                btime += tt2;
                btime += tt3;
                btime += ":";
                btime += tt4;

                QString hin = homepage;
                hin += "\\homework.txt";
                QFile hadd(hin);
                hadd.open(QIODevice::Append);
                QTextStream hs(&hadd);
                hs.setCodec("utf-8");
                hs << bnumber << " "
                   << bname << " "
                   << bfilename << " "
                   << btime
                   << "\n";
                hadd.close();


                QString tips = "上传成功！\n作业重复率为：";
                QString bbs = QString::number(bb, 'f', 4);
                tips += bbs;
                QMessageBox::information(this, "提醒", tips, QMessageBox::Close);

                QVector<QString>tnumber2;
                QVector<QString>tname2;
                QVector<QString>tddl2;

                QVector<QString>hnumber2;
                QVector<QString>hname2;
                QVector<QString>hfilename2;
                QVector<QString>htime2;

                HadUp(homepage, hnumber2, hname2, hfilename2, htime2);
                ToBeUp(allhomework, cou, tnumber2, tname2, tddl2, hnumber2, hname2);
                showtable(cou, tnumber2, tname2, tddl2, hnumber2, hname2, htime2);

                QFile op("d:\\dataexample\\op.txt");
                op.open(QIODevice::Append);
                QTextStream opp(&op);
                opp.setCodec(QTextCodec::codecForName("utf-8"));
                QString gg = GetTime();
                QString hh = " 作业提交成功 重复率：";
                QString oppr="学生学号：";
                opp << gg << " " << oppr << *xuehao << hh << bbs << "\n";
                op.close();
            }
        });
    });
}

homework::~homework()
{
    delete ui;
}

void homework::sendslot()
{
    emit hret();
}

void homework::showtable(Course cou[], QVector<QString> tnumber, QVector<QString> tname, QVector<QString> tddl, QVector<QString> hnumber, QVector<QString> hname, QVector<QString> htime)
{
    //ui->homelist->clear();

    int xx = ui->homelist->rowCount();
    for(int i = 0; i < xx; i ++){
        ui->homelist->removeRow(0);
    }

    QStringList header;
    header << "作业" << "课程" << "状态（截止日期）";
    ui->homelist->setColumnCount(3);
    ui->homelist->setHorizontalHeaderLabels(header);
    ui->homelist->verticalHeader()->setVisible(false);

    for(int i = 0; i < tnumber.size(); i ++){
        int kk = ui->homelist->rowCount();
        ui->homelist->insertRow(kk);
        ui->homelist->setItem(kk, 0, new QTableWidgetItem(tname[i]));
        QString coname = NumtoName(cou, tnumber[i]);
        ui->homelist->setItem(kk, 1, new QTableWidgetItem(coname));
        ui->homelist->setItem(kk, 2, new QTableWidgetItem(tddl[i]));
    }

    for(int i = 0; i < hnumber.size(); i ++){
        int kk = ui->homelist->rowCount();
        ui->homelist->insertRow(kk);
        ui->homelist->setItem(kk, 0, new QTableWidgetItem(hname[i]));
        QString coname = NumtoName(cou, hnumber[i]);
        ui->homelist->setItem(kk, 1, new QTableWidgetItem(coname));
        ui->homelist->setItem(kk, 2, new QTableWidgetItem("已提交"));
    }
    ui->homelist->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->homelist->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void homework::showhad(Course cou[], QVector<QString> hnumber, QVector<QString> hname, QVector<QString> htime)
{
    int xx = ui->homelist->rowCount();
    for(int i = 0; i < xx; i ++){
        ui->homelist->removeRow(0);
    }

    QStringList header;
    header << "作业" << "课程" << "状态";
    ui->homelist->setColumnCount(3);
    ui->homelist->setHorizontalHeaderLabels(header);
    ui->homelist->verticalHeader()->setVisible(false);

    for(int i = 0; i < hnumber.size(); i ++){
        int kk = ui->homelist->rowCount();
        ui->homelist->insertRow(kk);
        ui->homelist->setItem(kk, 0, new QTableWidgetItem(hname[i]));
        QString coname = NumtoName(cou, hnumber[i]);
        ui->homelist->setItem(kk, 1, new QTableWidgetItem(coname));
        ui->homelist->setItem(kk, 2, new QTableWidgetItem("已提交"));
    }
    ui->homelist->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->homelist->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void homework::showto(Course cou[], QVector<QString> tnumber, QVector<QString> tname, QVector<QString> tddl)
{
    int xx = ui->homelist->rowCount();
    for(int i = 0; i < xx; i ++){
        ui->homelist->removeRow(0);
    }

    QStringList header;
    header << "作业" << "课程" << "截止日期";
    ui->homelist->setColumnCount(3);
    ui->homelist->setHorizontalHeaderLabels(header);
    ui->homelist->verticalHeader()->setVisible(false);

    for(int i = 0; i < tnumber.size(); i ++){
        int kk = ui->homelist->rowCount();
        ui->homelist->insertRow(kk);
        ui->homelist->setItem(kk, 0, new QTableWidgetItem(tname[i]));
        QString coname = NumtoName(cou, tnumber[i]);
        ui->homelist->setItem(kk, 1, new QTableWidgetItem(coname));
        ui->homelist->setItem(kk, 2, new QTableWidgetItem(tddl[i]));
    }
    ui->homelist->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->homelist->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void homework::sortnumber(QVector<QString> &tnumber, QVector<QString> &tname, QVector<QString> &ttime)
{
    QVector<int> nn;
    for(int i = 0; i < tnumber.size(); i ++){
        nn.push_back(tnumber[i].toInt());
    }

    int temp = 0;
    QString tem1 = "";
    QString tem2 = "";
    QString tem3 = "";

    for(int i = 0; i < tnumber.size()-1; i ++){
        for(int j = 0; j < tnumber.size()-1-i; j ++){
            if(nn[j] > nn[j+1]){
                temp = nn[j+1];
                tem1 = tnumber[j+1];
                tem2 = tname[j+1];
                tem3 = ttime[j+1];

                nn[j+1] = nn[j];
                tnumber[j+1] = tnumber[j];
                tname[j+1] = tname[j];
                ttime[j+1] = ttime[j];

                nn[j] = temp;
                tnumber[j] = tem1;
                tname[j] = tem2;
                ttime[j] = tem3;
            }
        }
    }


}
