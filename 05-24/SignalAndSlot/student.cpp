#include "student.h"
#include<QDebug>

Student::Student(QObject *parent) : QObject(parent)
{

}

void Student::treat()
{
    qDebug()<<"请老师吃饭"<<endl;
}

void Student::treat(QString foodName)
{
    //将QString转为char *,使用toUtf8()转为QByteArray,再调用data()转为char *
    qDebug()<<"请老师吃饭,老师要吃："<<foodName.toUtf8().data()<<endl;
}
