#ifndef STUDENT_H
#define STUDENT_H

#include <QObject>

class Student : public QObject
{
    Q_OBJECT
public:
    explicit Student(QObject *parent = nullptr);

signals:

//自定义槽函数，写到public slots:下，或者全局函数，或者Public下，或者lambda表达式
public slots:
    //1.返回void
    //2.需要声明，也需要实现
    //3.可以发生重载
    void treat();
    void treat(QString foodName);
};

#endif // STUDENT_H
