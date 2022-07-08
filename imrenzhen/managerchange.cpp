#include "managerchange.h"
#include "ui_managerchange.h"
#include<QString>
#include<QDebug>
#include"quanju.h"
#include"courseplus.h"
#include"plusgroupact.h"
#include<QTextStream>
#include<QFile>
#include<QTextCodec>
#include<QDateTime>
#include<QDate>
#include<QTime>
#include <QFileInfo>
#include<QDir>

managerchange::managerchange(courseKu* thecourse,homeKu* homework,groupActKu* groupact,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::managerchange)
{
    ui->setupUi(this);

    course = thecourse;
    hwork = homework;
    groupa = groupact;

    //课程部分
    showNowCourse(thecourse);
    //实现排序
    connect(ui->thenum,&QRadioButton::clicked,[=](){
        soort(thecourse,0,(*coursenumber)-1,1);
        showNowCourse(thecourse);
    });
    connect(ui->thetime,&QRadioButton::clicked,[=](){
        soort(thecourse,0,(*coursenumber)-1,2);
        showNowCourse(thecourse);
    });
    //实现删除
    connect(ui->delete_2,&QPushButton::clicked,[=](){

        int x = ui->course_tableWidget->currentItem()->row();
        QString thenum = ui->course_tableWidget->item(x,0)->text();
        QString thename = ui->course_tableWidget->item(x,1)->text();
        QString opname = thename;

        for(int i = 0 ; i < *coursenumber; i++){
            //删all
            if(QString::compare(thecourse->all[i].cNum,thenum) == 0){
                int j;
                for(j = i ; j < (*coursenumber) - 1; j++)
                    thecourse->all[j] = thecourse->all[j+1];
                thecourse->all[j].cNum = thecourse->all[j].cName = "0";
                (*coursenumber) -= 1;
                break;
            }
        }
        //删num
        int nnplace = thecourse->Hash(thenum);
        while(thecourse->num[nnplace].cNum != ""){
            if(QString::compare(thenum,thecourse->num[nnplace].cNum) == 0){
                thecourse->num[nnplace].cName = "0";
                 thecourse->num[nnplace].cNum = "0";
                 break;
            }
            nnplace++;
        }
        //删name
        nnplace = thecourse->Hash(thename);
        while(thecourse->num[nnplace].cNum != ""){
            if(QString::compare(thename,thecourse->num[nnplace].cName) == 0){
                thecourse->num[nnplace].cName = thecourse->num[nnplace].cNum = "0";
                break;
            }
           nnplace++;
        }

        QFile op("D:\\dataexample\\op.txt");
        op.open(QIODevice::Append);
        QTextStream opp(&op);
        opp.setCodec(QTextCodec::codecForName("utf-8"));
        QDateTime dateTime(QDateTime::currentDateTime());
        QString oppp = dateTime.toString("yyyy-MM-dd hh:mm:ss ddd");
        oppp += " ";
        oppp += "管理员端  删除课程 " + opname;
        opp << oppp << endl;
        op.close();

        //删除对应文件夹
        QString ppath = "D:\\dataexample\\course\\" + thenum;
        DeleteFile(ppath);


       showNowCourse(thecourse);
    });
    //实现添加
    connect(ui->plus,&QPushButton::clicked,[=](){
        coursePlus w(thecourse);
        w.exec();
       showNowCourse(thecourse);
    });

    //作业部分
    //作业的展示
    showhomo(homework,thecourse);
    //按课程编号排序
    connect(ui->sortButton,&QPushButton::clicked,[=](){
    hosort(homework,0,(*allwork)-1);
    showhomo(homework,thecourse);
    });

    //添加
    connect(ui->plushome,&QPushButton::clicked,[=](){
        Plushome a(thecourse,homework);
        a.exec();
        showhomo(homework,thecourse);
    });

    //删除
    connect(ui->deletehome,&QPushButton::clicked,[=](){

        int x = ui->homeWidget->currentItem()->row();
        QString todnum = ui->homeWidget->item(x,0)->text();
        QString todname = ui->homeWidget->item(x,2)->text();

        for(int i = 0; i < *allwork; i++){
            if(homework->allhomo[i].cu_num == todnum){
                if(homework->allhomo[i].homo_name == todname){
                    int j;
                    for(j = i; j < (*allwork) - 1; j++)
                        homework->allhomo[j] = homework->allhomo[j+1];
                    homework->allhomo[j].cu_num = homework->allhomo[j].homo_name = "";
                    (*allwork) -= 1;
                    break;
                }
            }
        }

        QFile op2("D:\\dataexample\\op.txt");
        op2.open(QIODevice::Append);
        QTextStream opp2(&op2);
        opp2.setCodec(QTextCodec::codecForName("utf-8"));
        QDateTime dateTime2(QDateTime::currentDateTime());
        QString oppp2 = dateTime2.toString("yyyy-MM-dd hh:mm:ss ddd");
        oppp2 += " ";
        oppp2 += "管理员端 删除作业 课程编号为：" + todnum + " 作业名称为:" + todname;
        opp2 << oppp2 << endl;
        op2.close();

        showhomo(homework,thecourse);
    });

    //集体活动部分
    //展示当前集体活动
    showgroupact(groupact);

    //添加
    connect(ui->plusact,&QPushButton::clicked,[=](){
        plusgroupAct pp(groupact);
        pp.exec();
        showgroupact(groupact);
    });

    //删除
    connect(ui->deleteact,&QPushButton::clicked,[=](){
        int x = ui->groupWidget->currentItem()->row();
        QString classrr = ui->groupWidget->item(x,0)->text();
        QString classcon = ui->groupWidget->item(x,3)->text();        

        QFile op3("D:\\dataexample\\op.txt");
        op3.open(QIODevice::Append);
        QTextStream opp3(&op3);
        opp3.setCodec(QTextCodec::codecForName("utf-8"));
        QDateTime dateTime3(QDateTime::currentDateTime());
        QString oppp3 = dateTime3.toString("yyyy-MM-dd hh:mm:ss ddd");
        oppp3 += " ";
        oppp3 += "管理员端 删除集体活动 班级为：" + classrr + " 活动内容为：" + classcon;
        opp3 << oppp3 << endl;
        op3.close();

        bool ok;
        for(int i = 0; i < *allgroupAct; i++){
            if(groupact->gAct[i].classroom == classrr.toInt(&ok,10)){
                if(groupact->gAct[i].content == classcon){
                    int j;
                    for(j = i; j < *allgroupAct - 1; j++)
                        groupact->gAct[j] = groupact->gAct[j+1];
                    groupact->gAct[j].classroom = groupact->gAct[j].frequence = 0;
                    groupact->gAct[j].content = "";
                    (*allgroupAct) -= 1;
                    break;
                }
            }
        }
        showgroupact(groupact);
    });

    //资料部分




}

