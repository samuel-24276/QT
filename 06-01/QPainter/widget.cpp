#include "widget.h"
#include "ui_widget.h"
#include<QPainter>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    posX = 0;

    //点击按钮,图片移动
    connect(ui->btn_move, &QPushButton::clicked, [=](){
        //手动调用paintEvent()函数,update()
        update();
    });
}

void Widget::paintEvent(QPaintEvent *)
{
    //声明画家对象，在窗口设备中绘制图片
    QPainter painter(this);

    //图片超出屏幕,再设置回来
    if(posX>this->width())
        posX = 0;
    posX += 5;
    //利用画家画图片
    painter.drawPixmap(posX, 100, QPixmap(":/img/save.svg"));


//    painter.drawEllipse(QPoint(100,100), 50, 50);
//    //设置抗锯齿 效率低
//    painter.setRenderHint(QPainter::Antialiasing);
//    painter.drawEllipse(QPoint(250,100), 50, 50);

//    painter.drawRect(QRect(20, 20, 50, 50));

//    //移动画家
//    painter.translate(100, 0);

//    painter.save();//保存画家状态

//    painter.drawRect(QRect(20, 20, 50, 50));

//    painter.translate(100, 0);

//    //还原画家状态,即还原到刚才保存的状态,所以第2,3个矩形会重叠
//    painter.restore();

//    painter.drawRect(QRect(20, 20, 50, 50));


//    //指定画笔
//    QPen pen(QColor(255, 0, 0));
//    //设置笔宽
//    pen.setWidth(2);
//    //设置风格
//    pen.setStyle(Qt::DotLine);
//    //让画家拿起红色笔
//    painter.setPen(pen);

//    //画刷给封闭图形填充颜色
//    QBrush brush(Qt::blue);
//    //设置画刷风格
//    brush.setStyle(Qt::CrossPattern);
//    //让画家拿起画刷
//    painter.setBrush(brush);

//    //画线
//    painter.drawLine(QPoint(0,0), QPoint(100,100));

//    //画圆
//    painter.drawEllipse( QPoint(100,100), 50,50);

//    //画矩形
//    painter.drawRect(QRect(50, 50, 30, 30));

//    //画文字
//    painter.drawText(QRect(0, 150, 300, 100),"多情自古空余恨，此恨绵绵无绝期");
}

Widget::~Widget()
{
    delete ui;
}
