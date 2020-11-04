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
            if(!node.nextSibling().isNull())
                node = node.nextSibling();
            else
                break;
        }
        //==============这一块代码尚需验证3，4，5层节点的情况
        node = node.parentNode();
        node = node.nextSibling();
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

bool XMLReadWrite::addNode(QString nodeName, QString nodeValue, QVector<QPair<QString, QString> > &attrs)
{
    QDomNode node;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    QDomDocument doc;
    QString errorMsg;
    int errorRow, errorColumn;
    if(!doc.setContent(&file, &errorMsg, &errorRow, &errorColumn))
    {
        QMessageBox::warning(0, "warning", errorMsg+",Row:"+QString("%1").arg(errorRow)+",Column:"+QString("%1").arg(errorColumn));
        file.close();
        return false;
    }
    file.close();

    QDomElement newNode = doc.createElement(nodeName);        //新节点
    newNode.appendChild(doc.createTextNode(nodeValue));
    for(int i=0; i<attrs.size(); ++i)
    {
        QPair<QString, QString> pair = attrs.at(i);
        newNode.setAttribute(pair.first, pair.second);
    }

    QDomElement root = doc.documentElement();
    root.appendChild(newNode);
//    QDomNode last = root.lastChild();

//    qDebug()<<root.nodeName()<<last.nodeName();
//    root.insertAfter(newNode, last);                        //第二参数必须是调用者的直接子集，才能调用成功
    if(!file.open(QIODevice::WriteOnly))
        return false;
    QTextStream out(&file);
    doc.save(out, 4);     //保存到文件中
    file.close();
    return true;
}

bool XMLReadWrite::delNode(QString nodeName)
{
    QDomNode node;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    QDomDocument doc;
    QString errorMsg;
    int errorRow, errorColumn;
    if(!doc.setContent(&file, &errorMsg, &errorRow, &errorColumn))
    {
        QMessageBox::warning(0, "warning", errorMsg+",Row:"+QString("%1").arg(errorRow)+",Column:"+QString("%1").arg(errorColumn));
        file.close();
        return false;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if(root.isNull())
        return false;
    node = root.firstChild();

    while(node!=root)                       //当前节点不是根节点，循环进入子层
    {
        if(node.nodeName() == nodeName)
        {

            QDomNode parNode = node.parentNode();
            if(!parNode.removeChild(node).isNull())         //删除成功，跳出循环，保存当前修改
            {
                if(!file.open(QIODevice::WriteOnly))
                    return false;
                QTextStream out(&file);
                doc.save(out, 4);     //保存到文件中
                file.close();
                return true;
            }
        }
        if(node.firstChild().isElement())   //有孩子节点，继续进入下一层
        {
            node = node.firstChild();
        }
        else if(!node.nextSibling().isNull())//没孩子节点但兄弟节点不空，进入兄弟层
            node = node.nextSibling();
        else                                 //没孩子节点没兄弟节点，需要层层返回（循环）到父层的兄弟节点层去寻找
        {
            while(node.parentNode()!=root && node.nextSibling().isNull())
            {
                node = node.parentNode();
            }
            if(node.nextSibling().isNull())
                node = node.parentNode();
            else
                node = node.nextSibling();
        }
    }
    return false;
}

bool XMLReadWrite::updateNode(QString oldNodeName, QString newValue)
{
    QDomNode node;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    QDomDocument doc;
    QString errorMsg;
    int errorRow, errorColumn;
    if(!doc.setContent(&file, &errorMsg, &errorRow, &errorColumn))
    {
        QMessageBox::warning(0, "warning", errorMsg+",Row:"+QString("%1").arg(errorRow)+",Column:"+QString("%1").arg(errorColumn));
        file.close();
        return false;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if(root.isNull())
        return false;
    node = root.firstChild();

    while(node!=root)                       //当前节点不是根节点，循环进入子层
    {
        if(node.nodeName() == oldNodeName)
        {
            QDomNode parNode = node.parentNode();
            QDomNode newNode = doc.createElement(node.nodeName());
            newNode.appendChild(doc.createTextNode(newValue));
            parNode.replaceChild(newNode, node);
            if(!file.open(QIODevice::WriteOnly))
                return false;
            QTextStream out(&file);
            doc.save(out, 4);     //保存到文件中
            file.close();
            return true;
        }
        if(node.firstChild().isElement())   //有孩子节点，继续进入下一层
        {
            node = node.firstChild();
        }
        else if(!node.nextSibling().isNull())//没孩子节点但兄弟节点不空，进入兄弟层
            node = node.nextSibling();
        else                                 //没孩子节点没兄弟节点，需要层层返回（循环）到父层的兄弟节点层去寻找
        {
            while(node.parentNode()!=root && node.nextSibling().isNull())
            {
                node = node.parentNode();
            }
            if(node.nextSibling().isNull())
                node = node.parentNode();
            else
                node = node.nextSibling();
        }
    }
    return false;
}

QDomNode XMLReadWrite::findNode(QString nodeName)
{
    QDomNode node;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return node;
    QDomDocument doc;
    QString errorMsg;
    int errorRow, errorColumn;
    if(!doc.setContent(&file, &errorMsg, &errorRow, &errorColumn))
    {
        QMessageBox::warning(0, "warning", errorMsg+",Row:"+QString("%1").arg(errorRow)+",Column:"+QString("%1").arg(errorColumn));
        file.close();
        return node;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if(root.isNull())
        return node;
    node = root.firstChild();

    while(node!=root)                       //当前节点不是根节点，循环进入子层
    {
        if(node.nodeName() == nodeName)
            return node;
        if(node.firstChild().isElement())   //有孩子节点，继续进入下一层
        {
            node = node.firstChild();
        }
        else if(!node.nextSibling().isNull())//没孩子节点但兄弟节点不空，进入兄弟层
            node = node.nextSibling();
        else                                 //没孩子节点没兄弟节点，需要层层返回（循环）到父层的兄弟节点层去寻找
        {
            while(node.parentNode()!=root && node.nextSibling().isNull())
            {
                node = node.parentNode();
            }
            if(node.nextSibling().isNull())
                node = node.parentNode();
            else
                node = node.nextSibling();
        }
    }
    QDomNode nullNode;
    return nullNode;
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

void XMLReadWrite::addNode_Obsolete(QString nodeName, QString nodeValue, QVector<QPair<QString, QString> >& attrs)
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

    QDomElement newNode = doc.createElement(nodeName);        //新节点
    newNode.appendChild(doc.createTextNode(nodeValue));
    for(int i=0; i<attrs.size(); ++i)
    {
        QPair<QString, QString> pair = attrs.at(i);
        newNode.setAttribute(pair.first, pair.second);
    }

    QDomElement root = doc.documentElement();
    root.appendChild(newNode);
//    QDomNode last = root.lastChild();

//    qDebug()<<root.nodeName()<<last.nodeName();
//    root.insertAfter(newNode, last);                        //第二参数必须是调用者的直接子集，才能调用成功
    if(!file.open(QIODevice::WriteOnly))
        return;
    QTextStream out(&file);
    doc.save(out, 4);     //保存到文件中
    file.close();
}

bool XMLReadWrite::delNode_Obsolete(QString nodeName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite))
        return false;
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return false;
    }
    file.close();
    QDomElement root = doc.documentElement();
    if(root.isNull())
        return false;

    QDomNode node = root.firstChild();
    while(!node.isNull())
    {
        while(node.hasChildNodes())                 //value也是子节点，因此对于不含value只有属性的元素需要另做处理
        {
            node = node.firstChild();
            QString info = node.toElement().text();

            if(info.isEmpty())
                node = node.parentNode();
            if(node.nodeName() == nodeName)
            {
//                qDebug()<<"del";
                QDomNode parNode = node.parentNode();
                if(!parNode.removeChild(node).isNull())         //删除成功，跳出循环，保存当前修改
                    return true;
            }
            if(!node.nextSibling().isNull())
                node = node.nextSibling();
            else
                break;
        }
        if(node.nodeName() == nodeName)             //删除无value只有属性的元素
        {
            QDomNode parNode = node.parentNode();
            return !parNode.removeChild(node).isNull();
//            qDebug()<<"del noText ele";
        }
        //==============这一块代码尚需验证3，4，5层节点的情况
        node = node.parentNode();
//        if(!node.nextSibling().isNull())
            node = node.nextSibling();
    }

    if(!file.open(QIODevice::WriteOnly))
        return false;
    QTextStream out(&file);
    doc.save(out, 4);     //保存到文件中
    file.close();
}

