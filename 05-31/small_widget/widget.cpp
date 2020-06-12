#include "widget.h"
#include "ui_widget.h"
#include<QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //设置值为一半
    connect(ui->btn_sethalf, &QPushButton::clicked, [=](){
        ui->small_widget->setData(50);
    });
    //获取当前值
    connect(ui->btn_get, &QPushButton::clicked, [=](){
        qDebug()<<ui->small_widget->getData();
    });
}

Widget::~Widget()
{
    delete ui;
}
