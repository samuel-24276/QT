#include "xmlreadwrite.h"

Node::Node()
{
    docRootName = "LogRecords";
    recordRootName = "LogInfo";
    LogName = "LogName";
    LogPwd = "LogPwd";
    UserInfo = "UserInfo";
    UserName = "UserName";
    UserAge = "UserAge";
    UserSex = "UserSex";
    UserAddress = "UserAddress";
    Email = "Email";
    LogInTime = "LogInTime";
    LogOutTime = "LogOutTime";
}

XMLReadWrite::XMLReadWrite(QObject *parent, QString fName) : QObject(parent), fileName(fName)
{

}

QVector<QString> XMLReadWrite::read()
{    
    QDomNode node;
    QVector<QString> readInfo;
    QDomDocument doc;
    if(!getDoc(fileName, doc))
        return readInfo;

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

bool XMLReadWrite::addRecordNode(QString nodeName, QString recordRootId)   //recordRootId是当前xml文件最后一个LogInfo节点的id
{
    if(nodeName.isEmpty())              //在界面端已进行判断，可以去掉
        return false;
    QDomDocument doc;
    if(!getDoc(fileName, doc))
        return false;

    QFile file(fileName);
    QDomElement root = doc.documentElement();
    if(root.isNull())
        return false;
    QDomElement newNode = doc.createElement(nodeName);
    newNode.setAttribute("id", recordRootId);
    root.appendChild(newNode);
    return saveChange(doc, file);
}

bool XMLReadWrite::addElementById(QString nodeName, QString nodeValue, QVector<QPair<QString, QString> > &attrs, QString refNodeName, QString recordRootId)
{
    if(nodeName.isEmpty())              //在界面端已进行判断，可以去掉
        return false;
    QDomDocument doc;
    if(!getDoc(fileName, doc))
        return false;
    QFile file(fileName);
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
        return saveChange(doc, file);
    }

    QDomNode node = root.firstChild();                      //=========给根节点以外的节点添加子节点
    while(node!=root)                       //当前节点不是根节点，循环进入子层
    {
        if(!node.toElement().attribute("id", "").isEmpty() && node.toElement().attribute("id")!=recordRootId)//判空可以防止死循环
        {
            if(!node.nextSibling().isNull())
                node = node.nextSibling();
            else
                node = node.parentNode();
        }
        else
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
                return saveChange(doc, file);
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
    }
    return false;
}

