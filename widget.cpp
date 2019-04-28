#include "widget.h"
#include "ui_widget.h"
#include <QToolButton>
#include "dialowin.h"
#include <QMessageBox>
#include <QHostAddress>
#include <QDebug>
#include <QDateTime>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //窗口标题
    this->setWindowTitle("学生视力保护系统 V1.0");
    //设置窗口图标
    this->setWindowIcon(QIcon(":/icon/eye.jpg"));


    //姓名列表
    QStringList  nameList;
    nameList << "张一" << "张二" << "张三" << "张四" << "张五" << "张六"
             << "张七" << "张八" << "张九" << "张十" << "张十一" << "张十二" << "张十三";

    //头像列表
    QStringList iconList;
    iconList << "zhangyi" << "zhange" << "zhangsan" << "zhangsi" << "zhangwu"
             << "zhangliu" << "zhangqi" << "zhangba" << "zhangjiu" << "zhangshi"
             << "zhangshiyi" << "zhangshier" << "zhangshisan";

    //ip列表
    QStringList  addList;
    addList << "192.168.43.88" << "xx" << "xx" << "xx" << "xx" << "xx"
            << "xx"<< "xx" << "xx" << "xx" << "xx" << "xx" << "xx";

    //port
     qint16 port = 6666;

     //错误次数,弹窗信息容器初始化
     for(int i=0;i<13;i++)
     {

         wrongCount.push_back(0);
         wrongMes.push_back(000);
     }




    for(int i=0;i<13;i++)
    {
        //创建新按钮
        QToolButton * btn = new QToolButton;
         //设置按钮名称
        btn->setText(nameList[i]);
        //设置图片
        QString iconname = QString(":/icon/%1.jpg").arg(iconList.at(i));
        btn->setIcon(QPixmap(iconname));
        //设置图片大小
        btn->setIconSize(QPixmap(iconname).size());
        //设置图片透明效果
        btn->setAutoRaise(true);
        //设置按钮风格，同时显示文字和图标
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        //设置按钮名称
        ui->stuLayout->addWidget(btn);
        //将6个按钮放入到布局中
        stuBtn.push_back(btn);
        //打开标志置为false
        isShow.push_back(false);


        //创建套接字
        QTcpSocket * tcpClient = new QTcpSocket(this);

        //存放入套接字容器中
        client.push_back(tcpClient);

        //设置缓冲区大小100
        tcpClient->setReadBufferSize(100);

        //连接到服务器
        tcpClient->connectToHost(addList[i],port);

        //时间
        QString time = QDateTime::currentDateTime().toString("hh:mm:ss");


        //关联三种信号，连接，断开，收到消息
        connect(client[i],&QTcpSocket::connected,[=]()
        {
                ui->mesPlainText->appendPlainText("[" + nameList[i] + "]" + time + ":已经连接上了");
                stuNumCount(0);



         });

         connect(client[i],&QTcpSocket::disconnected,[=](){

                ui->mesPlainText->appendPlainText("[" + nameList[i] + "]" + time + "断开连接");
                stuNumCount(1);
                qDebug() << "断开连接";

          });

         connect(client[i],&QTcpSocket::readyRead,[=](){

                while(client[i]->canReadLine())
                {

                    //解包，处理数据

                    handleMes(client[i],nameList[i]);

                }

          });

         //开灯，关灯按钮
         connect(ui->kaiBtn,&QPushButton::clicked,[=](){

             ui->kaiBtn->setEnabled(false);
             ui->guanBtn->setEnabled(true);

             QString msg = QString("1");
             QByteArray str = msg.toUtf8();
             str.append('\n');
             client[i]->write(str);
         });

         connect(ui->guanBtn,&QPushButton::clicked,[=](){

             ui->kaiBtn->setEnabled(true);
             ui->guanBtn->setEnabled(false);

             QString msg = QString("0");
             QByteArray str = msg.toUtf8();
             str.append('\n');
             client[i]->write(str);
         });




    }

    //弹出窗口，避免窗口多次，重复弹出
    for(int i=0;i<stuBtn.size();i++)
    {
        connect(stuBtn[i],&QToolButton::clicked,[=](){
            //判断窗口是否已经打开
            if(isShow[i])
            {
                QString str =QString("%1已经被打开了").arg(stuBtn[i]->text());
                QMessageBox::warning(this,"警告",str);

                return;

            }

        //以顶层方式弹出窗口
        dialowin * dia = new  dialowin(0,stuBtn[i]->text(),wrongCount[i],wrongMes[i]);
        //设置窗口标题
        dia->setWindowTitle(stuBtn[i]->text());
        //设置窗口图标
        dia->setWindowIcon(stuBtn[i]->icon());
        //弹出
        dia->show();
        //打开标志置true
        isShow[i] = true;

        connect(dia,&dialowin::widgetClose,[=](){isShow[i] = false;});

        });





    }


}




Widget::~Widget()
{
    delete ui;
}


void Widget::handleMes(QTcpSocket * tcpClient,QString name)
{
    QByteArray mes;

    mes = tcpClient->readLine();

    //数据解析
    int head_1,id_1,id_2,mes_1,mes_2,mes_3,mes_4,tail_2;
    char head_2,tail_1;
    //头部数据
    head_1 = mes[0]-48; head_2 = mes[1];
    //设备id
    id_1 = mes[2]-48; id_2 = mes[3]-48;
    //状态信息
    mes_1 = mes[4]-48;  mes_2 = mes[5]-48;  mes_3 = mes[6]-48;  mes_4 = mes[7]-48;
    //尾部信息
    tail_1 = mes[8];    tail_2 = mes[9]-48;


    //校验数据
    if(head_1 == 0 && head_2 == 'x')
    {
        if(tail_1 == 'x' && tail_2 == 0)
        {
            //坐姿不正确添加列
            if(mes_1 == 1)
            {
                //判断是否有此事件
                bool isempty = ui->stuTable->findItems(name,Qt::MatchExactly).isEmpty();

                if(isempty)
                    {
                        //添加事件列
                        QTableWidgetItem *item = new QTableWidgetItem(name);
                        ui->stuTable->insertRow(0);
                        ui->stuTable->setItem(0,0,item);

                        //错误次数加一
                        wrongCount[id_2-1] += 1;

                        int flash = 0;
                        if(mes_2 == 1)
                        {
                            flash += 100;


                        }
                        if(mes_3 == 1)
                        {

                            flash += 10;

                        }

                        if(mes_4 == 1)
                        {

                            flash +=1;
                        }
                        //弹窗信息
                        wrongMes[id_2-1] = flash;


                    }

            }
            //坐姿正确之后删除列
            if(mes_1 == 0)
            {
                bool isempty = ui->stuTable->findItems(name,Qt::MatchExactly).isEmpty();
                if(!isempty)
                {

                    //删除
                    int row = ui->stuTable->findItems(name,Qt::MatchExactly).first()->row();
                    ui->stuTable->removeRow(row);


                 }


             }

          }

      }



}

void Widget::stuNumCount(int num)
{
    static int count;
    switch (num) {

        case 0:
        {
            count++;
            break;
        }
        case 1:
        {
            count--;
            break;
        }

    }

    QString stuNum = QString("在线人数:%1人").arg(count);
    ui->stuNumLabel->setText(stuNum);



}



void Widget::closeEvent(QCloseEvent *event)
{
    //窗口关闭断开所有套接字连接
    for(int i=0;i<client.size();i++)
    {

        client[i]->disconnectFromHost();
    }


}

