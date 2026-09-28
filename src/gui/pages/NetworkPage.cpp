#include "pages/NetworkPage.h"

#include "gui/common/CommonWidgets.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
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
    m_mode->addItems({"TCP 服务端", "TCP 客户端", "UDP"});
    m_ip = new QLineEdit("127.0.0.1");
    m_port = new QSpinBox;
    m_port->setRange(1, 65535);
    m_port->setValue(5000);

    m_toggleBtn = new QPushButton("启动服务", this);
    m_toggleBtn->setObjectName("PrimaryButton");
    m_led = new LedIndicator(this);
    m_statusLabel = new QLabel("未启动", this);
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
    addField("地址", m_ip);
    addField("端口", m_port);
    cfgRow->addSpacing(8);
    cfgRow->addWidget(m_led);
    cfgRow->addWidget(m_statusLabel);
    cfgRow->addStretch();
    cfgRow->addWidget(m_toggleBtn);
    configCard->bodyLayout()->addLayout(cfgRow);

    root->addWidget(configCard);

    // ---- clients + log ----
    auto *midRow = new QHBoxLayout;
    midRow->setSpacing(16);

    auto *clientsCard = new Card(this);
    clientsCard->setTitle("客户端列表");
    clientsCard->setSubtitle("当前在线连接");
    m_clients = new QTableWidget(this);
    m_clients->setColumnCount(5);
    m_clients->setHorizontalHeaderLabels({"ID", "地址", "端口", "连接时间", "状态"});
    m_clients->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_clients->verticalHeader()->setVisible(false);
    m_clients->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_clients->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_clients->setShowGrid(false);
    m_clients->setAlternatingRowColors(true);
    clientsCard->bodyLayout()->addWidget(m_clients);

    auto *logCard = new Card(this);
    logCard->setTitle("通信日志");
    logCard->setSubtitle("实时收发记录");
    m_log = new QPlainTextEdit(this);
    m_log->setReadOnly(true);
    m_log->setPlaceholderText("连接建立后，收发日志将显示在这里…");
    logCard->bodyLayout()->addWidget(m_log);

    midRow->addWidget(clientsCard, 3);
    midRow->addWidget(logCard, 2);
    root->addLayout(midRow, 1);

    connect(m_toggleBtn, &QPushButton::clicked, this, [this]() {
        m_running = !m_running;
        const QString ts = QDateTime::currentDateTime().toString("HH:mm:ss");
        if (m_running) {
            m_toggleBtn->setText("停止服务");
            m_led->setColor(QColor("#22c55e"));
            m_statusLabel->setText(QString("运行中 · %1").arg(m_mode->currentText()));
            m_log->appendPlainText(QString("[%1] [系统] %2 已启动 @ %3:%4")
                                       .arg(ts, m_mode->currentText(),
                                            m_ip->text(),
                                            QString::number(m_port->value())));

            m_clients->setRowCount(2);
            m_clients->setItem(0, 0, new QTableWidgetItem("1001"));
            m_clients->setItem(0, 1, new QTableWidgetItem("192.168.1.20"));
            m_clients->setItem(0, 2, new QTableWidgetItem("50824"));
            m_clients->setItem(0, 3, new QTableWidgetItem(ts));
            m_clients->setItem(0, 4, new QTableWidgetItem("在线"));
            m_clients->setItem(1, 0, new QTableWidgetItem("1002"));
            m_clients->setItem(1, 1, new QTableWidgetItem("192.168.1.33"));
            m_clients->setItem(1, 2, new QTableWidgetItem("51002"));
            m_clients->setItem(1, 3, new QTableWidgetItem(ts));
            m_clients->setItem(1, 4, new QTableWidgetItem("在线"));
            m_clients->item(0, 4)->setForeground(QColor("#22c55e"));
            m_clients->item(1, 4)->setForeground(QColor("#22c55e"));
        } else {
            m_toggleBtn->setText("启动服务");
            m_led->setColor(QColor("#8b90a0"));
            m_statusLabel->setText("未启动");
            m_log->appendPlainText(QString("[%1] [系统] 服务已停止").arg(ts));
            m_clients->setRowCount(0);
        }
    });
}
