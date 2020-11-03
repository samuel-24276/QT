#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QDebug>

#include "xmlreadwrite.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private slots:
    void on_OpenBtn_clicked();

private:
    Ui::MainWidget *ui;

    QString fileName;

    XMLReadWrite* xmlRW;
};

#endif // MAINWIDGET_H
