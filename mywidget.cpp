#include "mywidget.h"

#include <QDebug>

MyWidget::MyWidget(QWidget *parent) : QWidget(parent)
{
    this->installEventFilter(this);
}

bool MyWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        emit getFocus();
    }
    return false;
}
