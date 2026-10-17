#include "gui/pages/NetworkPage.h"

#include "gui/CommonWidgets.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QDateTime>

NetworkPage::NetworkPage(QWidget *parent)
    : QWidget(parent)
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 20, 24, 20);
    root->setSpacing(16);

    // ---- config bar ----
    auto *configCard = new Card(this);
    configCard->setTitle("服务配置");
    configCard->setSubtitle("TCP / UDP 通信参数");

    m_mode = new QComboBox;
    m_mode->addItems({"TCP", "UDP"});
    m_ip = new QLineEdit("127.0.0.1");
    m_port = new QSpinBox;
    m_port->setRange(1, 65535);
    m_port->setValue(5000);

    m_toggleBtn = new QPushButton("连接", this);
    m_toggleBtn->setObjectName("PrimaryButton");
    m_led = new LedIndicator(this);
    m_statusLabel = new QLabel("未连接", this);
    m_statusLabel->setObjectName("StatusText");

    auto *cfgRow = new QHBoxLayout;
    cfgRow->setSpacing(12);
    auto addField = [&](const QString &label, QWidget *w) {
        auto *l = new QLabel(label, this);
        l->setObjectName("FieldLabel");
        auto *h = new QHBoxLayout;
        h->setSpacing(6);
        h->addWidget(l);
        h->addWidget(w);
        cfgRow->addLayout(h);
    };
    addField("模式", m_mode);
    addField("IP地址", m_ip);
    addField("端口", m_port);
    cfgRow->addSpacing(8);
    cfgRow->addWidget(m_led);
    cfgRow->addWidget(m_statusLabel);
    cfgRow->addStretch();
    cfgRow->addWidget(m_toggleBtn);
    configCard->bodyLayout()->addLayout(cfgRow);

    root->addWidget(configCard);

    // ---- log + devices ----
    auto *logCard = new Card(this);
    logCard->setTitle("通信日志");
    logCard->setSubtitle("实时收发记录");
    m_log = new QPlainTextEdit(this);
    m_log->setReadOnly(true);
    m_log->setPlaceholderText("连接建立后，收发日志将显示在这里…");
    logCard->bodyLayout()->addWidget(m_log);

    m_devices = new DeviceListCard(this);

    auto *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setChildrenCollapsible(false);
    splitter->addWidget(logCard);
    splitter->addWidget(m_devices);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 2);
    splitter->setSizes({700, 440});
    root->addWidget(splitter, 1);

    connect(m_mode, &QComboBox::currentIndexChanged, this, [this]() {
        saveState();
        m_currentMode = m_mode->currentText();
        applyState();
    });

    connect(m_toggleBtn, &QPushButton::clicked, this, [this]() {
        const QString ts = QDateTime::currentDateTime().toString("HH:mm:ss");
        const QString mode = m_mode->currentText();

        if (mode == "UDP") {
            m_led->setColor(QColor("#f59e0b"));
            m_statusLabel->setText("测试中");
            m_log->appendPlainText(QString("[%1] [UDP] 测试连接 %2:%3 …")
                                       .arg(ts, m_ip->text(),
                                            QString::number(m_port->value())));
            return;
        }

        m_running = !m_running;
        if (m_running) {
            m_toggleBtn->setText(runningText());
            m_led->setColor(QColor("#22c55e"));
            m_statusLabel->setText("已连接");
            m_devices->connectDevice("网络 (TCP)",
                                     QString("%1:%2").arg(m_ip->text(),
                                                          QString::number(m_port->value())));
            m_log->appendPlainText(QString("[%1] [系统] 已连接到 %2:%3")
                                       .arg(ts, m_ip->text(),
                                            QString::number(m_port->value())));
        } else {
            m_toggleBtn->setText(idleText());
            m_led->setColor(QColor("#8b90a0"));
            m_statusLabel->setText("未连接");
            m_devices->disconnectDevice("网络 (TCP)");
            m_log->appendPlainText(QString("[%1] [系统] 连接已断开").arg(ts));
        }
    });

    m_currentMode = m_mode->currentText();
    for (const QString &m : {QString("TCP"), QString("UDP")}) {
        m_states.insert(m, ModeState{});
    }
}

QString NetworkPage::idleText() const
{
    return m_mode->currentText() == "TCP" ? "连接" : "测试连接";
}

QString NetworkPage::runningText() const
{
    return "断开";
}

void NetworkPage::saveState()
{
    ModeState &s = m_states[m_currentMode];
    s.running = m_running;
    s.status = m_statusLabel->text();
    s.led = m_led->color();
}

void NetworkPage::applyState()
{
    const ModeState s = m_states.value(m_currentMode);
    m_running = s.running;
    m_led->setColor(s.led);
    m_statusLabel->setText(s.status);
    m_toggleBtn->setText(m_running ? runningText() : idleText());
}
