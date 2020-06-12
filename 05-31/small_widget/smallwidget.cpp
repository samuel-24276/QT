#include "smallwidget.h"
#include "ui_smallwidget.h"

smallWidget::smallWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::smallWidget)
{
    ui->setupUi(this);

    //QSpinBox动，QSlider跟着动
    void(QSpinBox::* spinSignal)(int) = &QSpinBox::valueChanged;
    connect(ui->spinBox, spinSignal, ui->horizontalSlider, &QSlider::setValue);
    //QSlider动，QSpinBox跟着动
    connect(ui->horizontalSlider, &QSlider::valueChanged, ui->spinBox, &QSpinBox::setValue);
}

void smallWidget::setData(int val)
{
    ui->spinBox->setValue(val);
}

int smallWidget::getData()
{
    return ui->spinBox->value();
}

smallWidget::~smallWidget()
{
    delete ui;
}
