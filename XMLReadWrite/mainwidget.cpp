#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(10);
    ui->tableWidget->setHorizontalHeaderLabels(QString("AttributeName\n(Non Number),AttributeValue").split(","));
    ui->tableWidget->setColumnWidth(0, 110);
    ui->tableWidget->setColumnWidth(1, 110);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {"
                                                        "color: black;padding-left: 0px;border: 1px solid #6c6c6c;}");
    ui->tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section {"
                                                        "color: black;padding-left: 0px;border: 1px solid #6c6c6c;}");
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setAutoScroll(true);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_SelectBtn_clicked()
{
    fileName = QFileDialog::getOpenFileName();
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this, "warning", "You haven't select file");
        return;
    }
    ui->PathL->setText(fileName);
}

void MainWidget::on_btnRead_clicked()
{
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this, "warning", "You haven't select file");
        return;
    }
    XMLReadWrite* xmlRW = new XMLReadWrite(this, fileName);
    QVector<QString> vecInfo = xmlRW->read();
    delete xmlRW;
}

void MainWidget::on_btnWrite_clicked()
{
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this, "warning", "You haven't select file");
        return;
    }
    XMLReadWrite* xmlRW = new XMLReadWrite(this, fileName);
    xmlRW->write(fileName);
    delete xmlRW;
}

void MainWidget::on_btnAddSon_clicked()
{
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this, "warning", "You haven't select file");
        return;
    }
    XMLReadWrite* xmlRW = new XMLReadWrite(this, fileName);
    if(ui->nNameL->text().isEmpty() || ui->nRefNameL->text().isEmpty())                                    //节点名称不能为空，若空则添加失败；节点值可以为空，节点属性也可以为空
    {
        QMessageBox::information(this, "Failed", "NodeName and RefNodeName can't be Empty");
        return;
    }
    else
    {
        nodeName = ui->nNameL->text();
        refNodeName = ui->nRefNameL->text();
    }
    if(ui->nValueL->text().isEmpty())
        nodeValue = "";
    else
        nodeValue = ui->nValueL->text();
    int size = ui->tableWidget->rowCount();
    QPair<QString, QString> attr;
    QTableWidgetItem *item1, *item2;
    for(int i=0; i<size; ++i)
    {
        item1 = ui->tableWidget->item(i, 0);
        item2 = ui->tableWidget->item(i,1);
        if(item1==nullptr || item2==nullptr)                    //单元格未点击
            continue;
        if(item1->text().isEmpty() || item2->text().isEmpty())  //单元格点击后未输入
            continue;
        attr.first = item1->text();
        attr.second = item2->text();
        attrs.append(attr);
    }

    if(xmlRW->addSonNode(nodeName, nodeValue, refNodeName, attrs))
        QMessageBox::information(this, "Success", "AddSon Success");
    else
        QMessageBox::information(this, "Failed", "AddSon failed");

    delete xmlRW;
}

void MainWidget::on_btnInsertB_clicked()
{
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this, "warning", "You haven't select file");
        return;
    }
    XMLReadWrite* xmlRW = new XMLReadWrite(this, fileName);
    if(ui->nNameL->text().isEmpty() || ui->nRefNameL->text().isEmpty())                                    //节点名称不能为空，若空则添加失败；节点值可以为空，节点属性也可以为空
    {
        QMessageBox::information(this, "Failed", "NodeName and RefNodeName can't be Empty");
        return;
    }
    else
    {
        nodeName = ui->nNameL->text();
        refNodeName = ui->nRefNameL->text();
    }
    if(ui->nValueL->text().isEmpty())
        nodeValue = "";
    else
        nodeValue = ui->nValueL->text();
    int size = ui->tableWidget->rowCount();
    QPair<QString, QString> attr;
    QTableWidgetItem *item1, *item2;
    for(int i=0; i<size; ++i)
    {
        item1 = ui->tableWidget->item(i, 0);
        item2 = ui->tableWidget->item(i,1);
        if(item1==nullptr || item2==nullptr)                    //单元格未点击
            continue;
        if(item1->text().isEmpty() || item2->text().isEmpty())  //单元格点击后未输入
            continue;
        attr.first = item1->text();
        attr.second = item2->text();
        attrs.append(attr);
    }

    if(xmlRW->insertNodeBefore(nodeName, nodeValue, refNodeName, attrs))
        QMessageBox::information(this, "Success", "Insert Success");
    else
        QMessageBox::information(this, "Failed", "Insert failed");

    delete xmlRW;
}

