#include "xmlreadwrite.h"

XMLReadWrite::XMLReadWrite(QObject *parent, QString fName) : QObject(parent), fileName(fName)
{

}

QVector<QString> XMLReadWrite::read()
{    
    QDomNode node;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return readInfo;
    QDomDocument doc;
    QString errorMsg;
    int errorRow, errorColumn;
    if(!doc.setContent(&file, &errorMsg, &errorRow, &errorColumn))
    {
        QMessageBox::warning(0, "warning", errorMsg+",Row:"+QString("%1").arg(errorRow)+",Column:"+QString("%1").arg(errorColumn));
        file.close();
        return readInfo;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if(root.isNull())
        return readInfo;

    node = root.firstChild();
    while(node!=root)                       //当前节点不是根节点，循环进入子层
    {
        if(node.firstChild().isText())      //无孩子节点，包含nodeValue
        {
            QString info = node.firstChild().nodeValue();
            readInfo.append(info);
            while(node.parentNode()!=root && node.nextSibling().isNull())//兄弟节点为空，且父节点不是root，则循环返回到父节点
            {
                node = node.parentNode();
            }
            if(node.nextSibling().isNull())//父节点是root且无兄弟节点
                break;
            else
                node = node.nextSibling();
        }
        else if(node.firstChild().isElement())   //有孩子节点，继续进入下一层
        {
            node = node.firstChild();
        }
        else if(node.firstChild().isNull())
        {
            while(node.parentNode()!=root && node.nextSibling().isNull())//兄弟节点为空，且父节点不是root，则循环返回到父节点
            {
                node = node.parentNode();
            }
            if(node.nextSibling().isNull())//父节点是root且无兄弟节点
                break;
            else
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
    QDomElement root = doc.createElement("LogInfo");
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

bool XMLReadWrite::addSonNode(QString nodeName, QString nodeValue, QString refNodeName, QVector<QPair<QString, QString> > &attrs)
{
    if(nodeName.isEmpty())              //在界面端已进行判断，可以去掉
        return false;
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
    if(refNodeName == root.nodeName())                      //=======给根节点添加子节点
    {
        QDomElement newNode = doc.createElement(nodeName);
        newNode.appendChild(doc.createTextNode(nodeValue));
        for(int i=0; i<attrs.size(); ++i)
        {
            QPair<QString, QString> pair = attrs.at(i);
            newNode.setAttribute(pair.first, pair.second);
        }
        root.appendChild(newNode);
        if(!file.open(QIODevice::WriteOnly))
            return false;
        QTextStream out(&file);
        doc.save(out, 4);     //保存到文件中
        file.close();
        return true;
    }

    QDomNode node = root.firstChild();                      //=========给根节点以外的节点添加子节点
    while(node!=root)                       //当前节点不是根节点，循环进入子层
    {
        if(node.nodeName() == refNodeName)
        {
            if(node.firstChild().isText())                          //节点已含有值，则不可添加子节点
                return false;
            QDomElement newNode = doc.createElement(nodeName);        //新节点,因为要设置属性，所以要使用QDomNode的子类QDomElement
            newNode.appendChild(doc.createTextNode(nodeValue));
            for(int i=0; i<attrs.size(); ++i)
            {
                QPair<QString, QString> pair = attrs.at(i);
                newNode.setAttribute(pair.first, pair.second);
            }
            node.appendChild(newNode);
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

bool XMLReadWrite::insertNodeBefore(QString nodeName, QString nodeValue, QString refNodeName, QVector<QPair<QString, QString> > &attrs)
{
    if(nodeName.isEmpty())
        return false;
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
    QDomNode node = root.firstChild();
    while(node!=root)                       //当前节点不是根节点，循环进入子层
    {
        if(node.nodeName() == refNodeName)
        {
            QDomNode parNode = node.parentNode();
            QDomElement newNode = doc.createElement(nodeName);        //新节点,因为要设置属性，所以要使用QDomNode的子类QDomElement
            newNode.appendChild(doc.createTextNode(nodeValue));
            for(int i=0; i<attrs.size(); ++i)
            {
                QPair<QString, QString> pair = attrs.at(i);
                newNode.setAttribute(pair.first, pair.second);
            }
            parNode.insertBefore(newNode, node);                        //第二参数必须是调用者的直接子集，才能调用成功
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

bool XMLReadWrite::insertNodeAfter(QString nodeName, QString nodeValue, QString refNodeName, QVector<QPair<QString, QString> > &attrs)
{
    if(nodeName.isEmpty())
        return false;
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
    QDomNode node = root.firstChild();
    while(node!=root)                       //当前节点不是根节点，循环进入子层
    {
        if(node.nodeName() == refNodeName)
        {
            QDomNode parNode = node.parentNode();
            QDomElement newNode = doc.createElement(nodeName);        //新节点,因为要设置属性，所以要使用QDomNode的子类QDomElement
            newNode.appendChild(doc.createTextNode(nodeValue));
            for(int i=0; i<attrs.size(); ++i)
            {
                QPair<QString, QString> pair = attrs.at(i);
                newNode.setAttribute(pair.first, pair.second);
            }
            parNode.insertAfter(newNode, node);                        //第二参数必须是调用者的直接子集，才能调用成功
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

bool XMLReadWrite::updateNode(QString oldNodeName, QString newValue, QVector<QPair<QString, QString> >& attrs)
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
            QDomElement newNode = doc.createElement(node.nodeName());       //因为要设置属性，所以要使用QDomNode的子类QDomElement
            newNode.appendChild(doc.createTextNode(newValue));
            for(int i=0; i<attrs.size(); ++i)
            {
                QPair<QString, QString> pair = attrs.at(i);
                newNode.setAttribute(pair.first, pair.second);
            }
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

LogInfo& XMLReadWrite::toLogInfo()
{
    if(readInfo.size()>0)
    {
        logInfo.LogName = readInfo.at(0);
        logInfo.LogPwd = readInfo.at(1);
        logInfo.UserName = readInfo.at(2);
        logInfo.UserAge = readInfo.at(3).toInt();
        logInfo.UserSex = readInfo.at(4);
        logInfo.UserAddress = readInfo.at(5);
        logInfo.Email = readInfo.at(6);
        logInfo.LogInTime = readInfo.at(7);
        logInfo.LogOutTime = readInfo.at(8);
    }
    return logInfo;
}
