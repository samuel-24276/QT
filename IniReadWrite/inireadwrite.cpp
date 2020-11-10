#include "inireadwrite.h"

LogGroup::LogGroup()
{
    rootName = "LogRecord-";
    LogName = "LogName";
    LogPwd = "LogPwd";
    UserName = "UserName";
    UserAge = "UserAge";
    UserSex = "UserSex";
    UserAddress = "UserAddress";
    Email = "Email";
    LogInTime = "LogInTime";
    LogOutTime = "LogOutTime";
}

IniReadWrite::IniReadWrite(QObject *parent, QString fName) : QObject(parent), fileName(fName)
{
    group = new LogGroup;
}

IniReadWrite::~IniReadWrite()
{
    delete group;
}

/**
 * @brief IniReadWrite::read
 * @return
 * 每一节存储在一个QMap中，节名的key为Section，注解的key为Comment，其他参数按键值对处理
 */
QVector<QMap<QString, QString> > IniReadWrite::read()
{
    QVector<QMap<QString, QString> > params;
    QMap<QString, QString> param;
    QSettings* configIni = new QSettings(fileName, QSettings::IniFormat);
    QStringList groups = configIni->childGroups();
    for(int i=0; i<groups.size(); ++i)
    {
        configIni->beginGroup(groups[i]);
        QStringList attrs = configIni->childKeys();

        for(int j=0; j<attrs.size(); ++j)
        {
            param.insert(attrs[j], configIni->value(attrs[j]).toString());
        }
        params.append(param);
        param.clear();
        configIni->endGroup();
    }
    delete configIni;
    return params;
}

void IniReadWrite::write()                  //ini文件二级节点是通过/完成的，节只有1级，为了存储方便，舍弃UserInfo这一级
{
    QSettings* configIni = new QSettings(fileName, QSettings::IniFormat);
    QTextCodec* code=QTextCodec::codecForName("GB2312");                //设置编解码格式,用notepad++打开时也要用相同格式中文才不会乱码
    configIni->setIniCodec(code);
    configIni->beginGroup(group->rootName);
    configIni->setValue(group->LogName, tr("Samuel"));
    configIni->setValue(group->LogPwd,  tr("123456"));
    configIni->setValue(group->UserName,  tr("Winston"));
    configIni->setValue(group->UserAge,  tr("18"));
    configIni->setValue(group->UserSex,  tr("boy"));
    configIni->setValue(group->UserAddress,  tr("London"));
    configIni->setValue(group->Email,  tr("sdakjfa@163.com"));
    configIni->setValue(group->LogInTime,  tr("2020.11.05 14:53:023"));
    configIni->setValue(group->LogOutTime,  tr("2020.11.05 14:53:153"));
    configIni->endGroup();
    delete  configIni;
}

bool IniReadWrite::addParam(QString group, QString key, QString value)
{
    if(fileName.isEmpty())
        return false;
    QSettings* configIni = new QSettings(fileName, QSettings::IniFormat);
    QTextCodec* code=QTextCodec::codecForName("GB2312");
    configIni->setIniCodec(code);
    configIni->beginGroup(group);
    configIni->setValue(key, value);
    configIni->endGroup();
    delete configIni;
    return true;
}

/**
 * @brief IniReadWrite::deleteParam 删除节为group，键为key的属性，若该属性的group或key不存在，则返回false
 * @param group
 * @param key
 * @return
 */
bool IniReadWrite::deleteParam(QString group, QString key)
{
    if(fileName.isEmpty())
        return false;
    QSettings* configIni = new QSettings(fileName, QSettings::IniFormat);
    QStringList groups = configIni->childGroups();          //节不存在，直接返回false
    if(!groups.contains(group, Qt::CaseInsensitive))
        return false;

    configIni->beginGroup(group);
    QStringList keys = configIni->childKeys();
    bool flag = keys.contains(key);
    if(flag)
        configIni->remove(key);
    configIni->endGroup();
    delete configIni;
    return flag;
}

bool IniReadWrite::deleteGroup(QString group)
{
    if(fileName.isEmpty())
        return false;
    QSettings* configIni = new QSettings(fileName, QSettings::IniFormat);
    QStringList groups = configIni->childGroups();          //节不存在，直接返回false
    if(!groups.contains(group, Qt::CaseInsensitive))
        return false;
    configIni->remove(group);
    delete configIni;
    return true;
}

/**
 * @brief IniReadWrite::updateParam 将节为group，键为key的属性值修改为value，若group或key不存在，则返回false
 * @param group
 * @param key
 * @param value
 * @return
 */
bool IniReadWrite::updateParam(QString group, QString key, QString value)
{
    if(fileName.isEmpty())
        return false;
    QSettings* configIni = new QSettings(fileName, QSettings::IniFormat);
    QTextCodec* code=QTextCodec::codecForName("GB2312");
    configIni->setIniCodec(code);
    QStringList groups = configIni->childGroups();          //节不存在，直接返回false
    if(!groups.contains(group, Qt::CaseInsensitive))
        return false;
    configIni->beginGroup(group);
    QStringList keys = configIni->childKeys();
    bool flag = keys.contains(key);
    if(flag)
        configIni->setValue(key, value);
    configIni->endGroup();
    delete configIni;
    return flag;
}

/**
 * @brief IniReadWrite::findParam 查询以group为节的属性key的值，若key或节group不存在，则返回空；否则返回key对应的value
 * @param group
 * @param key
 * @return
 */
QString IniReadWrite::findParam(QString group, QString key)
{
    QString value;
    if(!fileName.isEmpty())
    {
        QSettings* configIni = new QSettings(fileName, QSettings::IniFormat);
        QStringList groups = configIni->childGroups();          //节不存在，直接返回NULL
        if(groups.contains(group, Qt::CaseInsensitive))
        {
            configIni->beginGroup(group);
            QStringList keys = configIni->childKeys();
            if(keys.contains(key))
                value = configIni->value(key, 0).toString();
        }
        delete configIni;
    }
    return value;
}

QString IniReadWrite::findParamGroup(QString key, QString value)
{
    QString group;
    QSettings* configIni = new QSettings(fileName, QSettings::IniFormat);
    QStringList groups = configIni->childGroups();
    for(int i=0; i<groups.size(); ++i)
    {
        configIni->beginGroup(groups[i]);
        if(configIni->contains(key))
        {
            if(configIni->value(key) == value)
            {
                group = groups[i];
                configIni->endGroup();
                break;
            }
        }
        configIni->endGroup();
    }
    return group;
}

int IniReadWrite::maxGroupId()
{
    QSettings* configIni = new QSettings(fileName, QSettings::IniFormat);
    QStringList groups = configIni->childGroups();          //节不存在，直接返回false
    int id = 0;
    for(int i=0; i<groups.size(); ++i)
    {
        int groupId = groups[groups.size()-1].split("-").at(1).toInt();
        if(groupId>id)
            id = groupId;
    }
    return id;
}
