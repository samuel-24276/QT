#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_OpenBtn_clicked()
{
    fileName = QFileDialog::getOpenFileName();
    xmlRW = new XMLReadWrite(this, fileName);
    QString writeName = fileName.split(".")[0] + "Write.xml";
    xmlRW->write(writeName);
    ui->lineEdit->setText(fileName);
    QVector<QString> vecInfo = xmlRW->read();
//    qDebug()<<vecInfo;
//    UserInfo info = xmlRW->toUserInfo();
//    ui->plainTextEdit->appendPlainText(info.LogName);
//    ui->plainTextEdit->appendPlainText(info.LogPwd);
//    ui->plainTextEdit->appendPlainText(info.UserName);
//    ui->plainTextEdit->appendPlainText(info.UserSex);
//    ui->plainTextEdit->appendPlainText(QString("%1").arg(info.UserAge));
//    ui->plainTextEdit->appendPlainText(info.UserAddress);

    QVector<QPair<QString, QString> > attrs;
    QPair<QString, QString> p;
    p.first = "year";
    p.second = "2020";
    attrs.append(p);
    p.first = "month";
    p.second = "11";
    attrs.append(p);
    p.first = "day";
    p.second = "04";
    attrs.append(p);
    xmlRW->addNode("DateInfo", "2020.11.13", attrs);                               //增
    if(xmlRW->delNode("DateInfo"))                      //删
        qDebug()<<"del success";
    else
        qDebug()<<"del failed";
    if(xmlRW->updateNode("UserAge", "28"))            //改
        qDebug()<<"update success";
    else
        qDebug()<<"update failed";
    QDomNode node = xmlRW->findNode("UserSex");//查，通过节点名称查询该节点，返回该节点副本，可以通过副本查询节点属性，节点值
    if(!node.isNull())
        qDebug()<<"find success"<<node.nodeName()<<node.firstChild().nodeValue();
    else
        qDebug()<<"find failed";
}
