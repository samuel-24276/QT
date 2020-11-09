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

/**
 * @brief The XMLReadWrite class xml读写，增删改查类
 */
class XMLReadWrite : public QObject
{
    Q_OBJECT
public:
    explicit XMLReadWrite(QObject *parent = 0, QString fName="");

    bool addRecordNode(QString nodeName, QString recordRootId);     //添加记录节点

    //添加非叶节点，需要知道其name、属性、父节点名称和所在记录的id即可
    bool addSonNodeById(QString nodeName, QVector<QPair<QString, QString> >& attrs, QString refNodeName, QString recordRootId);    

    //添加叶节点，因此需要知道其name、value、属性和父节点名称，及所在记录的id
    bool addElementById(QString nodeName, QString nodeValue, QVector<QPair<QString, QString> >& attrs, QString refNodeName, QString recordRootId);

    bool delRecordNode_complete(QString nodeName);     //删除整条记录节点

    QString updateRecordNode_complete(QString nodeName);//修改整条记录节点,实际只是删除后新增，且新增在上层完成，本方法只负责删除记录后返回此条记录的id

    QDomNode findRecordNode_complete(QString nodeName);     //根据登录名查询整条记录节点

    QMap<QString, QString> parseNode(QDomNode& root);         //读出某个节点root及其子节点的信息，存储到QMap中

    //=======================================以下为基本函数，组合使用可达到复杂功能（不含ID的情况下）======================================================================================
    QVector<QString> read();

    void write(QString fileName);

    bool addSonNode(QString nodeName, QString nodeValue, QString refNodeName, QVector<QPair<QString, QString> >& attrs);          //在root节点最后新增节点，第三参数存储属性名及属性值

    bool insertNodeBefore(QString nodeName, QString nodeValue, QString refNodeName, QVector<QPair<QString, QString> > &attrs);

    bool insertNodeAfter(QString nodeName, QString nodeValue, QString refNodeName, QVector<QPair<QString, QString> > &attrs);

    bool delNode(QString nodeName);//通过节点名称删除该节点

    bool updateNode(QString oldNodeName, QString newValue, QVector<QPair<QString, QString> >& attrs);//修改节点名称为oldNodeName的节点值

    QDomNode findNode(QString nodeName);//通过节点名称查询节点，若知道节点属性名，也可获得属性值，但是无法显示所有属性的键值对

    QDomNode lastNode();                //获取xml文件根节点下的最后一个节点，进而可获取其id，后续可用于添加节点时确定被添加节点的id

    bool getDoc(QString fileName, QDomDocument& doc);       //打开xml文件，以QDomDocument形式保存其内容

    bool saveChange(QDomDocument& doc, QFile& file);        //保存对xml文件的修改

private:
    QString fileName;

};

#endif // XMLREADWRITE_H
