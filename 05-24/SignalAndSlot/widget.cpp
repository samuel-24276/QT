#include "widget.h"
#include<QPushButton>

//需求：
//Teacher类  Student类
//ClassIsOver下课，老师发送自定义信号：饿了
//学生响应信号，并且请老师吃饭


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //创建老师和学生对象，并将this声明为他们的父窗口
    zt = new Teacher(this);
    st = new Student(this);

    //连接信号和槽
    //connect(zt, &Teacher::hungry, st, &Student::treat);
    //带参的信号和槽函数
    //指针->地址
    //函数指针->函数地址
    void (Teacher:: * teacherSignals )(QString) = &Teacher::hungry;
    void (Student:: * studentSlot )(QString) = &Student::treat;
    connect(zt, teacherSignals, st, studentSlot);
    //重载后无参的信号和槽函数
    void (Teacher:: * teacherSignals1 )() = &Teacher::hungry;
    void (Student:: * studentSlot1 )() = &Student::treat;
    connect(zt, teacherSignals1, st, studentSlot1);

    //信号和槽可以断开连接
    disconnect(zt, teacherSignals1, st, studentSlot1);

    //信号和槽的扩展
    //1.信号可以连接信号
    QPushButton * btn = new QPushButton(this);
    btn->setText("下课");
    this->resize(600, 400);
    void (Teacher:: * teacherSignals2 )() = &Teacher::hungry;
    void (Student:: * studentSlot2 )() = &Student::treat;
    connect(zt, teacherSignals2, st, studentSlot2);
    connect(btn, &QPushButton::clicked, zt, teacherSignals2);

    //2.一个信号连接多个槽函数
    connect(btn, &QPushButton::clicked, this, &Widget::close);

    //3.多个信号连接同一个槽函数

    //4.信号和槽函数的参数类型必须一一对应，参数个数可以不一致

    classIsOver();

    //利用Lambda表达式，实现点击关闭窗口
    QPushButton * btn1 = new QPushButton(this);
    btn1->setText("关闭窗口");
    btn1->move(100, 0);
    connect(btn1, &QPushButton::clicked, this, [=](){
        this->close();
        //st->treat("鱼香肉丝");
    });

}

void Widget::classIsOver()
{
    //自定义信号触发emit
    //emit zt->hungry();
    emit zt->hungry("宫保鸡丁");
    //emit zt->hungry();
}
Widget::~Widget()
{
}

