#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = nullptr);
    //鼠标进入
    void enterEvent(QEvent *);
    //鼠标离开
    void leaveEvent(QEvent *);

    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    //利用事件分发器进行拦截
    bool event(QEvent* e);

signals:

public slots:
};

#endif // MYLABEL_H
