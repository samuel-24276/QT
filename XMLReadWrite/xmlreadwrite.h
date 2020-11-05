#ifndef XMLREADWRITE_H
#define XMLREADWRITE_H

#include <QObject>
#include <QDebug>
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>
#include <QDomNode>
#include <QDomEntity>
#include <QXmlStreamReader>
#include <QFile>
#include <QMessageBox>

struct LogInfo
{
    QString LogName;
    QString LogPwd;
    QString UserName;    
    int UserAge;
    QString UserSex;
    QString UserAddress;
    QString Email;
    QString LogInTime;
    QString LogOutTime;
};

/**
 * @brief The XMLReadWrite class xml读写，增删改查类，增删改查均缺少对异常值（不存在的值）的处理，缺少对单<>元素的处理
 */
class XMLReadWrite : public QObject
{
    Q_OBJECT
public:
    explicit XMLReadWrite(QObject *parent = 0, QString fName="");

    QVector<QString> read();

    void write(QString fileName);

    bool addSonNode(QString nodeName, QString nodeValue, QString refNodeName, QVector<QPair<QString, QString> >& attrs);          //在root节点最后新增节点，第三参数存储属性名及属性值

    bool insertNodeBefore(QString nodeName, QString nodeValue, QString refNodeName, QVector<QPair<QString, QString> > &attrs);

    bool insertNodeAfter(QString nodeName, QString nodeValue, QString refNodeName, QVector<QPair<QString, QString> > &attrs);

    bool delNode(QString nodeName);//通过节点名称删除该节点

    bool updateNode(QString oldNodeName, QString newValue, QVector<QPair<QString, QString> >& attrs);//修改节点名称为oldNodeName的节点值

    QDomNode findNode(QString nodeName);//通过节点名称查询节点，若知道节点属性名，也可获得属性值，但是无法显示所有属性的键值对

    LogInfo& toLogInfo();

private:

    QString fileName;

    QVector<QString> readInfo;      //在read()和toUserInfo()方法中使用

    LogInfo logInfo;
};

#endif // XMLREADWRITE_H
