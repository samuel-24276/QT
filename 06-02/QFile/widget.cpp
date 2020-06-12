#include "widget.h"
#include "ui_widget.h"
#include<QFileDialog>
#include<QDebug>
#include<QFile>
#include<QMessageBox>//提示信息
#include<QTextCodec>
#include<QFileInfo>
#include<QDateTime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //点击按钮，选择文件
    connect(ui->btn, &QPushButton::clicked, [=](){
        //第一参数是打开的窗口，第二参数是窗口名字，第三参数是默认的打开路径,第四参数是选取得文件类型
        QString filePath = QFileDialog::getOpenFileName(this, "选择文件", "D:\\CODE\\QT\\06-02\\QFile", "(*.txt)");
        //qDebug()<<filePath;

        //如果用户取消打开文件
        if(filePath.isEmpty())
        {
            QMessageBox::warning(this, "打开失败", "文件路径为空");
            return;
        }
        //将文件路径放入到LineEdit中
        ui->lineEdit->setText(filePath);

        //将文件中的内容放入TextEdit
        QFile file(filePath);//默认支持格式是utf-8

        //设置文本编码格式
        //QTextCodec * codec = QTextCodec::codecForName("gbk");
        //设置打开方式
        file.open(QIODevice::ReadOnly);
        QByteArray arr;
        //arr = file.readAll();//一次性读取所有文件内容

        while(!file.atEnd())
        {
            arr += file.readLine();//按行读取数据
        }

        ui->textEdit->setText(arr);
        //ui->textEdit->setText(codec->toUnicode(arr));

        //关闭文件对象
        file.close();

        //写文件
//        file.open(QIODevice::Append);//利用追加的方式修改
//        file.write("我是傻逼");
//        file.close();
        //文件信息类
        QFileInfo info(filePath);

        qDebug()<<"大小"<<info.size()<<"后缀名"<<info.suffix()<<"文件路径"<<info.filePath();
        qDebug()<<"创建日期"<<info.created().toString("yyyy/MM/dd hh:mm:ss");
        qDebug()<<"最后修改日期"<<info.lastModified().toString("yyyy/MM/dd hh:mm:ss");
    });
}

Widget::~Widget()
{
    delete ui;
}
