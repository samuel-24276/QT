#include "xmlreadwrite.h"

XMLReadWrite::XMLReadWrite(QObject *parent, QString fName) : QObject(parent), fileName(fName)
{

}

QVector<QString> XMLReadWrite::read()
{    
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return readInfo;
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return readInfo;
    }
    file.close();
    QDomElement root = doc.documentElement();
    if(root.isNull())
        return readInfo;
    QDomNode node = root.firstChild();
    while(!node.isNull())
    {
        while(node.hasChildNodes())
        {
            node = node.firstChild();
            QString info = node.toElement().text();
            if(info.isEmpty())
                node = node.parentNode();
            readInfo.append(node.toElement().text());
            node = node.nextSibling();
        }
    }
    return readInfo;
}

void XMLReadWrite::write(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
        return;
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml", "version = \"1.0\" encoding = \"UTF-8\"");
    doc.appendChild(instruction);         //写入xml文件头
    QDomElement root = doc.createElement("WriteDemo");
    QDomElement node;
    QDomText text;

    node = doc.createElement("LogName");
    text = doc.createTextNode(QString::fromLocal8Bit("法外狂徒"));
    node.appendChild(text);
    root.appendChild(node);

    node = doc.createElement("LogPwd");
    text = doc.createTextNode("654321");
    node.appendChild(text);
    root.appendChild(node);

    node = doc.createElement(tr("UserInfo"));
    root.appendChild(node);
    QDomElement sonNode;
    sonNode = doc.createElement("UserName");
    sonNode.setAttribute("TrueName", QString::fromLocal8Bit("真实名字"));
    text = doc.createTextNode(QString::fromLocal8Bit("张三"));
    sonNode.appendChild(text);
    node.appendChild(sonNode);
    sonNode = doc.createElement("UserSex");
    text = doc.createTextNode(QString::fromLocal8Bit("男"));
    sonNode.appendChild(text);
    node.appendChild(sonNode);
    doc.appendChild(root);

    QTextStream out(&file);
    doc.save(out, 4);     //保存到文件中
    file.close();
}

void XMLReadWrite::addNode()
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite))
        return;
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement newNode = doc.createElement("dateInfo");        //新节点
    newNode.appendChild(doc.createTextNode("2020-11-03"));

    QDomNode root = doc.documentElement();
    QDomNode last = root.lastChild();

    qDebug()<<root.nodeName()<<last.nodeName();
    root.insertAfter(newNode, last);                        //第二参数必须是调用者的直接子集，才能调用成功
    if(!file.open(QIODevice::WriteOnly))
        return;
    QTextStream out(&file);
    doc.save(out, 4);     //保存到文件中
    file.close();
}

void XMLReadWrite::delNode(QString nodeName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite))
        return;
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();
    QDomElement root = doc.documentElement();
    if(root.isNull())
        return;

    QDomNode node = root.firstChild();
    while(!node.isNull())
    {
        while(node.hasChildNodes())
        {
            node = node.firstChild();
            QString info = node.toElement().text();

            if(info.isEmpty())
                node = node.parentNode();
            if(node.toElement().tagName() == nodeName)
            {
                QDomNode parNode = node.parentNode();
//                qDebug()<<node.nodeName()<<parNode.nodeName();
                if(!parNode.removeChild(node).isNull())         //删除成功，跳出循环，保存当前修改
                    break;
            }
            node = node.nextSibling();
        }
    }

    if(!file.open(QIODevice::WriteOnly))
        return;
    QTextStream out(&file);
    doc.save(out, 4);     //保存到文件中
    file.close();
}

void XMLReadWrite::updateNode(QString oldNodeName, QString newValue)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return;
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();
    QDomElement root = doc.documentElement();
    if(root.isNull())
        return;
    QDomNode node = root.firstChild();
    while(!node.isNull())
    {
        while(node.hasChildNodes())
        {
            node = node.firstChild();
            QString info = node.toElement().text();
            if(info.isEmpty())
                node = node.parentNode();
            if(node.toElement().tagName() == oldNodeName)
            {
                QDomNode parNode = node.parentNode();
                QDomNode newNode = doc.createElement(node.nodeName());
                newNode.appendChild(doc.createTextNode(newValue));
                parNode.replaceChild(newNode, node);
                if(!file.open(QIODevice::WriteOnly))
                    return;
                QTextStream out(&file);
                doc.save(out, 4);     //保存到文件中
                file.close();
                return;
            }
            node = node.nextSibling();
        }
    }
}

QDomNode XMLReadWrite::searchNode(QString nodeName)
{
    QDomNode node;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return node;
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return node;
    }
    file.close();
    QDomElement root = doc.documentElement();
    if(root.isNull())
        return node;

    node = root.firstChild();
    while(!node.isNull())
    {
        while(node.hasChildNodes())
        {
            node = node.firstChild();
            QString info = node.toElement().text();

            if(info.isEmpty())
                node = node.parentNode();
//            qDebug()<<node.nodeName();
            if(node.toElement().tagName() == nodeName)
                return node;
            node = node.nextSibling();
        }
        qDebug()<<"233";
    }
}

UserInfo& XMLReadWrite::toUserInfo()
{
    if(readInfo.size()<=0)
        return userInfo;
    userInfo.LogName = readInfo.at(0);
    userInfo.LogPwd = readInfo.at(1);
    userInfo.UserName = readInfo.at(2);
    userInfo.UserSex = readInfo.at(3);
    userInfo.UserAge = readInfo.at(4).toInt();
    userInfo.UserAddress = readInfo.at(5);
    return userInfo;
}