bool XMLReadWrite::addSonNodeById(QString nodeName, QVector<QPair<QString, QString> > &attrs, QString refNodeName, QString recordRootId)
{
    if(nodeName.isEmpty())
        return false;
    QDomDocument doc;
    if(!getDoc(fileName, doc))
        return false;
    QFile file(fileName);
    QDomElement root = doc.documentElement();
    if(root.nodeName() == refNodeName)                      //被添加节点父节点为根节点，直接返回，应该调用addRecordNode()方法
        return false;
    if(root.isNull())
        return false;

    QDomNode node = root.firstChild();                      //给根节点以外的节点添加子节点
    while(node!=root)                                       //当前节点不是根节点，循环进入子层
    {
        if(!node.toElement().attribute("id", "").isEmpty() && node.toElement().attribute("id", "")!=recordRootId)
        {
            if(!node.nextSibling().isNull())
                node = node.nextSibling();
            else
                node = node.parentNode();
        }
        else
        {
            if(node.nodeName() == refNodeName)
            {
                if(node.firstChild().isText())                          //节点已含有值，则不可添加子节点
                    return false;
                QDomElement newNode = doc.createElement(nodeName);        //新节点,因为要设置属性，所以要使用QDomNode的子类QDomElement
                for(int i=0; i<attrs.size(); ++i)
                {
                    QPair<QString, QString> pair = attrs.at(i);
                    newNode.setAttribute(pair.first, pair.second);
                }
                node.appendChild(newNode);
                return saveChange(doc, file);
            }
            else
            {
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
        }
    }
    return false;
}

bool XMLReadWrite::delRecordNode_complete(QString nodeName)
{
    QDomDocument doc;
    if(!getDoc(fileName, doc))
        return false;
    QFile file(fileName);
    QDomElement root = doc.documentElement();
    if(root.isNull())
        return false;
    QDomNode node = root.firstChild();
    while(node.firstChild().firstChild().nodeValue() != nodeName)
    {
        node = node.nextSibling();
        if(node.isNull())           //欲删除的节点不存在
            return false;
    }
    root.removeChild(node);
    return saveChange(doc, file);
}

QString XMLReadWrite::updateRecordNode_complete(QString nodeName)
{
    QDomDocument doc;
    if(!getDoc(fileName, doc))
        return NULL;
    QDomElement root = doc.documentElement();
    if(root.isNull())
        return NULL;
    QDomNode node = root.firstChild();
    while(node.firstChild().firstChild().nodeValue()!=nodeName)
    {
        if(node.nextSibling().isNull())     //欲更改的节点不存在
            return NULL;
        node = node.nextSibling();
    }
    QString id = node.toElement().attribute("id");
    delRecordNode_complete(nodeName);
    return id;
}

QDomNode XMLReadWrite::findRecordNode_complete(QString nodeName)
{
    QDomNode node;
    QDomDocument doc;
    if(!getDoc(fileName, doc))
        return node;
    QDomElement root = doc.documentElement();
    if(root.isNull())
        return node;
    node = root.firstChild();
    while(node.firstChild().firstChild().nodeValue()!=nodeName)
    {        
        if(node.nextSibling().isNull())     //欲查询的节点不存在
        {
            return node.nextSibling();
        }
        node = node.nextSibling();
    }
    return node;
}

QMap<QString, QString> XMLReadWrite::parseNode(QDomNode& root)
{
    QMap<QString, QString> readInfo;
    if(root.isNull())
        return readInfo;

    QDomNode node = root.firstChild();
    while(node!=root)                       //当前节点不是根节点，循环进入子层
    {
        if(node.firstChild().isText())      //无孩子节点，包含nodeValue
        {
            readInfo.insert(node.nodeName(), node.firstChild().nodeValue());
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

bool XMLReadWrite::addSonNode(QString nodeName, QString nodeValue, QString refNodeName, QVector<QPair<QString, QString> > &attrs)
{
    if(nodeName.isEmpty())              //在界面端已进行判断，可以去掉
        return false;
    QDomDocument doc;
    if(!getDoc(fileName, doc))
        return false;
    QFile file(fileName);
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
        return saveChange(doc, file);
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
            return saveChange(doc, file);
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
    QDomDocument doc;
    if(!getDoc(fileName, doc))
        return false;
    QFile file(fileName);

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
            return saveChange(doc, file);
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
    QDomDocument doc;
    if(!getDoc(fileName, doc))
        return false;
    QFile file(fileName);

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
            return saveChange(doc, file);
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
    QDomDocument doc;
    if(!getDoc(fileName, doc))
        return false;
    QFile file(fileName);

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
                return saveChange(doc, file);
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
    QDomDocument doc;
    if(!getDoc(fileName, doc))
        return false;
    QFile file(fileName);

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
            return saveChange(doc, file);
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
    QDomDocument doc;
    if(!getDoc(fileName, doc))
        return node;

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

QDomNode XMLReadWrite::lastNode()
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
    return root.lastChild();
}

bool XMLReadWrite::getDoc(QString fileName, QDomDocument& doc)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    QString errorMsg;
    int errorRow, errorColumn;
    if(!doc.setContent(&file, &errorMsg, &errorRow, &errorColumn))
    {
        QMessageBox::warning(0, "warning", errorMsg+",Row:"+QString("%1").arg(errorRow)+",Column:"+QString("%1").arg(errorColumn));
        file.close();
        return false;
    }
    file.close();
    return true;
}

bool XMLReadWrite::saveChange(QDomDocument &doc, QFile &file)
{
    if(!file.open(QIODevice::WriteOnly))
        return false;
    QTextStream out(&file);
    doc.save(out, 4);     //保存到文件中
    file.close();
    return true;
}