void MainWidget::on_btnInsertA_clicked()
{
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this, "warning", "You haven't select file");
        return;
    }
    XMLReadWrite* xmlRW = new XMLReadWrite(this, fileName);
    if(ui->nNameL->text().isEmpty() || ui->nRefNameL->text().isEmpty())                                    //节点名称不能为空，若空则添加失败；节点值可以为空，节点属性也可以为空
    {
        QMessageBox::information(this, "Failed", "NodeName and RefNodeName can't be Empty");
        return;
    }
    else
    {
        nodeName = ui->nNameL->text();
        refNodeName = ui->nRefNameL->text();
    }
    if(ui->nValueL->text().isEmpty())
        nodeValue = "";
    else
        nodeValue = ui->nValueL->text();
    int size = ui->tableWidget->rowCount();
    QPair<QString, QString> attr;
    QTableWidgetItem *item1, *item2;
    for(int i=0; i<size; ++i)
    {
        item1 = ui->tableWidget->item(i, 0);
        item2 = ui->tableWidget->item(i,1);
        if(item1==nullptr || item2==nullptr)                    //单元格未点击
            continue;
        if(item1->text().isEmpty() || item2->text().isEmpty())  //单元格点击后未输入
            continue;
        attr.first = item1->text();
        attr.second = item2->text();
        attrs.append(attr);
    }

    if(xmlRW->insertNodeAfter(nodeName, nodeValue, refNodeName, attrs))
        QMessageBox::information(this, "Success", "Insert Success");
    else
        QMessageBox::information(this, "Failed", "Insert failed");

    delete xmlRW;
}

void MainWidget::on_btnDel_clicked()
{
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this, "warning", "You haven't select file");
        return;
    }
    XMLReadWrite* xmlRW = new XMLReadWrite(this, fileName);
    if(ui->nNameL->text().isEmpty())                                    //节点名称不能为空，若空则删除失败
    {
        QMessageBox::information(this, "Failed", "NodeName can't be Empty");
        return;
    }
    else
        nodeName = ui->nNameL->text();

    if(xmlRW->delNode(nodeName))
        QMessageBox::information(this, "Success", "Delete success");
    else
        QMessageBox::information(this, "Failed", "Delete failed");      //删除xml文件内不存在的值会弹出此提示
    delete xmlRW;
}

void MainWidget::on_btnUpdate_clicked()
{
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this, "warning", "You haven't select file");
        return;
    }
    XMLReadWrite* xmlRW = new XMLReadWrite(this, fileName);
    if(ui->nNameL->text().isEmpty() || ui->nValueL->text().isEmpty())                                    //节点名称不能为空，若空则删除失败
    {
        QMessageBox::information(this, "Failed", "NodeName or NodeValue can't be Empty");
        return;
    }
    else
    {
        nodeName = ui->nNameL->text();
        nodeValue = ui->nValueL->text();
    }

    int size = ui->tableWidget->rowCount();
    QPair<QString, QString> attr;
    QTableWidgetItem *item1, *item2;
    for(int i=0; i<size; ++i)
    {
        item1 = ui->tableWidget->item(i, 0);
        item2 = ui->tableWidget->item(i, 1);
        if(item1==nullptr || item2==nullptr)
            continue;
        if(item1->text().isEmpty() || item2->text().isEmpty())
            continue;
        attr.first = item1->text();
        attr.second = item2->text();
        attrs.append(attr);
    }

    if(xmlRW->updateNode(nodeName, nodeValue, attrs))
        QMessageBox::information(this, "Success", "Update success");
    else
        QMessageBox::information(this, "Failed", "Update failed");      //修改xml文件里不存在的值会弹出此提示
    delete xmlRW;
}

void MainWidget::on_btnFind_clicked()
{
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this, "warning", "You haven't select file");
        return;
    }
    XMLReadWrite* xmlRW = new XMLReadWrite(this, fileName);
    if(ui->nNameL->text().isEmpty())                                    //节点名称不能为空，若空则删除失败
    {
        QMessageBox::information(this, "Failed", "NodeName can't be Empty");
        return;
    }
    else
        nodeName = ui->nNameL->text();

    QDomNode node = xmlRW->findNode(nodeName);

    if(!node.isNull())
    {
        QMessageBox::information(this, "Success", "Find success");
    }
    else
        QMessageBox::information(this, "Failed", "Find failed");
    delete xmlRW;
}