void managerchange::showNowCourse(courseKu* thecourse){
    //清除表格
    int x = ui->course_tableWidget->rowCount();
    for(int i = 0; i < x; i++)
        ui->course_tableWidget->removeRow(i);

    ui->course_tableWidget->setColumnCount(8);
    QStringList aa;
    aa << "课程编号" << "课程名称" << "上课地点" << "上课时间"  << "课程群" << "授课老师" << "考试时间" << "考试地点";
    ui->course_tableWidget->setHorizontalHeaderLabels(aa);
    ui->course_tableWidget->setRowCount(*coursenumber);

    bool ok;
    for(int i = 0; i < *coursenumber; i++){

        int y = 0;
        QString place = where(thecourse->all[i].cPlace.toInt(&ok,10)) + thecourse->all[i].cDetail;

        QString coutime = "星期";
        switch(thecourse->all[i].cWeek.toInt(&ok,10)){
        case 1:
            coutime += "一  ";
            break;
        case 2:
            coutime += "二  ";
            break;
        case 3:
            coutime += "三  ";
            break;
        case 4:
            coutime += "四  ";
            break;
        default:
            coutime += "五  ";
            break;
        }

        coutime += thecourse->all[i].cStartHour + ":";
        if(thecourse->all[i].cStartMin.toInt(&ok,10) < 10){
            coutime += "0" + thecourse->all[i].cStartMin;
        }else coutime += thecourse->all[i].cStartMin;
        coutime += "-";
        coutime += thecourse->all[i].cEndHour + ":";
        if(thecourse->all[i].cEndMin.toInt(&ok,10) < 10)
            coutime += "0" + thecourse->all[i].cEndMin;
        else coutime += thecourse->all[i].cEndMin;
        QString ksp = where(thecourse->all[i].kPlace.toInt(&ok,10)) + thecourse->all[i].kDetail;

        ui->course_tableWidget->setItem(i,y++,new QTableWidgetItem(thecourse->all[i].cNum));
        ui->course_tableWidget->setItem(i,y++,new QTableWidgetItem(thecourse->all[i].cName));
        ui->course_tableWidget->setItem(i,y++,new QTableWidgetItem(place));
        ui->course_tableWidget->setItem(i,y++,new QTableWidgetItem(coutime));
        ui->course_tableWidget->setItem(i,y++,new QTableWidgetItem(thecourse->all[i].cGroup));
        ui->course_tableWidget->setItem(i,y++,new QTableWidgetItem(thecourse->all[i].teacher));
        ui->course_tableWidget->setItem(i,y++,new QTableWidgetItem(thecourse->all[i].kTime));
        ui->course_tableWidget->setItem(i,y++,new QTableWidgetItem(ksp));
        y = 0;
    }

    ui->course_tableWidget->verticalHeader()->setMinimumSectionSize(70);
    ui->course_tableWidget->horizontalHeader()->setMinimumSectionSize(100);
    ui->course_tableWidget->resizeColumnsToContents();
    ui->course_tableWidget->resizeRowsToContents();
}