bool XMLReadWrite::updateNode_Obsolete(QString oldNodeName, QString newValue)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return false;
    }
    file.close();
    QDomElement root = doc.documentElement();
    if(root.isNull())
        return false;
    QDomNode node = root.firstChild();
    while(!node.isNull())
    {
        while(node.hasChildNodes())                                 //修改包含value的元素的value
        {
            node = node.firstChild();
            QString info = node.toElement().text();
            if(info.isEmpty())
                node = node.parentNode();
            if(node.nodeName() == oldNodeName)
            {
                QDomNode parNode = node.parentNode();
                QDomNode newNode = doc.createElement(node.nodeName());
                newNode.appendChild(doc.createTextNode(newValue));
                parNode.replaceChild(newNode, node);
                if(!file.open(QIODevice::WriteOnly))
                    return false;
                QTextStream out(&file);
                doc.save(out, 4);     //保存到文件中
                file.close();
                return true;
            }
            if(!node.nextSibling().isNull())
                node = node.nextSibling();
            else
                break;
        }
        if(node.nodeName() == oldNodeName)             //修改无value只有属性元素的value
        {
            QDomNode parNode = node.parentNode();
            QDomNode newNode = doc.createElement(node.nodeName());
            newNode.appendChild(doc.createTextNode(newValue));
            parNode.replaceChild(newNode, node);
            if(!file.open(QIODevice::WriteOnly))
                return false;
            QTextStream out(&file);
            doc.save(out, 4);     //保存到文件中
            file.close();
            return true;
        }
        //==============这一块代码尚需验证3，4，5层节点的情况
        node = node.parentNode();
//        if(!node.nextSibling().isNull())
            node = node.nextSibling();
    }
    if(node.isNull())           //未找到要修改的节点
        return false;
}

QDomNode XMLReadWrite::findNode_Obsolete(QString nodeName)
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
            if(node.toElement().tagName() == nodeName)
                return node;
            if(!node.nextSibling().isNull())
                node = node.nextSibling();
            else
                break;
        }
        if(node.nodeName() == nodeName)             //find无value只有属性的元素
            return node;
        //==============这一块代码造成超过层数后程序崩溃，只能使用固定层数的xml结构
        node = node.parentNode();
//        if(!node.nextSibling().isNull())
            node = node.nextSibling();
    }
    return node;
}
