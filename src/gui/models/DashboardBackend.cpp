#include "models/DashboardBackend.h"

#include <QTimer>
#include <QRandomGenerator>
#include <QtMath>

namespace {
double walk(double value, double step, double min, double max)
{
    value += (QRandomGenerator::global()->generateDouble() - 0.5) * step;
    return qBound(min, value, max);
}
}

DashboardBackend::DashboardBackend(QObject *parent)
    : QObject(parent)
{
    for (int i = 0; i < 60; ++i)
        m_series.append(40.0 + qSin(i / 5.0) * 12.0);

    for (int i = 0; i < m_totalDevices; ++i)
        m_deviceStates.append(i < m_onlineDevices ? 1 : 0);

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer, &QTimer::timeout, this, &DashboardBackend::tick);
    m_timer->start();
}

QString DashboardBackend::uptime() const
{
    const long long h = m_uptimeSeconds / 3600;
    const long long m = (m_uptimeSeconds % 3600) / 60;
    const long long s = m_uptimeSeconds % 60;
    return QString("%1:%2:%3")
        .arg(h, 2, 10, QChar('0'))
        .arg(m, 2, 10, QChar('0'))
        .arg(s, 2, 10, QChar('0'));
}

void DashboardBackend::acknowledgeAlarm()
{
    m_alarmLevel = 0;
    m_alarmText = QStringLiteral("报警已确认，系统运行正常");
    emit dataChanged();
}

void DashboardBackend::tick()
{
    m_uptimeSeconds++;

    m_cpu = int(walk(m_cpu, 22, 8, 96));
    m_memory = int(walk(m_memory, 10, 25, 92));
    m_disk = int(walk(m_disk, 2, 40, 88));
    m_netUp = walk(m_netUp, 160, 5, 900);
    m_netDown = walk(m_netDown, 380, 20, 1800);

    if (QRandomGenerator::global()->bounded(6) == 0) {
        m_onlineDevices = qBound(6, m_onlineDevices + (QRandomGenerator::global()->bounded(3) - 1),
                                 m_totalDevices);
        m_deviceStates.clear();
        for (int i = 0; i < m_totalDevices; ++i)
            m_deviceStates.append(i < m_onlineDevices ? 1 : 0);
    }

    // random alarm events
    if (m_alarmLevel == 0 && QRandomGenerator::global()->bounded(9) == 0) {
        m_alarmLevel = QRandomGenerator::global()->bounded(2) + 1; // 1 warn / 2 critical
        m_alarmCount++;
        m_alarmText = (m_alarmLevel == 2)
            ? QStringLiteral("CH-04 通道通信中断，请检查网络")
            : QStringLiteral("CPU 负载偏高，超过 85% 阈值");
    } else if (m_alarmLevel > 0 && QRandomGenerator::global()->bounded(4) == 0) {
        m_alarmLevel = 0;
        m_alarmText = QStringLiteral("当前无报警，系统运行正常");
    }

    m_series.append(m_cpu);
    while (m_series.size() > 60)
        m_series.removeFirst();

    emit dataChanged();
    emit seriesChanged();
}
