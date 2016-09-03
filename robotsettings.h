#ifndef ROBOTSETTINGS_H
#define ROBOTSETTINGS_H

#include <QWidget>

namespace Ui {
class RobotSettings;
}

class RobotSettings : public QWidget
{
    Q_OBJECT

public:
    explicit RobotSettings(QWidget *parent = 0);
    ~RobotSettings();

signals:
    void table_request();

private slots:
    void on_button_download_clicked();

private:
    Ui::RobotSettings *ui;
};

#endif // ROBOTSETTINGS_H
