#include "mainwidget.h"
#include "ui_mainwidget.h"

Node::Node()
{
    docRootName = "LogRecords";
    recordRootName = "LogInfo";
    LogName = "LogName";
    LogPwd = "LogPwd";
    UserInfo = "UserInfo";
    UserName = "UserName";
    UserAge = "UserAge";
    UserSex = "UserSex";
    UserAddress = "UserAddress";
    Email = "Email";
    LogInTime = "LogInTime";
    LogOutTime = "LogOutTime";
}

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    nName = new Node();
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

    connect(ui->btnAddInfo, SIGNAL(clicked()), this, SLOT(addInfo()));
    connect(ui->btnDelInfo, SIGNAL(clicked()), this, SLOT(delInfo()));
    connect(ui->btnUpdateInfo, SIGNAL(clicked()), this, SLOT(updateInfo()));
    connect(ui->btnFindInfo, SIGNAL(clicked()), this, SLOT(findInfo()));
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::addInfo()
{
    if(!isFileSelected() || !checkInfo())           //未选择文件或者信息不完善，直接返回
        return;
    XMLReadWrite* xmlRW = new XMLReadWrite(this, fileName);
    fillInfo();
    QDomNode node = xmlRW->findRecordNode_complete(logInfo.LogName);
    if(node.isNull())                               //未找到要添加的LogName
    {
        QString recordId = QString("%1").arg(xmlRW->maxNodeId()+1);
        bool flag=true;
        flag = flag && xmlRW->addRecordNode(nName->recordRootName, recordId);
        flag = flag && xmlRW->addElementById(nName->LogName, logInfo.LogName, attrs, nName->recordRootName, recordId);
        flag = flag && xmlRW->addElementById(nName->LogPwd, logInfo.LogPwd, attrs, nName->recordRootName, recordId);
        flag = flag && xmlRW->addSonNodeById(nName->UserInfo, attrs, nName->recordRootName, recordId);
        flag = flag && xmlRW->addElementById(nName->UserName, logInfo.UserName, attrs, nName->UserInfo, recordId);
        flag = flag && xmlRW->addElementById(nName->UserAge, logInfo.UserAge, attrs, nName->UserInfo, recordId);
        flag = flag && xmlRW->addElementById(nName->UserSex, logInfo.UserSex, attrs, nName->UserInfo, recordId);
        flag = flag && xmlRW->addElementById(nName->UserAddress, logInfo.UserAddress, attrs, nName->UserInfo, recordId);
        flag = flag && xmlRW->addElementById(nName->Email, logInfo.Email, attrs, nName->UserInfo, recordId);
        flag = flag && xmlRW->addElementById(nName->LogInTime, logInfo.LogInTime, attrs, nName->recordRootName, recordId);
        flag = flag && xmlRW->addElementById(nName->LogOutTime, logInfo.LogOutTime, attrs, nName->recordRootName, recordId);
        if(flag)
            QMessageBox::information(this, "Success", "Add success");
        else
            QMessageBox::information(this, "Failed", "Add failed");
        delete xmlRW;
    }
    else                                            //LogName已存在
        QMessageBox::information(this, "Failed", "LogName already exists");
}

void MainWidget::delInfo()
{
    if(!isFileSelected() || !checkInfo(1))           //未选择文件或者信息不完善，直接返回
        return;
    XMLReadWrite* xmlRW = new XMLReadWrite(this, fileName);
    if(xmlRW->delRecordNode_complete(ui->LogNameL->text()))
        QMessageBox::information(this, "Success", "Delete success");
    else
        QMessageBox::information(this, "Failed", "Delete failed");
    delete xmlRW;
}

void MainWidget::updateInfo()
{
    if(!isFileSelected() || !checkInfo(1))           //未选择文件或者信息不完善，直接返回
        return;
    XMLReadWrite* xmlRW = new XMLReadWrite(this, fileName);
    QString recordId = xmlRW->updateRecordNode_complete(ui->LogNameL->text());
    if(recordId.isEmpty())                          //修改的节点不存在
    {
        QMessageBox::information(this, "Failed", "Update failed");
        return;
    }
    fillInfo();
    bool flag=true;
    flag = flag && xmlRW->addRecordNode(nName->recordRootName, recordId);
    flag = flag && xmlRW->addElementById(nName->LogName, logInfo.LogName, attrs, nName->recordRootName, recordId);
    flag = flag && xmlRW->addElementById(nName->LogPwd, logInfo.LogPwd, attrs, nName->recordRootName, recordId);
    flag = flag && xmlRW->addSonNodeById(nName->UserInfo, attrs, nName->recordRootName, recordId);
    flag = flag && xmlRW->addElementById(nName->UserName, logInfo.UserName, attrs, nName->UserInfo, recordId);
    flag = flag && xmlRW->addElementById(nName->UserAge, logInfo.UserAge, attrs, nName->UserInfo, recordId);
    flag = flag && xmlRW->addElementById(nName->UserSex, logInfo.UserSex, attrs, nName->UserInfo, recordId);
    flag = flag && xmlRW->addElementById(nName->UserAddress, logInfo.UserAddress, attrs, nName->UserInfo, recordId);
    flag = flag && xmlRW->addElementById(nName->Email, logInfo.Email, attrs, nName->UserInfo, recordId);
    flag = flag && xmlRW->addElementById(nName->LogInTime, logInfo.LogInTime, attrs, nName->recordRootName, recordId);
    flag = flag && xmlRW->addElementById(nName->LogOutTime, logInfo.LogOutTime, attrs, nName->recordRootName, recordId);
    if(flag)
        QMessageBox::information(this, "Success", "Update success");
    else
        QMessageBox::information(this, "Failed", "Update failed");
    delete xmlRW;
}

