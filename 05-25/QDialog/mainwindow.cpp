#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QAction>
#include<QDebug>
#include<QDialog>
#include<QMessageBox>
#include<QFileDialog>
#include<QColorDialog>
#include<QFontDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //需求：点击新建，弹出对话框
    connect(ui->actionnew, &QAction::triggered, [=](){
        qDebug()<<"弹出对话框";
        //对话框分为模态对话框和非模态对话框。
        //模态对话框     不能对其他窗口进行操作（阻塞功能实现），但可以继续直接敲代码
//        QDialog dlg(this);
//        dlg.resize(120, 60);
//        dlg.exec();     //阻塞功能
        //非模态对话框    可以对其他窗口进行操作，但不能继续直接敲代码，否则会有警报声。
//        QDialog dlg2(this);     //在Lambda创建对象，执行完了对象就没了，因此一闪而过
//        dlg2.resize(120, 60);
//        dlg2.show();
//        QDialog * dlg3 = new QDialog(this);     //在Lambda创建对象，执行完了对象就没了，因此一闪而过
//        dlg3->resize(120, 60);
//        dlg3->show();
//        dlg3->setAttribute(Qt::WA_DeleteOnClose);//55号属性
        //弹出一个QMessageBox对话框
        //错误提示对话框
        //QMessageBox::critical(this, "错误", "error");//静态成员函数通过类调用
        //信息提示对话框
        //QMessageBox::information(this, "信息提示", "information");
        //警告对话框
        //QMessageBox::warning(this, "警告", "warning");
        //提问对话框,参数4 按键类型，有ok,open,save,cancle,apply,close,discard,reset，help等；
        //参数5 关联回车按键的选择
        if(QMessageBox::Save ==QMessageBox::question(this, "提问", "question", QMessageBox::Save | QMessageBox::Cancel, QMessageBox::Save))
        {
            qDebug()<<"保存";
        }
        else {
            qDebug()<<"取消";
        }

    });
    connect(ui->actionopen, &QAction::triggered, [=](){
        //文件对话框 参数1 父窗口 参数2 标题 参数3 默认打开路径 参数4 过滤后缀名
        QString fileName = QFileDialog::getOpenFileName(this, "文件", "C:\\Users\\DEll\\Desktop", "(*.jpg)");
    });
    //颜色对话框
    connect(ui->actioncolor, &QAction::triggered, [=](){
        QColor color = QColorDialog::getColor(QColor(255,0,0));
        qDebug()<<color.red()<<color.green()<<color.blue();
    });
    //字体对话框QFontDialog
    connect(ui->actionfont, &QAction::triggered, [=](){
        bool ok;
        QFont font = QFontDialog::getFont(&ok, QFont("宋体", 36));
        qDebug()<<"字体"<<font.family().toUtf8().data()<<"字号"<<font.pointSize()<<"是否倾斜"<<font.italic()<<"是否加粗"<<font.bold();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
