#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //在新建菜单项上显示图片
//    ui->actionnew->setIcon(QIcon("D:/CODE/QT/05-25/QtResourseFile/img/new.svg"));
    //资源文件的使用方式“ ： + 前缀名 + 文件名”
    ui->actionnew->setIcon(QIcon(":/ /img/new.svg"));
    ui->actionsave->setIcon(QIcon(":/ /img/save.svg"));

}

MainWindow::~MainWindow()
{
    delete ui;
}
