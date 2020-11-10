#ifndef INIREADWRITE_H
#define INIREADWRITE_H

#include <QObject>
#include <QFile>
#include <QSettings>
#include <QTextCodec>
#include <QStringList>
#include <QDebug>

/**
 * @brief The Node class 登陆信息的结构
 */
class LogGroup
{
public:
    QString rootName;
    QString LogName;
    QString LogPwd;
    QString UserName;
    QString UserAge;
    QString UserSex;
    QString UserAddress;
    QString Email;
    QString LogInTime;
    QString LogOutTime;
public:
    LogGroup();
};

class IniReadWrite : public QObject
{
    Q_OBJECT
public:
    explicit IniReadWrite(QObject *parent = nullptr, QString fName="");

    ~IniReadWrite();

    QVector<QMap<QString,QString> > read();

    void write();

    bool addParam(QString group, QString key, QString value);

    bool deleteParam(QString group, QString key);

    bool deleteGroup(QString group);

    bool updateParam(QString group, QString key, QString value);

    QString findParam(QString group, QString key);

    QString findParamGroup(QString key, QString value);             //根据key值查询其对应的组

    int maxGroupId();

signals:

private:
    QString fileName;

    LogGroup* group;
};

#endif // INIREADWRITE_H
