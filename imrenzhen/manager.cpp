#include "manager.h"
#include "ui_manager.h"
#include"quanju.h"
#include"QLineEdit"
#include"QString"
#include"courseku.h"
#include<QDebug>
#include<QString>
#include<QTextStream>
#include<QFile>
#include<QTextCodec>
#include<QDateTime>
#include<QDate>
#include<QTime>
#include<QDir>

Manager::Manager(courseKu* a,Course* b,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Manager)
{
    ui->setupUi(this);
    ui->resultEdit->setPlaceholderText("请输入你想查找的课程的相关信息");
    ui->resultEdit->placeholderText();

    //设置单选按钮默认选中相似度
    ui->similar->setChecked(true);

    //当前课表的显示
    setcu(b);
    showall(a);

    //实现搜索
    connect(ui->c_search,&QPushButton::clicked,[=](){

    QString theline = ui->resultEdit->text();
//    int place;
//    int iii = 1;
    //清空数组
    cclear(sedcourse);
//    place = a->search(theline,NAME);
//    if(place != -1){
//        sedcourse[iii] = a->name[place];
//        iii++;
//    }else{
//        place = a->search(theline,NUM);
//        if(place != -1){
//            sedcourse[iii] = a->num[place];
//            iii++;
//        }
//    }

    //完成搜索
    a->Muhu(theline,sedcourse);
    showresult(sedcourse);
    });

    //实现排序
    connect(ui->similar,&QRadioButton::clicked,[=](){
        sort(sedcourse,1,sedcourse[0].similar,SIMI);
        showresult(sedcourse);       
    });
    connect(ui->popular,&QRadioButton::clicked,[=](){
        sort(sedcourse,1,sedcourse[0].similar,POPU);
        showresult(sedcourse);
    });
    connect(ui->time,&QRadioButton::clicked,[=](){
        sort(sedcourse,1,sedcourse[0].similar,TIME);
        showresult(sedcourse);
    });

    //实现添加
    //获取现在点击的字符串 取名字或编号 用查找添加到当前课程 同时右边显示
    connect(ui->plus,&QPushButton::clicked,[=](){
        QString stringed = ui->resultList->currentItem()->text();
        QString toplus = "";
        int i = 0;
        while(stringed[i] != " "){
            toplus += stringed[i];
            i++;
        }
        int jjj = a->search(toplus,NUM);
        Course p = a->num[jjj];

        QFile op4("D:\\dataexample\\op.txt");
        op4.open(QIODevice::Append);
        QTextStream opp4(&op4);
        opp4.setCodec(QTextCodec::codecForName("utf-8"));
        QDateTime dateTime4(QDateTime::currentDateTime());
        QString oppp4 = dateTime4.toString("yyyy-MM-dd hh:mm:ss ddd");
        oppp4 += " ";
        oppp4 += "学生学号：" + *xuehao + "  添加课程：" + p.cName;
        opp4 << oppp4 << endl;
        op4.close();

        //添加对应文件夹
        QString dir_str = *cPath + "\\" + p.cNum;
        QDir dir;
        if(!dir.exists(dir_str)){
            bool res = dir.mkpath(dir_str);
        }

        b[*allcourse] = p;
        *(allcourse) += 1;
        setcu(b);
    });
    //实现删除
    connect(ui->delete_2,&QPushButton::clicked,[=](){

        QString destring = ui->cList->currentItem()->text();
        QString todelete = "";
        int j = 0;            

        while(destring[j] != " "){
            todelete += destring[j];
            j++;
        }
        int i = 0;
        for(i = 0; i < *allcourse; i++){
            if(b[i].cNum == todelete)
                break;
        }

        QFile op3("D:\\dataexample\\op.txt");
        op3.open(QIODevice::Append);
        QTextStream opp3(&op3);
        opp3.setCodec(QTextCodec::codecForName("utf-8"));
        QDateTime dateTime3(QDateTime::currentDateTime());
        QString oppp3 = dateTime3.toString("yyyy-MM-dd hh:mm:ss ddd");
        oppp3 += " ";
        oppp3 += "学生学号：" + *xuehao + "  删除所选课程：" + b[i].cName;
        opp3 << oppp3 << endl;
        op3.close();

        //删除对应课程文件夹
        QString ppath = *cPath + "\\" + todelete;
        DeleteFile(ppath);

        for(int ij = i; ij < *allcourse - 1 ;ij++){
            b[ij] = b[ij+1];
        }
        *(allcourse) -= 1;
        setcu(b);

    });
}

