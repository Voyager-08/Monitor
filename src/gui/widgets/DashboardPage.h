#pragma once

#include <QWidget>

class StatCard;
class LineChart;
class QTableWidget;
class QTimer;

class DashboardPage : public QWidget
{
    Q_OBJECT
public:
    explicit DashboardPage(QWidget *parent = nullptr);

private:
    void refreshDevices();

    StatCard *m_online = nullptr;
    StatCard *m_messages = nullptr;
    StatCard *m_throughput = nullptr;
    StatCard *m_uptime = nullptr;
    LineChart *m_chart = nullptr;
    QTableWidget *m_devices = nullptr;
    QTimer *m_timer = nullptr;
    int m_uptimeSeconds = 3725;
    int m_totalMessages = 12840;
};
