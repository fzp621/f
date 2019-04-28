#ifndef DIALOWIN_H
#define DIALOWIN_H

#include <QWidget>
#include "widget.h"
#include <QVector>


namespace Ui {
class dialowin;
}

class dialowin : public QWidget
{
    Q_OBJECT

public:
    explicit dialowin(QWidget *parent,QString username,int count,int wrongMes);
    ~dialowin();

signals:
    void widgetClose();

protected:
   void closeEvent(QCloseEvent *);

protected:



private:
    Ui::dialowin *ui;
};

#endif // DIALOWIN_H
