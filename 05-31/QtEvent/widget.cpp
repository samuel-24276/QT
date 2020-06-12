#include "widget.h"
#include "ui_widget.h"
#include<QTimer>
#include<QDebug>
#include<QMouseEvent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //启动定时器事件
    id1 = startTimer(1000);//定时器间隔为1000毫秒
    id2 = startTimer(2000);//定时器间隔为2秒

    //利用定时器类实现功能
    QTimer * timer = new QTimer(this);
    timer->start(500);
    connect(timer, &QTimer::timeout, [=](){
        static int num=1;
        ui->label_4->setText(QString::number(num++));
    });

    //点击暂停按钮，实现定时器暂停
    connect(ui->btn_pause, &QPushButton::clicked, [=](){
        timer->stop();
    });

    //利用事件过滤器，给事件做更高级的拦截
    //步骤一：给label安装事件过滤器
    ui->label->installEventFilter(this);
    //步骤二：重写eventFilter事件

}

bool Widget::eventFilter(QObject * obj, QEvent * e)
{
    if(obj == ui->label)
    {
        if(e->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent * ev = static_cast<QMouseEvent *>(e);
            QString str = QString("eventFilter事件中鼠标按下 x=%1 y=%2").arg(ev->x()).arg(ev->y());
            qDebug()<<str;
            return true;//代表如果是鼠标按下，用户自己处理
        }
    }

    //默认处理
    return QWidget::eventFilter(obj, e);
}

void Widget::timerEvent(QTimerEvent *e)
{
    if(e->timerId()==id1)
    {
        static int num=1;
        ui->label_2->setText(QString::number(num++));
    }

    if(e->timerId()==id2)
    {
        static int num2=1;
        ui->label_3->setText(QString::number(num2++));
    }
}


Widget::~Widget()
{
    delete ui;
}
