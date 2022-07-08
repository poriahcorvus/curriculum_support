#include "down.h"
#include "ui_down.h"
#include "dealfile.h"
#include "course.h"
#include "courseku.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QTextCodec>
#include"quanju.h"

down::down(courseKu *c, QDialog *parent) :
    QDialog(parent),
    ui(new Ui::down)
{
    ui->setupUi(this);
    this->resize(QSize(800, 600));

    //connect(ui->downback, &QPushButton::clicked, this, &down::sendslot);
    QVector<QString> filesn;
    FindFilePath("d:\\dataexample\\course", filesn);
    QVector<QString> sn;
    for(int i = 0; i < filesn.size(); i ++){
        QFileInfo info(filesn[i]);
        sn.push_back(info.completeBaseName());
        ui->docselect->addItem(sn[i]);
        ui->docselect->setFont(QFont("宋体", 10));
    }
    ui->docselect->setCurrentIndex(-1);
    connect(ui->downdown, &QPushButton::clicked, this, [=]()
    {
        QString cat = ui->docselect->currentText();
        QString catalog = "";
        //int kk = ui->docselect->currentIndex();
        //QString cat = filesn[kk];
        for(int i = 0; i < filesn.size(); i ++){
            if(cat == sn[i])
                catalog = filesn[i];
        }
        int a = uncompress(catalog);
        qDebug() << a;
        if(a == -1){
            QMessageBox::critical(this, "错误信息", "下载失败！", QMessageBox::Close);

            QFile op("d:\\dataexample\\op.txt");
            op.open(QIODevice::Append);
            QTextStream opp(&op);
            opp.setCodec(QTextCodec::codecForName("utf-8"));
            QString gg = GetTime();
            QString hh = " 资料下载失败";
            QString oppr="学生学号：";
            opp << gg << " " << oppr << *xuehao << hh << " " << cat << "\n";
            op.close();
        }
        else if(a == 1){
            QMessageBox::information(this, "提醒", "下载成功！", QMessageBox::Close);

            QFile op("d:\\dataexample\\op.txt");
            op.open(QIODevice::Append);
            QTextStream opp(&op);
            opp.setCodec(QTextCodec::codecForName("utf-8"));
            QString gg = GetTime();
            QString hh = " 资料下载成功";
            QString oppr="学生学号：";
            opp << gg << " " << oppr << *xuehao << hh << " " << cat << "\n";
            op.close();
        }
    });
}

down::~down()
{
    delete ui;
}

void down::sendslot()
{
    emit downret();
}
