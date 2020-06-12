#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);//a是应用程序对象，Qt中有且仅有一个程序对象
    Widget w;                   //创建一个窗口对象
    w.show();                   //窗口对象默认不会弹出，需要调用show()函数
    return a.exec();            //a.exec()进入消息循环机制，不然的话窗口会消失
}