//#define SIMI 1
//#define POPU 2
//#define TIME 3
void Manager::sort(Course* tosort,int left,int right,int type){

    if(left >= right)
    return ;

    int l = left;
    int r = right;
    bool ok;
    Course p;

        if(type == 1){
            while(l < r){
                while(tosort[r].similar <= tosort[left].similar && l < r){
                    r --;
                }
                while(tosort[l].similar >= tosort[left].similar && l < r){
                    l ++;
                }
                p = tosort[r];
                tosort[r] = tosort[l];
                tosort[l] = p;
                r--;
            }
                p = tosort[left];
                tosort[left] = tosort[l];
                tosort[l] = p;

        }else if(type == 2){
            while(l < r){
                while(tosort[r].cPopular.toInt(&ok,10) <= tosort[left].cPopular.toInt(&ok,10) && l < r){
                    r --;
                }
                while(tosort[l].cPopular.toInt(&ok,10) >= tosort[left].cPopular.toInt(&ok,10) && l < r){
                    l ++;
                }
                p = tosort[r];
                tosort[r] = tosort[l];
                tosort[l] = p;
                r--;
            }
                p = tosort[left];
                tosort[left] = tosort[l];
                tosort[l] = p;
        }else if(type == 3){
            while(l < r){
                while(isbiger(tosort[r],tosort[left]) && l < r)
                    r--;
                while((!isbiger(tosort[l],tosort[left])) && l < r)
                    l++;

                p = tosort[r];
                tosort[r] = tosort[l];
                tosort[l] = p;
                r--;
            }
                p = tosort[left];
                tosort[left] = tosort[l];
                tosort[l] = p;
        }

    //递归
    sort(tosort,left,l-1,type);
    sort(tosort,l+1,right,type);
}
int Manager::isbiger(Course a,Course b){
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

void Manager::cclear(Course* source){
    for(int i = 0; i < 50; i++){
        source[i].cNum = source[i].cName = source[i].cGroup = source[i].teacher = "";
        source[i].similar = 0;
        source[i].cStartHour = source[i].cStartMin = "";
        source[i].cEndHour = source[i].cEndMin = "";
        source[i].cDetail = source[i].kDetail = source[i].kPlace = source[i].kTime = "";
        source[i].cPlace = source[i].cPopular = source[i].cWeek = "";
    }
}

void Manager::setcu(Course* source){
    //清空list
    for(int j = ui->cList->count() - 1; j >= 0 ; j--){
        QListWidgetItem *p = ui->cList->takeItem(j);
        ui->cList->removeItemWidget(p);
    }
    bool ok;
    QString cutable = "";
    QStringList result;

    for(int i = 0; i < *allcourse; i++){
        cutable += source[i].cNum + "  ";
        cutable += source[i].cName + "  ";
        cutable += source[i].teacher + "  ";
        cutable += where(source[i].cPlace.toInt(&ok,10)) + source[i].cDetail + "  星期";
        switch(source[i].cWeek.toInt(&ok,10)){
        case 1:
            cutable += "一 ";
            break;
        case 2:
            cutable += "二 ";
            break;
        case 3:
            cutable += "三 ";
            break;
        case 4:
            cutable += "四 ";
            break;
        default:
            cutable += "五 ";
            break;
        }
        cutable += source[i].cStartHour + ":";
        if(source[i].cStartMin.toInt(&ok,10) < 10)
            cutable += "0" + source[i].cStartMin;
        else cutable += source[i].cStartMin;

        cutable += "-";
            cutable += source[i].cEndHour + ":";
        if(source[i].cEndMin < 10)
             cutable += "0" + source[i].cEndMin;
        else cutable += source[i].cEndMin;

        result << cutable;
        cutable = "";

    }
    ui->cList->addItems(result);
}

void Manager::showall(courseKu* source){
    ui->resultList->clear();
    int i = 1;
    bool ok;
    QString cutable = "";
    QStringList result;
        while(i < *coursenumber ){
            cutable += source->all[i].cNum + "  ";
            cutable += source->all[i].cName + "  ";
            cutable += source->all[i].teacher + "  ";
            cutable += where(source->all[i].cPlace.toInt(&ok,10)) + source->all[i].cDetail + "  星期";
            switch(source->all[i].cWeek.toInt(&ok,10)){
            case 1:
                cutable += "一 ";
                break;
            case 2:
                cutable += "二 ";
                break;
            case 3:
                cutable += "三 ";
                break;
            case 4:
                cutable += "四 ";
                break;
            default:
                cutable += "五 ";
                break;
            }
            cutable += source->all[i].cStartHour + ":";
            if(source->all[i].cStartMin.toInt(&ok,10) < 10)
                cutable += "0" + source->all[i].cStartMin;
            else cutable += source->all[i].cStartMin;

            cutable += "-" + source->all[i].cEndHour + ":";
            if(source->all[i].cEndMin.toInt(&ok,10) < 10)
                cutable += "0" + source->all[i].cEndMin;
            else cutable += source->all[i].cEndMin;

            result << cutable;

            i++;
            cutable = "";
        }
        ui->resultList->addItems(result);
}

void Manager::showresult(Course* source){

    //清空当前列表

    ui->resultList->clear();

    int i = 1;
    bool ok;
    QString cutable = "";
    QStringList result;
        while(i <= source[0].similar ){
            cutable += source[i].cNum + "  ";
            cutable += source[i].cName + "  ";
            cutable += source[i].teacher + "  ";
            cutable += where(source[i].cPlace.toInt(&ok,10)) + source[i].cDetail + "  星期";
            switch(source[i].cWeek.toInt(&ok,10)){
            case 1:
                cutable += "一 ";
                break;
            case 2:
                cutable += "二 ";
                break;
            case 3:
                cutable += "三 ";
                break;
            case 4:
                cutable += "四 ";
                break;
            default:
                cutable += "五 ";
                break;
            }
            cutable += source[i].cStartHour + ":";
            if(source[i].cStartMin.toInt(&ok,10) < 10)
                cutable += "0" + source[i].cStartMin;
            else cutable += source[i].cStartMin;

            cutable += "-" + source[i].cEndHour + ":";
            if(source[i].cEndMin.toInt(&ok,10) < 10)
                cutable += "0" + source[i].cEndMin;
            else cutable += source[i].cEndMin;

            result << cutable;

            i++;
            cutable = "";
        }
        ui->resultList->addItems(result);
}

bool Manager::DeleteFile(QString &strPath){

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

Manager::~Manager()
{
    delete ui;
}
