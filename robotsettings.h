#ifndef ROBOTSETTINGS_H
#define ROBOTSETTINGS_H

#include <QWidget>

#include "sharetypes.h"

namespace Ui {
class RobotSettings;
}

class RobotSettings : public QWidget
{
    Q_OBJECT

public:
    explicit RobotSettings(QWidget *parent = 0);
    ~RobotSettings();
    void set_table(dh_table &t);
    void get_table(dh_table &t);

signals:
    void download_request();
    void upload_request();

private slots:
    void on_button_download_clicked();

    void on_button_upload_clicked();

    void on_button_add_clicked();

    void on_button_del_clicked();

private:
    Ui::RobotSettings *ui;
};

#endif // ROBOTSETTINGS_H
