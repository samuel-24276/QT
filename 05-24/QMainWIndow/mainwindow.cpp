#include "mainwindow.h"
#include<QMenuBar>
#include<QToolBar>
#include<QPushButton>
#include<QLabel>
#include<QStatusBar>
#include<QDockWidget>
#include<QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(600, 400);

    //1.菜单栏
    QMenuBar * bar = menuBar();
    //将菜单栏设置到窗口中
    setMenuBar(bar);
    //设置菜单并添加菜单项
    QMenu * fileMenu = bar->addMenu("文件");
    QMenu * editMenu = bar->addMenu("编辑");
    QAction * newAction = fileMenu->addAction("新建");
    //添加分割线
    fileMenu->addSeparator();
    QAction * openAction = fileMenu->addAction("打开");

    //2.添加工具栏，可以有多个
    QToolBar * toolBar = new QToolBar;
    //将工具栏放到窗口中,Qt::内是所有枚举的值
    addToolBar(Qt::LeftToolBarArea, toolBar);
    //只允许左右停靠
    toolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);
    //设置浮动
    toolBar->setFloatable(false);
    //设置移动（总开关）
    toolBar->setMovable(false);
    //工具栏中添加小控件
    QPushButton * btn = new QPushButton("按钮", this);
    toolBar->addWidget(btn);
    //工具栏中添加菜单项
    toolBar->addAction(newAction);
    //tian'jia'feng添加分割线
    toolBar->addSeparator();
    toolBar->addAction(openAction);

    //3.添加状态栏
    QStatusBar * status = statusBar();
    //将状态栏放入窗口中
    setStatusBar(status);
    //放入标签
    QLabel * label = new QLabel("左侧信息", this);
    //将标签放入状态栏中
    status->addWidget(label);
    //从右往左放
    QLabel * label2 = new QLabel("右侧信息", this);
    status->addPermanentWidget(label2);

    //4.铆接部件，浮动窗口，可以有多个
    QDockWidget * dock = new QDockWidget("铆接部件", this);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    //设置停靠范围为顶部，不能左右停靠
    dock->setAllowedAreas(Qt::TopDockWidgetArea);

    //5.核心部件,只能有一个
    QTextEdit * edit = new QTextEdit(this);
    setCentralWidget(edit);
}

MainWindow::~MainWindow()
{
}

