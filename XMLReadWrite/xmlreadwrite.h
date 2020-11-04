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

struct UserInfo
{
    QString LogName;
    QString LogPwd;
    QString UserName;
    QString UserSex;
    int UserAge;
    QString UserAddress;
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

    bool addNode(QString nodeName, QString nodeValue, QVector<QPair<QString, QString> >& attrs);          //在root节点最后新增节点，第三参数存储属性名及属性值

//    bool addNodeBefore(QString nodeName, QString nodeValue, QString refNodeName);

//    bool addNodeAfter(QString nodeName, QString nodeValue, QString refNodeName);

    bool delNode(QString nodeName);//通过节点名称删除该节点

    bool updateNode(QString oldNodeName, QString newValue);//修改节点名称为oldNodeName的节点值

    QDomNode findNode(QString nodeName);//通过节点名称查询节点信息

    UserInfo& toUserInfo();

    //===============老版函数==================
    void addNode_Obsolete(QString nodeName, QString nodeValue, QVector<QPair<QString, QString> >& attrs);          //在root节点最后新增节点，第三参数存储属性名及属性值

    bool delNode_Obsolete(QString nodeName);//通过节点名称删除该节点

    bool updateNode_Obsolete(QString oldNodeName, QString newValue);//修改节点名称为oldNodeName的节点值

    QDomNode findNode_Obsolete(QString nodeName);//通过节点名称查询节点信息

private:

    QString fileName;

    QVector<QString> readInfo;

    UserInfo userInfo;

signals:

public slots:
};

#endif // XMLREADWRITE_H
