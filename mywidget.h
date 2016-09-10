#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QEvent>

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = 0);

signals:
    void getFocus();

public slots:

private:
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // MYWIDGET_H
