#include "updoc.h"
#include "ui_updoc.h"
#include "dealfile.h"
#include "course.h"
#include "courseku.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QDir>
#include <QTextCodec>
#include"quanju.h"
updoc::updoc(courseKu *c, QDialog *parent) :
    QDialog(parent),
    ui(new Ui::updoc)
{
    ui->setupUi(this);
    this->resize(QSize(800, 600));
    //connect(ui->upback, &QPushButton::clicked, this, &updoc::sendslot);

    //QString catalog = "";
    QString homepage = "d:\\dataexample\\course";
    QVector<QString> dirsn;
    QVector<QString> ccsn;
    FindCoursename(homepage, dirsn);
    //ui->coursename->setCurrentIndex(-1);

    for(int i = 0; i < dirsn.size(); i ++){
        QString nnum = dirsn[i];
        QString nname = "";
        int pplace = c->search(nnum,2);
        while(c->num[pplace].cNum != ""){
            if(c->num[pplace].cNum == nnum){
                nname = c->num[pplace].cName;
                break;
            }
        }
        ccsn.push_back(nname);
        ui->coursename->addItem(nname);
        ui->coursename->setFont(QFont("宋体", 10));
    }
    ui->coursename->setCurrentIndex(-1);

    connect(ui->upselect, &QPushButton::clicked, this, [=]()
    {
        QString docfile = "";
        docfile = QFileDialog::getOpenFileName(this, "Choose File", "d:\\PoriaHua\\Poria'sGarbageCan");
        if(!docfile.isEmpty()){
            QFileInfo info(docfile);
            QString s = info.fileName();
            ui->uppath->setFont(QFont("宋体", 10));
            ui->uppath->setText(s);
        }

        connect(ui->upup, &QPushButton::clicked, this, [=]()
        {
            QString cname = ui->coursename->currentText();
            QString cnum = "";
            for(int i = 0; i < dirsn.size(); i ++){
                if(cname == ccsn[i]){
                    cnum = dirsn[i];
                    break;
                }

            }
            QString catalog = homepage;
            catalog += "\\";
            catalog += cnum;

            int a = compress_doc(catalog, docfile);
            if(a == -1){
                if(QMessageBox::Ok == QMessageBox::critical(this, "错误信息", "上传失败！", QMessageBox::Ok)){
                    QFile op("d:\\dataexample\\op.txt");
                    op.open(QIODevice::Append);
                    QTextStream opp(&op);
                    opp.setCodec(QTextCodec::codecForName("utf-8"));
                    QString gg = GetTime();
                    QString hh = " 资料上传失败";
                    QString oppr="学生学号：";
                    opp << gg << " " << oppr << *xuehao << hh << "\n";
                    op.close();
                    this->close();
                }
            }else if(a == 1){
                if(QMessageBox::Ok == QMessageBox::information(this, "提醒", "上传成功！", QMessageBox::Ok)){
                    sendslot();
                    QFile op("d:\\dataexample\\op.txt");
                    op.open(QIODevice::Append);
                    QTextStream opp(&op);
                    opp.setCodec(QTextCodec::codecForName("utf-8"));
                    QString gg = GetTime();
                    QString hh = " 资料上传成功";
                    QString oppr="学生学号：";
                    opp << gg << " " << oppr << *xuehao << hh << "\n";
                    op.close();
                    this->close();
                }
            }else if(a == 2){
                if(QMessageBox::Ok == QMessageBox::critical(this, "错误信息", "资料已存在，请勿重复上传！", QMessageBox::Ok)){
                    QFile op("d:\\dataexample\\op.txt");
                    op.open(QIODevice::Append);
                    QTextStream opp(&op);
                    opp.setCodec(QTextCodec::codecForName("utf-8"));
                    QString gg = GetTime();
                    QString hh = " 资料重复上传";
                    QString oppr="学生学号：";
                    opp << gg << " " << oppr << *xuehao << hh << "\n";
                    op.close();
                    this->close();
                }

            }
        });
    });
}

updoc::~updoc()
{
    delete ui;
}

void updoc::sendslot()
{
    emit upret();
}

