#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QDebug>

#include "xmlreadwrite.h"

namespace Ui {
class MainWidget;
}

/**
 * @brief The Node class 登陆信息的结构
 */
class Node
{
public:
    QString docRootName;        //文档根节点名称
    QString recordRootName;     //每条记录的根节点名称
    //记录信息的节点名称
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
    bool isFileSelected();              //判断是否选中文件，未选中文件会弹出提示

    bool checkInfo(int i=0);            //判断登录信息LogInfo哪些可以输入为空，默认全部不能为空，i=1则只有登录名不可为空

    void fillInfo();                    //将界面信息存入结构体中

    void pullInfo(QMap<QString, QString>& info);//将节点信息取出显示到界面上

    void infoClear();                   //清空除登录名之外的控件内容

private slots:

    void on_SelectBtn_clicked();

    //================下方四个函数为登陆信息的增删改查================
    void addInfo();//所有信息均不能为空，删，改，查只需要登录名不为空即可

    void delInfo();//应该根据输入的登录名LogName删除其所在的整条登录信息

    void updateInfo();//和addInfo做区别，可以修改除登录名之外的信息为空值

    void findInfo();//只需登录名

    //===================以下为xml读写和某个节点的增删改查============
    void on_btnRead_clicked();

    void on_btnWrite_clicked();

    void on_btnAddSon_clicked();

    void on_btnInsertB_clicked();

    void on_btnInsertA_clicked();

    void on_btnDelNode_clicked();

    void on_btnUpdateNode_clicked();

    void on_btnFindNode_clicked();


private:
    Ui::MainWidget *ui;

    QString fileName;

    LogInfo logInfo;

    Node* nName;

    QString nodeName;

    QString nodeValue;

    QString refNodeName;

    QVector<QPair<QString, QString> > attrs;

};

#endif // MAINWIDGET_H
