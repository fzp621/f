#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QVector>
#include <QToolButton>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    enum MsgType {Msg,UsrEnter,UsrLeft};

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

public:
    //存放打开标志
    QVector<bool>isShow;
    //存放ToolButoon
    QVector<QToolButton *> stuBtn;
    //存放tcp套接字
    QVector<QTcpSocket *>  client;
    //存放客户端id
    QVector<int> clientID;
    //存放错误次数
    QVector<int> wrongCount;
    //存放弹窗显示信息
     QVector<int> wrongMes;


private:
    Ui::Widget *ui;

protected:
    //解包
    void handleMes(QTcpSocket *tcpClient,QString name);
    //在线人数统计
    void stuNumCount(int num);

protected:
    void closeEvent(QCloseEvent *event);

//signals:
//    void wrongMes();
//public slots:
//    void set





};

#endif // WIDGET_H
