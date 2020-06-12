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
    //基本绘图事件
    void paintEvent(QPaintEvent *);

    int posX;

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
