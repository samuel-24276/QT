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

    void addNode(QString nodeName, QString nodeValue, QVector<QPair<QString, QString> >& attrs);          //在root节点最后新增节点，第三参数存储属性名及属性值

//    void addNodeBefore(QString nodeName, QString nodeValue, QString refNodeName);

//    void addNodeAfter(QString nodeName, QString nodeValue, QString refNodeName);

    bool delNode(QString nodeName);//通过节点名称删除该节点

    bool updateNode(QString oldNodeName, QString newValue);//修改节点名称为oldNodeName的节点值

    QDomNode findNode2(QString nodeName);   //通过nameItem()函数进行节点查找，未完成

    QDomNode findNode(QString nodeName);//通过节点名称查询节点信息，只能查询2个<>的，查询1个<>的会引起异常，查询不到内容好像也会引起异常

    UserInfo& toUserInfo();

private:

    QString fileName;

    QVector<QString> readInfo;

    UserInfo userInfo;

signals:

public slots:
};

#endif // XMLREADWRITE_H
