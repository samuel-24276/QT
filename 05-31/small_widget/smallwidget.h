#ifndef SMALLWIDGET_H
#define SMALLWIDGET_H

#include <QWidget>

namespace Ui {
class smallWidget;
}

class smallWidget : public QWidget
{
    Q_OBJECT

public:
    explicit smallWidget(QWidget *parent = nullptr);
    ~smallWidget();

    void setData(int val);

    int getData();

private:
    Ui::smallWidget *ui;
};

#endif // SMALLWIDGET_H
