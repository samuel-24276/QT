#include "widget.h"
#include "ui_widget.h"
#include<QMovie>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //设置默认页面
    ui->stackedWidget->setCurrentIndex(0);
//    //ScrollArea切换
//    connect(ui->btn_scroll, &QPushButton::clicked, [=](){
//        ui->stackedWidget->setCurrentIndex(0);
//    });
//    //TabWidget切换
//    connect(ui->btn_tab, &QPushButton::clicked, [=](){
//        ui->stackedWidget->setCurrentIndex(1);
//    });
//    //Tool Box切换
//    connect(ui->btn_tool, &QPushButton::clicked, [=](){
//        ui->stackedWidget->setCurrentIndex(2);
//    });
//    //下拉框控件
//    ui->comboBox->addItem("奔驰");
//    ui->comboBox->addItem("宝马");
//    ui->comboBox->addItem("路虎");
//    //点击路虎按钮，定位到路虎
//    connect(ui->btn_combo, &QPushButton::clicked,[=](){
//        //索引定位
//        //ui->comboBox->setCurrentIndex(2);
//        //确定路虎存在的话
//        ui->comboBox->setCurrentText("路虎");
//    });
    //利用Label显示图片
    ui->label_img->setPixmap(QPixmap(":/img/new.svg"));
    //利用Label显示动图
    QMovie * mv = new QMovie(":/img/save.svg");
    ui->label_img->setMovie(mv);
    //播放
    mv->start();

}

Widget::~Widget()
{
    delete ui;
}
