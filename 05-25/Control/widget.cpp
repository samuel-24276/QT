#include "widget.h"
#include "ui_widget.h"
#include<QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //单选按钮，默认选中男
    ui->radioButton->setChecked(true);
    ui->radioButton_4->setChecked(true);
    //捕获复选框的选中信号
    connect(ui->checkBox, &QCheckBox::stateChanged, [=](int state){
        qDebug()<<"state="<<state;
    });
    //ListWidget
    QListWidgetItem * item = new QListWidgetItem("悯农");
    ui->listWidget->addItem(item);
    item->setTextAlignment(Qt::AlignHCenter);//水平居中
    //添加字符串列表，但无法进行对齐
    QStringList list;
    list<<"锄禾日当午"<<"汗滴禾下土"<<"谁知盘中餐"<<"粒粒皆辛苦";
    ui->listWidget->addItems(list);
}

Widget::~Widget()
{
    delete ui;
}
