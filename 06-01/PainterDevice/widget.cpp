#include "widget.h"
#include "ui_widget.h"
#include<QPainter>
#include<QPicture>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
//    //QPixmap做绘图设备,对不同平台的设备做显示优化
//    QPixmap pix(300, 300);
//    //更改填充色为白色,默认黑色
//    pix.fill(Qt::white);
//    QPainter painter(&pix);
//    painter.setPen(QPen(Qt::red));
//    painter.drawEllipse(QPoint(150, 150), 100 ,100);
//    //保存
//    pix.save("D:\\pix.png");

//    //QImage做绘图设备
//    QImage  img(300, 300, QImage::Format_RGB32);
//    img.fill(Qt::white);
//    QPainter painter(&img);
//    painter.setPen(QPen(Qt::green));
//    painter.drawEllipse(QPoint(150, 150), 100 ,100);
//    //保存
//    img.save("D:\\img.png");

    QPicture pic;//记录和重现QPainter的各条命令
    QPainter painter;
    painter.begin(&pic);
    painter.setPen(QPen(Qt::blue));
    painter.drawEllipse(QPoint(150, 150), 100 ,100);
    painter.end();

    pic.save("D:\\img.png");

}

void Widget::paintEvent(QPaintEvent * e)
{
//    QImage img;
//    img.load(":/img/new.svg");

//    QPainter painter(this);
//    //在画出图片前修改像素点
//    for (int i=0; i<20; ++i) {
//        for (int j=0;j<20;++j) {
//            QRgb val = qRgb(255, 0, 0);
//            img.setPixel(i, j, val);
//        }
//    }
//    painter.drawImage(0, 0, img);
    //重现QPicture的绘图命令
    QPainter painter(this);
    QPicture pic;
    pic.load("D:\\img.png");

    painter.drawPicture(0, 0, pic);
}

Widget::~Widget()
{
    delete ui;
}
