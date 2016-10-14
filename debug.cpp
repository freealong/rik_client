#include "debug.h"
#include "ui_debug.h"

Debug::Debug(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Debug)
{
    ui->setupUi(this);
}

Debug::~Debug()
{
    delete ui;
}