void MainWidget::findInfo()
{
    if(!isFileSelected() || !checkInfo(1))           //未选择文件或者信息不完善，直接返回
        return;
    XMLReadWrite* xmlRW = new XMLReadWrite(this, fileName);
    QDomNode node = xmlRW->findRecordNode_complete(ui->LogNameL->text());
    if(!node.isNull())
    {
        QMap<QString, QString> info = xmlRW->parseNode(node);
        pullInfo(info);
        QMessageBox::information(this, "Success", "Find success");
    }
    else
    {
        infoClear();
        QMessageBox::information(this, "Failed", "Find failed");
    }
    delete xmlRW;
}

bool MainWidget::isFileSelected()
{
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this, "warning", "You haven't select file");
        return false;
    }
    return true;
}

bool MainWidget::checkInfo(int i)
{
    bool flag;
    if(i)
        flag = ui->LogNameL->text().isEmpty();
    else
        flag = ui->LogNameL->text().isEmpty() || ui->LogPwdL->text().isEmpty() ||
            ui->UserNameL->text().isEmpty() || ui->UserAgeL->text().isEmpty() ||
            ui->UserSexL->text().isEmpty() || ui->UserAddressL->text().isEmpty() ||
            ui->EmailL->text().isEmpty() || ui->LogInTimeL->text().isEmpty() ||
            ui->LogOutTimeL->text().isEmpty();
    if(flag)
    {
        QMessageBox::information(this, "Failed", "Some info doesn't fill in.");
        return false;
    }
    return true;
}

void MainWidget::fillInfo()
{
    logInfo.LogName = ui->LogNameL->text();
    logInfo.LogPwd = ui->LogPwdL->text();
    logInfo.LogInTime = ui->LogInTimeL->text();
    logInfo.LogOutTime = ui->LogOutTimeL->text();
    logInfo.UserName = ui->UserNameL->text();
    logInfo.UserAge = ui->UserAgeL->text();
    logInfo.UserSex = ui->UserSexL->text();
    logInfo.UserAddress = ui->UserAddressL->text();
    logInfo.Email = ui->EmailL->text();
}

void MainWidget::pullInfo(QMap<QString, QString>& info)
{
    ui->LogNameL->setText(info.value(nName->LogName));
    ui->LogPwdL->setText(info.value(nName->LogPwd));
    ui->UserNameL->setText(info.value(nName->UserName));
    ui->UserAgeL->setText(info.value(nName->UserAge));
    ui->UserSexL->setText(info.value(nName->UserSex));
    ui->UserAddressL->setText(info.value(nName->UserAddress));
    ui->EmailL->setText(info.value(nName->Email));
    ui->LogInTimeL->setText(info.value(nName->LogInTime));
    ui->LogOutTimeL->setText(info.value(nName->LogOutTime));
}

void MainWidget::infoClear()
{
    ui->LogPwdL->clear();
    ui->UserNameL->clear();
    ui->UserAgeL->clear();
    ui->UserSexL->clear();
    ui->UserAddressL->clear();
    ui->EmailL->clear();
    ui->LogInTimeL->clear();
    ui->LogOutTimeL->clear();
}

void MainWidget::on_SelectBtn_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("xml file (*.xml);;other file(*.*)"));
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this, "warning", "You haven't select file");
        return;
    }
    ui->PathL->setText(fileName);
    XMLReadWrite* xmlRW = new XMLReadWrite(this, fileName);
    delete xmlRW;
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
        item2 = ui->tableWidget->item(i, 1);
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

void MainWidget::on_btnDelNode_clicked()
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

void MainWidget::on_btnUpdateNode_clicked()
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

void MainWidget::on_btnFindNode_clicked()
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
