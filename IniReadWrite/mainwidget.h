#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>

#include "inireadwrite.h"

namespace Ui {
class MainWidget;
}

struct LogInfo
{
public:
    QString LogName;
    QString LogPwd;
    QString UserName;
    QString UserAge;
    QString UserSex;
    QString UserAddress;
    QString Email;
    QString LogInTime;
    QString LogOutTime;
};

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private slots:

    void on_SelectBtn_clicked();

    void on_btnWrite_clicked();

    void on_btnRead_clicked();

    void on_btnAddP_clicked();

    void on_btnDelP_clicked();

    void on_btnUpdateP_clicked();

    void on_btnFindP_clicked();

    void fillInfo();        //将界面控件上的内容填充至结构体

    void pullInfo();        //将结构体的内容填充至界面上的组件

    bool checkInfo(int mode=0);       //检查信息是否为空,默认是检测所有信息，mode=1只检测登录名

    void infoClear();       //清空界面上的LogInfo内容，刚才输入的LogName不清空

    //=============登陆信息的增删改查============
    void on_btnAddInfo_clicked();

    void on_btnDelInfo_clicked();

    void on_btnUpdateInfo_clicked();

    void on_btnFindInfo_clicked();

private:
    Ui::MainWidget *ui;

    QString fileName;

    QString group;

    QString key;

    QString value;

    LogInfo logInfo;

    LogGroup* logGroup;

private:
    bool isFileSelected();

};

#endif // MAINWIDGET_H
