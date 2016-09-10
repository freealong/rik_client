#include "visualize.h"
#include "ui_visualize.h"

Visualize::Visualize(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Visualize),
    show_pose(false),
    show_joints(false)
{
    ui->setupUi(this);

    plot_pose = ui->widget_pose;
    plot_joints = ui->widget_joints;

    for (unsigned i = 0; i < 6; i++)
    {
        plot_pose->addGraph();
//        plot_pose->graph(0)->setAntialiasedFill(false);
    }
//    plot_pose->addGraph(); // blue line
//    plot_pose->graph(0)->setPen(QPen(Qt::blue));
//    plot_pose->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
//    plot_pose->graph(0)->setAntialiasedFill(false);
//    plot_pose->addGraph(); // red line
//    plot_pose->graph(1)->setPen(QPen(Qt::red));
//    plot_pose->graph(1)->setAntialiasedFill(false);
//    plot_pose->addGraph(); // yellow line
//    plot_pose->graph(2)->setPen(QPen(Qt::yellow));
//    plot_pose->graph(2)->setAntialiasedFill(false);
//    plot_pose->addGraph(); // green line
//    plot_pose->graph(3)->setPen(QPen(Qt::green));
//    plot_pose->graph(3)->setAntialiasedFill(false);
//    plot_pose->addGraph(); // gray line
//    plot_pose->graph(4)->setPen(QPen(Qt::gray));
//    plot_pose->graph(4)->setAntialiasedFill(false);
//    plot_pose->addGraph(); // black line
//    plot_pose->graph(5)->setPen(QPen(Qt::black));
//    plot_pose->graph(5)->setAntialiasedFill(false);

    plot_pose->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    plot_pose->xAxis->setDateTimeFormat("hh:mm:ss");
    plot_pose->xAxis->setAutoTickStep(false);
    plot_pose->xAxis->setTickStep(2);
    plot_pose->axisRect()->setupFullAxesBox();

//    for (unsigned i = 0; i < 6; i++)
//    {
//        plot_joints->addGraph();
//    }
    plot_joints->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    plot_joints->xAxis->setDateTimeFormat("hh:mm:ss");
    plot_joints->xAxis->setAutoTickStep(false);
    plot_joints->xAxis->setTickStep(2);
    plot_joints->axisRect()->setupFullAxesBox();

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(plot_pose->xAxis, SIGNAL(rangeChanged(QCPRange)), plot_pose->xAxis2, SLOT(setRange(QCPRange)));
    connect(plot_pose->yAxis, SIGNAL(rangeChanged(QCPRange)), plot_pose->yAxis2, SLOT(setRange(QCPRange)));
    connect(plot_joints->xAxis, SIGNAL(rangeChanged(QCPRange)), plot_joints->xAxis2, SLOT(setRange(QCPRange)));
    connect(plot_joints->yAxis, SIGNAL(rangeChanged(QCPRange)), plot_joints->yAxis2, SLOT(setRange(QCPRange)));
}

Visualize::~Visualize()
{
    delete ui;
}

void Visualize::update_pose(Eigen::VectorXf &v)
{
    double now = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    QString pose("pose: ");
    for (unsigned i = 0; i < v.size(); i++)
    {
        if (ui->list_pose->item(i)->checkState() == Qt::Checked)
        {
            plot_pose->graph(i)->addData(now, v(i));
            // remove data of lines that's outside visible range:
            plot_pose->graph(i)->removeDataBefore(now-8);
            // rescale value (vertical) axis to fit the current data:
            plot_pose->graph(i)->rescaleValueAxis();
            // make key axis range scroll with the data (at a constant range size of 8):
            plot_pose->xAxis->setRange(now + 0.25, 8, Qt::AlignRight);
        }
        else
        {
            plot_pose->graph(i)->clearData();
        }
        // compose data to string
        pose += QString::number(v(i));
        pose += QString(", ");
    }
    // show data in graph
    plot_pose->replot();
    // show data in label
    ui->label_pose->setText(pose);
}

void Visualize::update_joints(Eigen::VectorXf &v)
{
    double now = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    QString joints("joints: ");
    for (unsigned i = 0; i < v.size(); i++)
    {
        if (ui->list_joints->item(i)->checkState() == Qt::Checked)
        {
            plot_joints->graph(i)->addData(now, v(i));
            plot_joints->graph(i)->removeDataBefore(now-8);
            plot_joints->graph(i)->rescaleValueAxis();
            plot_joints->xAxis->setRange(now + 0.25, 8, Qt::AlignRight);
        }
        else
        {
            plot_joints->graph(i)->clearData();
        }

        joints += QString::number(v(i));
        joints += QString(", ");
    }
    // show data in graph
    plot_joints->replot();
    ui->label_joints->setText(joints);
}

void Visualize::update_joints_widget(dh_table &t)
{
    ui->list_joints->clear();
    plot_joints->clearGraphs();
    for (unsigned i = 0; i < t.size(); ++i)
    {
        // update joints list
        QListWidgetItem* item = new QListWidgetItem();
        item->setText(QString::fromStdString(t[i].joint_name));
        item->setCheckState(Qt::Unchecked);
        ui->list_joints->addItem(item);
        // updat joints graph
        plot_joints->addGraph();
        // plot_pose->graph(0)->setAntialiasedFill(false);
    }
}

void Visualize::on_button_pose_clicked()
{
    show_pose = !show_pose;
    emit pose_request(show_pose);
}

void Visualize::on_button_joints_clicked()
{
    show_joints = !show_joints;
    emit joints_request(show_joints);
}
