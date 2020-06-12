#include "mylabel.h"
#include<QDebug>
#include<QMouseEvent>

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{
    this->setMouseTracking(true);
}

//鼠标进入
void MyLabel::enterEvent(QEvent *)
{
    qDebug()<<"鼠标进入";
}

//鼠标离开
void MyLabel::leaveEvent(QEvent *)
{
    qDebug()<<"鼠标离开";
}

//鼠标按下
void MyLabel::mousePressEvent(QMouseEvent *ev)
{
    //只有鼠标左键按下才输出
//    if(ev->button()==Qt::LeftButton)
//    {
        QString str = QString("鼠标按下 x=%1 y=%2").arg(ev->x()).arg(ev->y());
        qDebug()<<str;
//    }
}

//鼠标释放
void MyLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    //只有鼠标左键按下才输出
//    if(ev->button()==Qt::LeftButton)
//    {
        //字符串格式化,使用%加给变量数字占位
        QString str = QString("鼠标释放 x=%1 y=%2").arg(ev->x()).arg(ev->y());
        qDebug()<<str;
//    }
}

//鼠标移动
void MyLabel::mouseMoveEvent(QMouseEvent *ev)
{
    //只有鼠标左键按下才输出
    //if(ev->button()==Qt::LeftButton)//移动过程中左右键可以切换，因此不输出
//    if(ev->buttons() & Qt::LeftButton)//位与操作，保证只有左键移动才输出
    //在mousePressEvent中找到Public Functions下的button(),点入后再点see also可以看到各种按键的状态码
//    {
        //字符串格式化,使用%加给变量数字占位
        QString str = QString("鼠标移动 x=%1 y=%2").arg(ev->x()).arg(ev->y());
        qDebug()<<str;
//    }
}

//利用事件分发器进行拦截
bool MyLabel::event(QEvent* e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent * ev = static_cast<QMouseEvent*>(e);
        QString str = QString("event事件中鼠标按下 x=%1 y=%2").arg(ev->x()).arg(ev->y());
        qDebug()<<str;
        return true;//代表如果是鼠标按下，用户自己处理
    }
    //其他事件交给父类处理
    return QLabel::event(e);
}
