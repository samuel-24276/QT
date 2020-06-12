#include "widget.h"
#include "ui_widget.h"
#include "mypushbutton.h"
#include<QPushButton>
#include<QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QPushButton *btn = new QPushButton;  //创建按钮对象
    //btn->show();                         //show()函数默认是顶层方式弹出窗口
    btn->setParent(this);
    //显示文本
    btn->setText("按钮");     //若中文乱码，则工具->选项->文本编辑器->默认编码格式修改为UTF-8
    //创建第二个按钮
    QPushButton *btn2 = new QPushButton("按钮2");
    btn2->setParent(this);
    //移动按钮
    btn2->move(100,100);
    //设置窗口大小
    this->resize(600, 400);
    //设置窗口标题
    this->setWindowTitle("第一个项目");
    //设置固定窗口大小
    this->setFixedSize(600, 400);

    //创建自定义按钮
    MyPushButton * mybtn = new MyPushButton;
    mybtn->setText("自定义按钮");
    mybtn->move(100,50);
    mybtn->setParent(this);         //设置了父窗口，将控件放入到对象树上，不用管理释放了

    //需求：点击自定义按钮，把窗口关掉
    //利用connect进行链接：参数1 信号发送者， 参数2 发送的信号， 参数3 信号接收者， 参数4 处理信号（槽函数）
    //connect(mybtn, &QPushButton::clicked, this, &QWidget::close);
    connect(mybtn, &MyPushButton::clicked, this, &Widget::close);

}

Widget::~Widget()
{
    qDebug()<<"Widget析构函数调用"<<endl;
    delete ui;
}

