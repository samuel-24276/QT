#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //TreeWidget树控件使用
    //设置头
    ui->treeWidget->setHeaderLabels(QStringList()<<"英雄"<<"英雄介绍");
    //TreeWidget中每个项目都称为QTreeWidgetItem
    QTreeWidgetItem * adItem = new QTreeWidgetItem(QStringList()<<"物理英雄");
    QTreeWidgetItem * apItem = new QTreeWidgetItem(QStringList()<<"魔法英雄");
    //添加根节点
    ui->treeWidget->addTopLevelItem(adItem);
    ui->treeWidget->addTopLevelItem(apItem);
    QStringList hero1;
    hero1<<"剑魔"<<"恕瑞玛飞升者";
    QStringList hero2;
    hero2<<"盖伦"<<"德玛西亚之力";
    QStringList hero3;
    hero3<<"瑞兹"<<"符文法师";
    QStringList hero4;
    hero4<<"奥丽安娜"<<"发条魔灵";
    //添加子节点
    QTreeWidgetItem * h1 = new QTreeWidgetItem(hero1);
    adItem->addChild(h1);
    QTreeWidgetItem * h2 = new QTreeWidgetItem(hero2);
    adItem->addChild(h2);
    QTreeWidgetItem * h3 = new QTreeWidgetItem(hero3);
    apItem->addChild(h3);
    QTreeWidgetItem * h4 = new QTreeWidgetItem(hero4);
    apItem->addChild(h4);
}

Widget::~Widget()
{
    delete ui;
}
