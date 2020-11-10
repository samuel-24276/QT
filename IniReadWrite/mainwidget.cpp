#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    logGroup = new LogGroup;
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_SelectBtn_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("ini file (*.ini);; other file(*.*)"));
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this, "warning", "You haven't select file");
        return;
    }
    ui->PathL->setText(fileName);
    IniReadWrite* iniRW = new IniReadWrite(this, fileName);
    delete iniRW;
}

void MainWidget::on_btnWrite_clicked()
{
    if(isFileSelected())
    {
        IniReadWrite* iniRW = new IniReadWrite(this, fileName);
        iniRW->write();
        delete iniRW;
    }
}

void MainWidget::on_btnRead_clicked()
{
    if(isFileSelected())
    {
        IniReadWrite* iniRW = new IniReadWrite(this, fileName);
        iniRW->read();
        delete iniRW;
    }
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

void MainWidget::on_btnAddP_clicked()
{
    if(!isFileSelected())
        return;
    group = ui->GroupL->text();
    key = ui->keyL->text();
    value = ui->ValueL->text();
    if(group.isEmpty() || key.isEmpty() || value.isEmpty())
    {
        QMessageBox::information(this, "Failed", "group, key and value can't be empty");
        return;
    }
    IniReadWrite* iniRW = new IniReadWrite(this, fileName);
    iniRW->addParam(group, key, value);
    delete iniRW;
}

void MainWidget::on_btnDelP_clicked()
{
    if(!isFileSelected())
        return;
    group = ui->GroupL->text();
    key = ui->keyL->text();
    if(group.isEmpty() || key.isEmpty())
    {
        QMessageBox::information(this, "Failed", "group and key can't be empty");
        return;
    }
    IniReadWrite* iniRW = new IniReadWrite(this, fileName);
    if(iniRW->deleteParam(group, key))
        QMessageBox::information(this, "Success", "Delete success");
    else
        QMessageBox::information(this, "Failed", "Delete failed");
    delete iniRW;
}

void MainWidget::on_btnUpdateP_clicked()
{
    if(!isFileSelected())
        return;
    group = ui->GroupL->text();
    key = ui->keyL->text();
    value = ui->ValueL->text();
    if(group.isEmpty() || key.isEmpty() || value.isEmpty())
    {
        QMessageBox::information(this, "Failed", "group, key and value can't be empty");
        return;
    }
    IniReadWrite* iniRW = new IniReadWrite(this, fileName);
    if(iniRW->updateParam(group, key, value))
        QMessageBox::information(this, "Success", "Update success");
    else
        QMessageBox::information(this, "Failed", "Update failed");
    delete iniRW;
}

void MainWidget::on_btnFindP_clicked()
{
    if(!isFileSelected())
        return;
    group = ui->GroupL->text();
    key = ui->keyL->text();
    if(group.isEmpty() || key.isEmpty())
    {
        QMessageBox::information(this, "Failed", "group and key can't be empty");
        return;
    }

    IniReadWrite* iniRW = new IniReadWrite(this, fileName);
    QString value = iniRW->findParam(group, key);
    if(!value.isEmpty())
    {
        QMessageBox::information(this, "Success", "find "+key+" success");
        ui->ValueL->setText(value);
    }
    else
        QMessageBox::information(this, "Failed", "find "+key+" failed");
    delete iniRW;
}

void MainWidget::fillInfo()
{
    logInfo.LogName = ui->LogNameL->text();
    logInfo.LogPwd = ui->LogPwdL->text();
    logInfo.UserName = ui->UserNameL->text();
    logInfo.UserAge = ui->UserAgeL->text();
    logInfo.UserSex = ui->UserSexL->text();
    logInfo.UserAddress = ui->UserAddressL->text();
    logInfo.Email = ui->EmailL->text();
    logInfo.LogInTime = ui->LogInTimeL->text();
    logInfo.LogOutTime = ui->LogOutTimeL->text();
}

void MainWidget::pullInfo()
{
    ui->LogNameL->setText(logInfo.LogName);
    ui->LogPwdL->setText(logInfo.LogPwd);
    ui->UserNameL->setText(logInfo.UserName);
    ui->UserAgeL->setText(logInfo.UserAge);
    ui->UserSexL->setText(logInfo.UserSex);
    ui->UserAddressL->setText(logInfo.UserAddress);
    ui->EmailL->setText(logInfo.Email);
    ui->LogInTimeL->setText(logInfo.LogInTime);
    ui->LogOutTimeL->setText(logInfo.LogOutTime);
}

bool MainWidget::checkInfo(int mode)
{
    bool flag=true;
    if(mode)
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

void MainWidget::on_btnAddInfo_clicked()
{
    if(checkInfo())
    {
        fillInfo();
        IniReadWrite* iniRW = new IniReadWrite(this, fileName);
        if(iniRW->findParamGroup(logGroup->LogName, logInfo.LogName).isEmpty())
        {
            group = logGroup->rootName + QString("%1").arg(iniRW->maxGroupId()+1);
            iniRW->addParam(group, logGroup->LogName, logInfo.LogName);
            iniRW->addParam(group, logGroup->LogPwd, logInfo.LogPwd);
            iniRW->addParam(group, logGroup->UserName, logInfo.UserName);
            iniRW->addParam(group, logGroup->UserAge, logInfo.UserAge);
            iniRW->addParam(group, logGroup->UserSex, logInfo.UserSex);
            iniRW->addParam(group, logGroup->UserAddress, logInfo.UserAddress);
            iniRW->addParam(group, logGroup->Email, logInfo.Email);
            iniRW->addParam(group, logGroup->LogInTime, logInfo.LogInTime);
            iniRW->addParam(group, logGroup->LogOutTime, logInfo.LogOutTime);
            QMessageBox::information(this, "Success", "Add logInfo success");
        }
        else
            QMessageBox::warning(this, "Failed", "The LogName already exists");
        delete iniRW;

    }
}

void MainWidget::on_btnDelInfo_clicked()
{
    if(checkInfo(1))
    {
        IniReadWrite* iniRW = new IniReadWrite(this, fileName);
        fillInfo();
        QString group = iniRW->findParamGroup(logGroup->LogName, ui->LogNameL->text());
        if(group.isEmpty())
        {
            QMessageBox::information(this, "Failed", "Delete logInfo failed");
            infoClear();
        }
        else
        {
            if(iniRW->deleteGroup(group))   //返回值一定为true，因为通过了之前的findParamGroup()判空测试
                QMessageBox::information(this, "Success", "Delete logInfo success");
        }
        delete iniRW;
    }
}

void MainWidget::on_btnUpdateInfo_clicked()
{
    if(checkInfo(1))
    {
        IniReadWrite* iniRW = new IniReadWrite(this, fileName);
        QString group = iniRW->findParamGroup(logGroup->LogName, ui->LogNameL->text());
        if(group.isEmpty())
        {
            QMessageBox::information(this, "Failed", "Update logInfo failed");
            infoClear();
        }
        else
        {
            fillInfo();
            iniRW->updateParam(group, logGroup->LogName, logInfo.LogName);
            iniRW->updateParam(group, logGroup->LogPwd, logInfo.LogPwd);
            iniRW->updateParam(group, logGroup->UserName, logInfo.UserName);
            iniRW->updateParam(group, logGroup->UserAge, logInfo.UserAge);
            iniRW->updateParam(group, logGroup->UserSex, logInfo.UserSex);
            iniRW->updateParam(group, logGroup->UserAddress, logInfo.UserAddress);
            iniRW->updateParam(group, logGroup->Email, logInfo.Email);
            iniRW->updateParam(group, logGroup->LogInTime, logInfo.LogInTime);
            iniRW->updateParam(group, logGroup->LogOutTime, logInfo.LogOutTime);
            QMessageBox::information(this, "Success", "Update logInfo success");
        }
        delete iniRW;
    }
}

void MainWidget::on_btnFindInfo_clicked()
{
    if(checkInfo(1))
    {
        IniReadWrite* iniRW = new IniReadWrite(this, fileName);
        QString group = iniRW->findParamGroup(logGroup->LogName, ui->LogNameL->text());
        if(group.isEmpty())
        {
            QMessageBox::information(this, "Failed", "Find logInfo failed");
            infoClear();
        }
        else
        {
            logInfo.LogName = iniRW->findParam(group, logGroup->LogName);
            logInfo.LogPwd = iniRW->findParam(group, logGroup->LogPwd);
            logInfo.UserName = iniRW->findParam(group, logGroup->UserName);
            logInfo.UserAge = iniRW->findParam(group, logGroup->UserAge);
            logInfo.UserSex = iniRW->findParam(group, logGroup->UserSex);
            logInfo.UserAddress = iniRW->findParam(group, logGroup->UserAddress);
            logInfo.Email = iniRW->findParam(group, logGroup->Email);
            logInfo.LogInTime = iniRW->findParam(group, logGroup->LogInTime);
            logInfo.LogOutTime = iniRW->findParam(group, logGroup->LogOutTime);
            QMessageBox::information(this, "Success", "Find logInfo success");
            pullInfo();
        }
        delete iniRW;
    }
}
