#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    //定时器事件
    void timerEvent(QTimerEvent *);

    int id1;    //定时器标识
    int id2;

    bool eventFilter(QObject *, QEvent *);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
