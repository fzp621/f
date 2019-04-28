#include "dialowin.h"
#include "ui_dialowin.h"




dialowin::dialowin(QWidget *parent,QString username,int count,int wrongMes) :
    QWidget(parent),
    ui(new Ui::dialowin)
{
    ui->setupUi(this);

    //错误次数显示
    QString mes = QString("%1次").arg(count);
    ui->wrongCountLine->setText(mes);



    //错误信息显示



        if(wrongMes != 0)
        {

            int H = 0 , T = 0, D = 0;

            H = wrongMes/100;

            T = (wrongMes%100)/10;

            D = wrongMes%10;


            QString mes_1 = QString("当前坐姿错误原因:");
            if(H == 1)
            {
                mes_1 += "身体靠桌子太近/";

            }
            if(T == 1)
            {
                mes_1 += "头部靠桌子太近/";

            }
            if(D == 1)
            {
                mes_1 += "身体过于倾斜/";

            }

            ui->wrongPlainText->appendPlainText(mes_1);

        }








}

dialowin::~dialowin()
{
    delete ui;
}

void dialowin::closeEvent(QCloseEvent *e)
{
    emit this->widgetClose();

    QWidget::closeEvent(e);


}
