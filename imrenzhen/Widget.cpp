#include "widget.h"
#include"studentwidget.h"
#include "ui_widget.h"
#include<QPushButton>
#include<QDebug>
#include<QDialog>
#include <QMessageBox>
#include"clockthread.h"
#include"quanju.h"
#include"managerchange.h"
#include<QFile>
#include <QTextStream>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    a = new courseKu;
    b = new homeKu;
    c = new groupActKu;
    xuehao = new QString;

    //准备地点数组
    QFile fillle("D:\\dataexample\\skplace.txt");
    fillle.open(QIODevice::ReadOnly);
    bool ok;
    QString p;
    QString line = fillle.readLine();
    while(!fillle.atEnd()){
        p = "";
        int i = 0;
        while(line[i] != " "){
            p += line[i];
            i++;
        }
        line = line.mid(i+1);
        line.remove(line.size()-2,2);
        Whereplace[p.toInt(&ok,10)] = line;
        line = fillle.readLine();
    }
    fillle.close();

    connect(ui->loadBtn,&QPushButton::clicked,[=](){

        QString name = ui->loadLine->text();
        QString path = "D:\\dataexample\\student";
        *xuehao = name;

        if(name <= "2020211490" && name >= "2020211481")
        {
            path += "\\7\\" + name;
            studentWidget s(path,a,b,c);
            s.exec();
        }else if(name >= "2020211511" && name <= "2020211520")
        {
            path += "\\8\\" + name;
            studentWidget s(path,a,b,c);
            s.exec();
        }else{
            QMessageBox::warning(this," ","该学号不存在，请确认输入是否正确");
        }
    });

    connect(ui->mangaButton,&QPushButton::clicked,[=](){
        managerchange mana(a,b,c);
        mana.exec();
    });
}

Widget::~Widget()
{
    delete a;
    delete b;
    delete c;
    delete base;
    delete allclock;
    delete naozhong;
    delete cPath;
    delete nowbase;
    delete allcourse;
    delete coursenumber;
    delete allwork;
    delete allgroupAct;
    delete xuehao;
    delete ui;
}