void managerchange::showhomo(homeKu* home,courseKu* thecourse){

    //清除表格
    int x = ui->homeWidget->rowCount();
    for(int i = 0; i < x; i++)
        ui->homeWidget->removeRow(i);

    ui->homeWidget->setColumnCount(5);
    QStringList aa;
    aa << "课程编号" << "课程名字" << "作业名称" << "截止时间" << "备注";
    ui->homeWidget->setHorizontalHeaderLabels(aa);
    ui->homeWidget->setRowCount(*allwork);

    for(int i = 0; i < *allwork; i++){
        int y = 0;

        QString cuname;
        QString time = "第" + QString::number(home->allhomo[i].ddl_howweek) + "周 ";
        int pp = thecourse->Hash(home->allhomo[i].cu_num);

        while(thecourse->num[pp].cNum != ""){
            if(thecourse->num[pp].cNum == home->allhomo[i].cu_num){
                cuname = thecourse->num[pp].cName;
                break;
            }
            pp++;
        }

        time += "星期";
        switch (home->allhomo[i].ddl_week) {
        case 1:
            time += "一 ";
            break;
        case 2:
            time += "二 ";
            break;
        case 3:
            time += "三 ";
            break;
        case 4:
            time += "四 ";
            break;
        case 5:
            time += "五 ";
            break;
        case 6:
            time += "六 ";
            break;
        default:
            time += "七 ";
            break;
        }
        time += QString::number(home->allhomo[i].ddl_hour) + ":";
        if(home->allhomo[i].ddl_min < 10)
            time += "0" + QString::number(home->allhomo[i].ddl_min);
        else time += QString::number(home->allhomo[i].ddl_min);

        ui->homeWidget->setItem(i,y++,new QTableWidgetItem(home->allhomo[i].cu_num));
        ui->homeWidget->setItem(i,y++,new QTableWidgetItem(cuname));
        ui->homeWidget->setItem(i,y++,new QTableWidgetItem(home->allhomo[i].homo_name));
        ui->homeWidget->setItem(i,y++,new QTableWidgetItem(time));
        ui->homeWidget->setItem(i,y++,new QTableWidgetItem(home->allhomo[i].homo_desc));

        y = 0;
    }
    ui->homeWidget->verticalHeader()->setMinimumSectionSize(70);
    ui->homeWidget->horizontalHeader()->setMinimumSectionSize(100);
    ui->homeWidget->resizeColumnsToContents();
    ui->homeWidget->resizeRowsToContents();
}

