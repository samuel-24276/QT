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
    void on_SelectBtn_clicked();

    void on_btnRead_clicked();

    void on_btnWrite_clicked();

    void on_btnAddSon_clicked();

    void on_btnInsertB_clicked();

    void on_btnInsertA_clicked();

    void on_btnDel_clicked();

    void on_btnUpdate_clicked();

    void on_btnFind_clicked();

private:
    Ui::MainWidget *ui;

    QString fileName;    

    QString nodeName;

    QString nodeValue;

    QString refNodeName;

    QVector<QPair<QString, QString> > attrs;
};

#endif // MAINWIDGET_H
