#include "document.h"
#include "ui_document.h"
#include "dealfile.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QVector>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextCodec>
#include"quanju.h"

document::document(courseKu *c, QDialog *parent) :
    QDialog(parent),
    ui(new Ui::document)
{
    ui->setupUi(this);
    this->resize(QSize(800, 600));

    //textedit展示现有资料
    QVector<QString> filesn;
    FindBaseFile("d:\\dataexample\\course", filesn);
    /*
    QString doc = "";
    for(int i = 0; i < filesn.size(); i ++){
            doc += filesn[i];
            doc += "\n";
    }
    ui->existdoc->setText(doc);
    ui->existdoc->setFont(QFont("宋体", 10));
    */

    showtable(filesn);

/*
    connect(ui->docback, &QPushButton::clicked, this, &document::sendslot);
    connect(ui->docup, &QPushButton::clicked, this, &document::gotoup);
    connect(&uppage, &updoc::upret, this, &document::upslot);
    connect(ui->docdown, &QPushButton::clicked, this, &document::gotodown);
    connect(&downpage, &down::downret, this, &document::downslot);
*/

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
        ui->cn->addItem(nname);
        ui->cn->setFont(QFont("宋体", 10));
    }
    ui->cn->setCurrentIndex(-1);

    //查询
    connect(ui->choose, &QPushButton::clicked, this, [=]()
    {
        QString couname = ui->cn->currentText();
        QString cpath = "";
        for(int i = 0; i < ccsn.size(); i ++){
            if(couname == ccsn[i]){
                cpath = dirsn[i];
                break;
            }
        }
        QString ca = homepage;
        ca += "\\";
        ca += cpath;

        QVector<QString> fsn;
        FindBaseFile(ca, fsn);
        showtable(fsn);

        QFile op("d:\\dataexample\\op.txt");
        op.open(QIODevice::Append);
        QTextStream opp(&op);
        opp.setCodec(QTextCodec::codecForName("utf-8"));
        QString gg = GetTime();
        QString hh = " 资料查询";
        QString oppr="学生学号：";
        opp << gg << " " <<oppr << *xuehao << hh << " " << couname << "\n";
        op.close();
    });

    //排序
    ui->sorttype->addItem("资料名称");
    ui->sorttype->setCurrentIndex(-1);

    connect(ui->sort, &QPushButton::clicked, this, [=]()
    {
        QString st = ui->sorttype->currentText();
        if(st == "资料名称"){
            QVector<QString> ssn;
            FindBaseFile("d:\\dataexample\\course", ssn);
            sortname(ssn);
            showtable(ssn);

            QFile op("d:\\dataexample\\op.txt");
            op.open(QIODevice::Append);
            QTextStream opp(&op);
            opp.setCodec(QTextCodec::codecForName("utf-8"));
            QString gg = GetTime();
            QString hh = " 资料排序";
            QString oppr = "学生学号：";
            opp << gg << " " << oppr << *xuehao << hh << "\n";
            op.close();
        }

    });

    //上传
    connect(ui->docup, &QPushButton::clicked, this, [=]()
    {
        updoc uppage(c);
        connect(&uppage, &updoc::upret, this, [=]()
        {
            QVector<QString> ssn;
            FindBaseFile("d:\\dataexample\\course", ssn);
            showtable(ssn);
        });
        uppage.exec();
    });

    //下载
    connect(ui->docdown, &QPushButton::clicked, this, [=]()
    {
        down downpage(c);
        downpage.exec();
    });

}

document::~document()
{
    delete ui;
}

void document::showtable(QVector<QString> filesn)
{
    int xx = ui->docexist->rowCount();
    for(int i = 0; i < xx; i ++){
        ui->docexist->removeRow(0);
    }

    QStringList header;
    header << "资料名称";
    ui->docexist->setColumnCount(1);
    ui->docexist->setHorizontalHeaderLabels(header);
    ui->docexist->verticalHeader()->setVisible(false);

    for(int i = 0; i < filesn.size(); i ++){
        ui->docexist->insertRow(ui->docexist->rowCount());
        ui->docexist->setItem(i, 0, new QTableWidgetItem(filesn[i]));
    }
    ui->docexist->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->docexist->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->docexist->setShowGrid(false);
}

void document::sortname(QVector<QString> &ssn)
{
    QString temp = "";
    for(int i = 0; i < ssn.size()-1; i ++){
        for(int j = 0; j < ssn.size()-1-i; j ++){
            if((ssn[j])[0] > (ssn[j+1])[0]){
                temp = ssn[j+1];
                ssn[j+1] = ssn[j];
                ssn[j] = temp;
            }
        }
    }
}

/*
void document::sendslot()
{
    emit dret();
}

void document::gotodown()
{
    downpage.show();
    this->hide();
}

void document::downslot()
{
    downpage.hide();
    this->show();
}

void document::gotoup()
{
    uppage.show();
    this->hide();
}

void document::upslot()
{
    uppage.hide();
    this->show();
}
*/
