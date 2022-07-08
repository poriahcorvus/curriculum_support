#ifndef NAVDIALOG_H
#define NAVDIALOG_H

#include <QDialog>
#include<QFile>
#include<QString>
#include<QTextStream>
#include<QDebug>
#include<QString>
#include<vector>
#include<queue>
#include<QPainter>
using namespace std;
namespace Ui {
class navdialog;
}

class navdialog : public QDialog
{
    Q_OBJECT

public:
    explicit navdialog(int start_index=0,int end_index=0,int way_index=0,QWidget *parent = nullptr);
    ~navdialog();

private:
    Ui::navdialog *ui;
public:
    //绘图事件
    void paintEvent(QPaintEvent *);
};

#endif // NAVDIALOG_H
