#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //QTableWidget使用
    //1.设置列
    ui->tableWidget->setColumnCount(3);
    //2.设置水平表头
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"姓名"<<"性别"<<"年龄");
    //3.设置行数
    ui->tableWidget->setRowCount(3);
    //4.设置具体内容
    //ui->tableWidget->setItem(0,0,new QTableWidgetItem("张三"));
    QStringList nameList;
    nameList<<"张三"<<"李四"<<"王五";
    QStringList sexList;
    sexList<<"男"<<"女"<<"男";
    for(int i=0; i<3; ++i)
    {
        int col=0;
        ui->tableWidget->setItem(i,col++,new QTableWidgetItem(nameList[i]));
        ui->tableWidget->setItem(i,col++,new QTableWidgetItem(sexList[i]));
        ui->tableWidget->setItem(i,col++,new QTableWidgetItem(QString::number(18+i)));
    }
}

Widget::~Widget()
{
    delete ui;
}
