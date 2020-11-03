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
//    UserInfo info = xmlRW->toUserInfo();
//    ui->plainTextEdit->appendPlainText(info.LogName);
//    ui->plainTextEdit->appendPlainText(info.LogPwd);
//    ui->plainTextEdit->appendPlainText(info.UserName);
//    ui->plainTextEdit->appendPlainText(info.UserSex);
//    ui->plainTextEdit->appendPlainText(QString("%1").arg(info.UserAge));
//    ui->plainTextEdit->appendPlainText(info.UserAddress);

    xmlRW->addNode();                               //增
    xmlRW->delNode("UserAge");                      //删
    xmlRW->updateNode("UserAddress", QString::fromLocal8Bit("齐齐哈尔"));//改
    QDomNode node = xmlRW->searchNode("UserAddress");//查，通过节点名称查询该节点，返回该节点副本，可以通过副本查询节点属性，节点值
    qDebug()<<node.toElement().tagName()<<node.toElement().text();
}