void managerchange::showgroupact(groupActKu* groupact){

    //清除表格
    int x = ui->groupWidget->rowCount();
    for(int i = 0; i < x; i++)
        ui->groupWidget->removeRow(i);

    ui->groupWidget->setColumnCount(4);
    QStringList aa;
    aa << "班级" << "频率" << "活动时间" << "活动内容";
    ui->groupWidget->setHorizontalHeaderLabels(aa);
    ui->groupWidget->setRowCount(*allgroupAct);

    for(int i = 0; i < *allgroupAct; i++){

        int y = 0;
        QString fre;
        switch (groupact->gAct[i].frequence){
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
        QString time;
        time += "星期";
        switch (groupact->gAct[i].week) {
        case 1:
            time += "一 ";
            break;
        case 2:
            time += "二 ";
            break;
        case 3:
            time += "三 ";
            break;
        case 4:
            time += "四 ";
            break;
        case 5:
            time += "五 ";
            break;
        case 6:
            time += "六 ";
            break;
        default:
            time += "七 ";
            break;
        }
        time += QString::number(groupact->gAct[i].hour) + ":";
        if(groupact->gAct[i].min < 10)
            time += "0" + QString::number(groupact->gAct[i].min);
        else time += QString::number(groupact->gAct[i].min);

        ui->groupWidget->setItem(i,y++,new QTableWidgetItem(QString::number(groupact->gAct[i].classroom)));
        ui->groupWidget->setItem(i,y++,new QTableWidgetItem(fre));
        ui->groupWidget->setItem(i,y++,new QTableWidgetItem(time));
        ui->groupWidget->setItem(i,y++,new QTableWidgetItem(groupact->gAct[i].content));

        y = 0;
    }
    ui->groupWidget->verticalHeader()->setMinimumSectionSize(70);
    ui->groupWidget->horizontalHeader()->setMinimumSectionSize(50);
    ui->groupWidget->resizeColumnsToContents();
    ui->groupWidget->resizeRowsToContents();
}

void managerchange::soort(courseKu* tosort,int left,int right,int type){

    if(left >= right)
        return;

    int l = left;
    int r = right;
    bool ok;
    Course p;

    //1 num  2 time
    if(type == 1){
        while(l < r){

            while(tosort->all[r].cNum.toInt(&ok,10) >= tosort->all[left].cNum.toInt(&ok,10) && l < r){
                r --;
            }
            while(tosort->all[l].cNum.toInt(&ok,10) <= tosort->all[left].cNum.toInt(&ok,10) && l < r){
                l ++;
            }
            p = tosort->all[r];
            tosort->all[r] = tosort->all[l];
            tosort->all[l] = p;
            r--;
        }
            p = tosort->all[left];
            tosort->all[left] = tosort->all[l];
            tosort->all[l] = p;

    }else if(type == 2){
        while(l < r){

            while(isbiger(tosort->all[r],tosort->all[left]) && l < r)
                r--;
            while((!isbiger(tosort->all[l],tosort->all[left])) && l < r)
                l++;

            p = tosort->all[r];
            tosort->all[r] = tosort->all[l];
            tosort->all[l] = p;
            r--;
        }
            p = tosort->all[left];
            tosort->all[left] = tosort->all[l];
            tosort->all[l] = p;

    }

    //递归
    soort(tosort,left,l-1,type);
    soort(tosort,l+1,right,type);
}

void managerchange::hosort(homeKu* tosort,int left,int right){
    if(left >= right)
        return;

    int l = left;
    int r = right;
    bool ok;
    Manahome p;

    //1 num  2 time
        while(l < r){

            while(tosort->allhomo[r].cu_num.toInt(&ok,10) >= tosort->allhomo[left].cu_num.toInt(&ok,10) && l < r){
                r --;
            }
            while(tosort->allhomo[l].cu_num.toInt(&ok,10) <= tosort->allhomo[left].cu_num.toInt(&ok,10) && l < r){
                l ++;
            }
            p = tosort->allhomo[r];
            tosort->allhomo[r] = tosort->allhomo[l];
            tosort->allhomo[l] = p;
            r--;
        }
            p = tosort->allhomo[left];
            tosort->allhomo[left] = tosort->allhomo[l];
            tosort->allhomo[l] = p;

    //递归
    hosort(tosort,left,l-1);
    hosort(tosort,l+1,right);
}

int managerchange::isbiger(Course a,Course b){
    int result = 0;
    bool ok;
    if(a.cWeek.toInt(&ok,10) > b.cWeek.toInt(&ok,10)){
        result = 1;
    }else if(a.cWeek == b.cWeek){
        if(a.cStartHour.toInt(&ok,10) > b.cStartHour.toInt(&ok,10))
            result = 1;
        else if(a.cStartHour.toInt(&ok,10) == b.cStartHour.toInt(&ok,10)){
            if(a.cStartMin.toInt(&ok,10) > b.cStartMin.toInt(&ok,10))
                result = 1;
        }
    }
    return result;
}

bool managerchange::DeleteFile(QString &strPath){

    if(strPath.isEmpty() || !QDir().exists(strPath))
        return false;

    QFileInfo FileInfo(strPath);

    if(FileInfo.isFile())
        QFile::remove(strPath);
    else if(FileInfo.isDir()){
        QDir qDir(strPath);
        qDir.removeRecursively();
    }
    return true;
}

managerchange::~managerchange()
{
    //在这里写回文件
    //写回课程
    QFile file("D:\\dataexample\\course\\allCourse.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream txtoutput(&file);
    txtoutput.setCodec("utf-8");
    QString aa = "0 ";
    QString toput;
    for(int i = 0; i < *coursenumber; i++){
        toput = "";
        toput += course->all[i].cNum + " ";
        toput += course->all[i].cName + " ";
        toput += course->all[i].teacher + " ";
        toput += course->all[i].cGroup + " ";
        toput += course->all[i].cWeek + " ";
        toput += course->all[i].cStartHour + " ";
        if(course->all[i].cStartMin == "00"){
        toput += aa;
        }else toput += course->all[i].cStartMin + " ";

        toput += course->all[i].cEndHour + " ";
        if(course->all[i].cEndMin == "00"){
        toput += aa;
        }else toput += course->all[i].cEndMin + " ";

        toput += course->all[i].cPlace + " ";
        toput += course->all[i].cDetail + " ";
        toput += course->all[i].kTime + " ";
        toput += course->all[i].kPlace + " ";
        toput += course->all[i].kDetail + " ";
        toput += course->all[i].cPopular;
        txtoutput << toput << endl;
    }
    file.close();

    //写回作业
    QFile homfile("D:\\dataexample\\course\\allHome.txt");
    homfile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream htextoutput(&homfile);
    htextoutput.setCodec("utf-8");
    for(int i = 0; i < *allwork; i++){

        toput = "";
        toput += hwork->allhomo[i].cu_num + " ";
        toput += hwork->allhomo[i].homo_name + " ";
        toput += QString::number(hwork->allhomo[i].ddl_howweek) + " ";
        toput += QString::number(hwork->allhomo[i].ddl_week) + " ";
        toput += QString::number(hwork->allhomo[i].ddl_hour) + " ";
        toput += QString::number(hwork->allhomo[i].ddl_min) + " ";
        toput += hwork->allhomo[i].homo_desc;
        htextoutput << toput << endl;
    }
    homfile.close();

    //写回活动
    QFile actfile("D:\\dataexample\\course\\allgroupact.txt");
    actfile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream actfille(&actfile);
    actfille.setCodec("utf-8");
    for(int i = 0 ; i < *allgroupAct; i++){
        toput = "";
        toput += QString::number(groupa->gAct[i].classroom) + " ";
        toput += QString::number(groupa->gAct[i].frequence) + " ";
        toput += QString::number(groupa->gAct[i].week) + " ";
        toput += QString::number(groupa->gAct[i].hour) + " ";
        toput += QString::number(groupa->gAct[i].min) + " ";
        toput += groupa->gAct[i].content;
        actfille << toput << endl;
    }
    actfile.close();

    delete ui;
}

