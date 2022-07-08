#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QWidget>
#include <QDialog>
#include "down.h"
#include "updoc.h"
#include "course.h"
#include "courseku.h"

namespace Ui {
class document;
}

class document : public QDialog
{
    Q_OBJECT

public:
    explicit document(courseKu *c, QDialog *parent = 0);
    ~document();
    void showtable(QVector<QString>filesn);
    void sortname(QVector<QString>&ssn);
    void gotodown();
    void downslot();
    void gotoup();
    void upslot();
    void sendslot();
signals:
    void dret();
public slots:

private:
    Ui::document *ui;
    //down downpage;
    //updoc uppage;
};

#endif // DOCUMENT_H
