#ifndef TEACHER_H
#define TEACHER_H

#include <QObject>

class Teacher : public QObject
{
    Q_OBJECT
public:
    explicit Teacher(QObject *parent = nullptr);
//自定义信号写到signals:下
signals:
    //自定义信号写法：
    //1.返回void
    //2.信号只需要声明，不需要实现
    //3.自定义信号可以发生重载
    void hungry();
    void hungry(QString foodName);
};

#endif // TEACHER_H
