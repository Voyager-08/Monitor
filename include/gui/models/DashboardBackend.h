#pragma once

#include <QObject>
#include <QVariantList>
#include <QString>

class QTimer;

class DashboardBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool serverOnline READ serverOnline NOTIFY dataChanged)
    Q_PROPERTY(QString serverName READ serverName CONSTANT)
    Q_PROPERTY(QString version READ version CONSTANT)
    Q_PROPERTY(QString uptime READ uptime NOTIFY dataChanged)
    Q_PROPERTY(int cpu READ cpu NOTIFY dataChanged)
    Q_PROPERTY(int memory READ memory NOTIFY dataChanged)
    Q_PROPERTY(int disk READ disk NOTIFY dataChanged)
    Q_PROPERTY(double netUp READ netUp NOTIFY dataChanged)
    Q_PROPERTY(double netDown READ netDown NOTIFY dataChanged)
    Q_PROPERTY(int onlineDevices READ onlineDevices NOTIFY dataChanged)
    Q_PROPERTY(int totalDevices READ totalDevices CONSTANT)
    Q_PROPERTY(int alarmLevel READ alarmLevel NOTIFY dataChanged)
    Q_PROPERTY(QString alarmText READ alarmText NOTIFY dataChanged)
    Q_PROPERTY(int alarmCount READ alarmCount NOTIFY dataChanged)
    Q_PROPERTY(QVariantList series READ series NOTIFY seriesChanged)
    Q_PROPERTY(QVariantList deviceStates READ deviceStates NOTIFY dataChanged)

public:
    explicit DashboardBackend(QObject *parent = nullptr);

    bool serverOnline() const { return m_serverOnline; }
    QString serverName() const { return m_serverName; }
    QString version() const { return m_version; }
    QString uptime() const;
    int cpu() const { return m_cpu; }
    int memory() const { return m_memory; }
    int disk() const { return m_disk; }
    double netUp() const { return m_netUp; }
    double netDown() const { return m_netDown; }
    int onlineDevices() const { return m_onlineDevices; }
    int totalDevices() const { return m_totalDevices; }
    int alarmLevel() const { return m_alarmLevel; }
    QString alarmText() const { return m_alarmText; }
    int alarmCount() const { return m_alarmCount; }
    QVariantList series() const { return m_series; }
    QVariantList deviceStates() const { return m_deviceStates; }

    Q_INVOKABLE void acknowledgeAlarm();

signals:
    void dataChanged();
    void seriesChanged();

private:
    void tick();

    QTimer *m_timer = nullptr;
    bool m_serverOnline = true;
    QString m_serverName = QStringLiteral("Monitor 主服务");
    QString m_version = QStringLiteral("v0.1.0");
    long long m_uptimeSeconds = 3725;

    int m_cpu = 38;
    int m_memory = 56;
    int m_disk = 62;
    double m_netUp = 128.4;
    double m_netDown = 512.7;

    int m_onlineDevices = 12;
    int m_totalDevices = 16;

    int m_alarmLevel = 0;
    QString m_alarmText = QStringLiteral("当前无报警，系统运行正常");
    int m_alarmCount = 0;

    QVariantList m_series;
    QVariantList m_deviceStates;
};
