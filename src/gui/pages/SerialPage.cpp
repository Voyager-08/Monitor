#include "pages/SerialPage.h"

#include "gui/common/CommonWidgets.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QDateTime>

static QLabel *makeFieldLabel(const QString &text, QWidget *parent)
{
    auto *l = new QLabel(text, parent);
    l->setObjectName("FieldLabel");
    return l;
}

SerialPage::SerialPage(QWidget *parent)
    : QWidget(parent)
{
    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(24, 20, 24, 20);
    root->setSpacing(16);

    // ---- left: settings ----
    auto *settingsCard = new Card(this);
    settingsCard->setTitle("串口设置");
    settingsCard->setSubtitle("配置并打开串口连接");
    settingsCard->setFixedWidth(300);

    m_port = new QComboBox;
    for (int i = 1; i <= 16; ++i)
        m_port->addItem(QString("COM%1").arg(i));
    m_baud = new QComboBox;
    m_baud->addItems({"9600", "19200", "38400", "57600", "115200", "230400"});
    m_baud->setCurrentText("115200");
    m_dataBits = new QComboBox;
    m_dataBits->addItems({"8", "7", "6", "5"});
    m_stopBits = new QComboBox;
    m_stopBits->addItems({"1", "1.5", "2"});
    m_parity = new QComboBox;
    m_parity->addItems({"无校验", "奇校验", "偶校验"});

    auto *form = new QFormLayout;
    form->setSpacing(10);
    form->setLabelAlignment(Qt::AlignRight);
    form->addRow(makeFieldLabel("端口", this), m_port);
    form->addRow(makeFieldLabel("波特率", this), m_baud);
    form->addRow(makeFieldLabel("数据位", this), m_dataBits);
    form->addRow(makeFieldLabel("停止位", this), m_stopBits);
    form->addRow(makeFieldLabel("校验位", this), m_parity);
    settingsCard->bodyLayout()->addLayout(form);

    m_openBtn = new QPushButton("打开串口", this);
    m_openBtn->setObjectName("PrimaryButton");

    m_led = new LedIndicator(this);
    m_statusLabel = new QLabel("未连接", this);
    m_statusLabel->setObjectName("StatusText");

    auto *statusRow = new QHBoxLayout;
    statusRow->addWidget(m_led);
    statusRow->addWidget(m_statusLabel);
    statusRow->addStretch();

    settingsCard->bodyLayout()->addSpacing(4);
    settingsCard->bodyLayout()->addLayout(statusRow);
    settingsCard->bodyLayout()->addWidget(m_openBtn);

    // ---- right: send / recv ----
    auto *ioCard = new Card(this);
    ioCard->setTitle("数据收发");
    ioCard->setSubtitle("接收与发送数据");

    auto *recvLabel = new QLabel("接收区", this);
    recvLabel->setObjectName("SectionTitle");
    m_recv = new QPlainTextEdit(this);
    m_recv->setReadOnly(true);
    m_recv->setPlaceholderText("接收到的数据将显示在这里…");

    auto *sendLabel = new QLabel("发送区", this);
    sendLabel->setObjectName("SectionTitle");
    m_send = new QPlainTextEdit(this);
    m_send->setPlaceholderText("输入要发送的内容…");

    m_hexRecv = new QCheckBox("HEX 显示", this);
    m_hexSend = new QCheckBox("HEX 发送", this);
    auto *clearBtn = new QPushButton("清空", this);
    clearBtn->setObjectName("GhostButton");
    auto *sendBtn = new QPushButton("发送", this);
    sendBtn->setObjectName("PrimaryButton");

    auto *toolRow = new QHBoxLayout;
    toolRow->addWidget(m_hexRecv);
    toolRow->addWidget(m_hexSend);
    toolRow->addStretch();
    toolRow->addWidget(clearBtn);
    toolRow->addWidget(sendBtn);

    ioCard->bodyLayout()->addWidget(recvLabel);
    ioCard->bodyLayout()->addWidget(m_recv, 3);
    ioCard->bodyLayout()->addWidget(sendLabel);
    ioCard->bodyLayout()->addWidget(m_send, 2);
    ioCard->bodyLayout()->addLayout(toolRow);

    root->addWidget(settingsCard);
    root->addWidget(ioCard, 1);

    connect(m_openBtn, &QPushButton::clicked, this, [this]() {
        m_opened = !m_opened;
        if (m_opened) {
            m_openBtn->setText("关闭串口");
            m_led->setColor(QColor("#22c55e"));
            m_statusLabel->setText(QString("已连接 · %1 @ %2")
                                       .arg(m_port->currentText(), m_baud->currentText()));
            appendLog("[系统] 串口打开成功");
        } else {
            m_openBtn->setText("打开串口");
            m_led->setColor(QColor("#8b90a0"));
            m_statusLabel->setText("未连接");
            appendLog("[系统] 串口已关闭");
        }
    });

    connect(sendBtn, &QPushButton::clicked, this, [this]() {
        const QString data = m_send->toPlainText();
        if (data.isEmpty())
            return;
        appendLog(QString("[发送] %1").arg(data));
        m_send->clear();
    });

    connect(clearBtn, &QPushButton::clicked, this, [this]() { m_recv->clear(); });
}

void SerialPage::appendLog(const QString &text)
{
    const QString ts = QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
    m_recv->appendPlainText(QString("[%1] %2").arg(ts, text));
}
