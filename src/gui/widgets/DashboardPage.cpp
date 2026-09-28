#include "gui/widgets/DashboardPage.h"

#include "gui/widgets/CommonWidgets.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QTimer>
#include <QDateTime>
#include <QRandomGenerator>

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent)
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 20, 24, 20);
    root->setSpacing(16);

    // ---- stat cards ----
    auto *statsRow = new QHBoxLayout;
    statsRow->setSpacing(16);

    m_online = new StatCard("设备在线", "12", "台", QColor("#22c55e"), this);
    m_messages = new StatCard("今日消息", "12,840", "条", QColor("#3b82f6"), this);
    m_throughput = new StatCard("数据吞吐", "1.24", "KB/s", QColor("#a855f7"), this);
    m_uptime = new StatCard("运行时长", "01:02:05", "", QColor("#f59e0b"), this);

    m_online->setTrend("+2 较昨日", true);
    m_messages->setTrend("+18%", true);
    m_throughput->setTrend("稳定", true);
    m_uptime->setTrend("持续运行中", true);

    statsRow->addWidget(m_online);
    statsRow->addWidget(m_messages);
    statsRow->addWidget(m_throughput);
    statsRow->addWidget(m_uptime);
    root->addLayout(statsRow);

    // ---- chart + devices ----
    auto *midRow = new QHBoxLayout;
    midRow->setSpacing(16);

    auto *chartCard = new Card(this);
    chartCard->setTitle("实时数据趋势");
    chartCard->setSubtitle("最近 60 秒采样");
    m_chart = new LineChart(this);
    m_chart->setTitle("吞吐量 (KB/s)");
    chartCard->bodyLayout()->addWidget(m_chart);

    auto *deviceCard = new Card(this);
    deviceCard->setTitle("通道状态");
    deviceCard->setSubtitle("实时连接概览");
    m_devices = new QTableWidget(this);
    m_devices->setColumnCount(4);
    m_devices->setHorizontalHeaderLabels({"通道", "类型", "地址", "状态"});
    m_devices->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_devices->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_devices->verticalHeader()->setVisible(false);
    m_devices->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_devices->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_devices->setShowGrid(false);
    m_devices->setAlternatingRowColors(true);
    deviceCard->bodyLayout()->addWidget(m_devices);

    midRow->addWidget(chartCard, 3);
    midRow->addWidget(deviceCard, 2);
    root->addLayout(midRow, 1);

    refreshDevices();

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [this]() {
        static double value = 50.0;
        value += (QRandomGenerator::global()->generateDouble() - 0.5) * 18.0;
        value = qBound(5.0, value, 95.0);
        m_chart->pushValue(value);
        m_throughput->setValue(QString::number(qMax(0.2, value * 0.02), 'f', 2));

        m_uptimeSeconds++;
        const int h = m_uptimeSeconds / 3600;
        const int m = (m_uptimeSeconds % 3600) / 60;
        const int s = m_uptimeSeconds % 60;
        m_uptime->setValue(QString("%1:%2:%3")
                               .arg(h, 2, 10, QChar('0'))
                               .arg(m, 2, 10, QChar('0'))
                               .arg(s, 2, 10, QChar('0')));

        if (QRandomGenerator::global()->bounded(10) == 0)
            m_messages->setValue(QString::number(++m_totalMessages));
    });
    m_timer->start(1000);
}

void DashboardPage::refreshDevices()
{
    m_devices->setRowCount(0);

    struct Row { QString channel, type, addr, status; };
    const QVector<Row> rows = {
        {"CH-01", "TCP", "192.168.1.10:5000", "在线"},
        {"CH-02", "串口", "COM3 · 115200", "在线"},
        {"CH-03", "Modbus RTU", "从站 01", "在线"},
        {"CH-04", "UDP", "0.0.0.0:6000", "离线"},
        {"CH-05", "TCP", "192.168.1.21:5001", "在线"},
        {"CH-06", "串口", "COM5 · 9600", "离线"},
    };

    for (const auto &r : rows) {
        const int row = m_devices->rowCount();
        m_devices->insertRow(row);
        m_devices->setItem(row, 0, new QTableWidgetItem(r.channel));
        m_devices->setItem(row, 1, new QTableWidgetItem(r.type));
        m_devices->setItem(row, 2, new QTableWidgetItem(r.addr));
        auto *status = new QTableWidgetItem(r.status);
        status->setForeground(r.status == "在线" ? QColor("#22c55e") : QColor("#ef4444"));
        m_devices->setItem(row, 3, status);
    }
    m_devices->setFixedHeight(6 * 40 + 40);
}
