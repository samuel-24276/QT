#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QDebug>

#include "xmlreadwrite.h"

namespace Ui {
class MainWidget;
}

class Node
{
public:
    QString docRootName;
    QString recordRootName;
    QString LogName;
    QString LogPwd;
    QString UserInfo;
    QString UserName;
    QString UserAge;
    QString UserSex;
    QString UserAddress;
    QString Email;
    QString LogInTime;
    QString LogOutTime;
public:
    Node();
};

struct LogInfo
{
    QString id;
    QString LogName;        //登录名唯一，不可重名，删除，查询，更新都要通过LogName进行
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

private:

    bool isFileSelected();

    bool checkInfo(int i=0);

    void fillInfo();

private slots:

    void on_SelectBtn_clicked();

    void on_btnRead_clicked();

    void on_btnWrite_clicked();

    void on_btnAddSon_clicked();

    void on_btnInsertB_clicked();

    void on_btnInsertA_clicked();

    void on_btnDelNode_clicked();

    void on_btnUpdateNode_clicked();

    void on_btnFindNode_clicked();

    //================下方为新函数================

    void addInfo();//所有信息均不能为空，删，改，查只需要登录名不为空即可

    void delInfo();//应该根据输入的登录名LogName删除其所在的整条登录信息

    void updateInfo();//和addInfo做区别，可以修改除登录名之外的信息为空值

    void findInfo();//只需登录名

private:
    Ui::MainWidget *ui;

    QString fileName;    

    QString nodeName;

    QString nodeValue;

    QString refNodeName;

    QVector<QPair<QString, QString> > attrs;

    //=========下方为新变量=============

    LogInfo logInfo;

    Node* nName;

    int recordNum;
};

#endif // MAINWIDGET_H
