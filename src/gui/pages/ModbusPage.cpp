#include "pages/ModbusPage.h"

#include "gui/common/CommonWidgets.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QTimer>
#include <QDateTime>
#include <QRandomGenerator>

ModbusPage::ModbusPage(QWidget *parent)
    : QWidget(parent)
{
    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(24, 20, 24, 20);
    root->setSpacing(16);

    // ---- config ----
    auto *configCard = new Card(this);
    configCard->setTitle("从站配置");
    configCard->setSubtitle("Modbus 轮询参数");
    configCard->setFixedWidth(300);

    m_transport = new QComboBox;
    m_transport->addItems({"Modbus RTU", "Modbus TCP"});
    m_slave = new QSpinBox;
    m_slave->setRange(1, 247);
    m_slave->setValue(1);
    m_func = new QComboBox;
    m_func->addItems({"03 读保持寄存器", "04 读输入寄存器", "01 读线圈", "02 读离散输入",
                      "06 写单个寄存器", "16 写多个寄存器"});
    m_start = new QSpinBox;
    m_start->setRange(0, 65535);
    m_count = new QSpinBox;
    m_count->setRange(1, 125);
    m_count->setValue(10);

    auto *form = new QFormLayout;
    form->setSpacing(10);
    form->setLabelAlignment(Qt::AlignRight);
    auto addRow = [&](const QString &t, QWidget *w) {
        auto *l = new QLabel(t, this);
        l->setObjectName("FieldLabel");
        form->addRow(l, w);
    };
    addRow("传输方式", m_transport);
    addRow("从站地址", m_slave);
    addRow("功能码", m_func);
    addRow("起始地址", m_start);
    addRow("寄存器数量", m_count);
    configCard->bodyLayout()->addLayout(form);

    m_autoPoll = new QCheckBox("自动轮询", this);
    m_readBtn = new QPushButton("读取", this);
    m_readBtn->setObjectName("PrimaryButton");
    m_statusLabel = new QLabel("就绪", this);
    m_statusLabel->setObjectName("StatusText");

    auto *actionRow = new QHBoxLayout;
    actionRow->addWidget(m_autoPoll);
    actionRow->addStretch();
    actionRow->addWidget(m_readBtn);

    configCard->bodyLayout()->addSpacing(4);
    configCard->bodyLayout()->addLayout(actionRow);
    configCard->bodyLayout()->addWidget(m_statusLabel);

    // ---- registers ----
    auto *regCard = new Card(this);
    regCard->setTitle("寄存器数据");
    regCard->setSubtitle("实时寄存器值");

    m_regs = new QTableWidget(this);
    m_regs->setColumnCount(5);
    m_regs->setHorizontalHeaderLabels({"地址", "值 (HEX)", "值 (DEC)", "说明", "更新时间"});
    m_regs->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_regs->verticalHeader()->setVisible(false);
    m_regs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_regs->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_regs->setShowGrid(false);
    m_regs->setAlternatingRowColors(true);
    regCard->bodyLayout()->addWidget(m_regs);

    root->addWidget(configCard);
    root->addWidget(regCard, 1);

    connect(m_readBtn, &QPushButton::clicked, this, &ModbusPage::pollOnce);
    connect(m_autoPoll, &QCheckBox::toggled, this, [this](bool on) {
        if (on) {
            m_timer->start(1000);
            m_readBtn->setEnabled(false);
        } else {
            m_timer->stop();
            m_readBtn->setEnabled(true);
        }
    });

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &ModbusPage::pollOnce);

    pollOnce();
}

void ModbusPage::pollOnce()
{
    const int count = m_count->value();
    const int start = m_start->value();
    const QString ts = QDateTime::currentDateTime().toString("HH:mm:ss");

    m_regs->setRowCount(count);
    for (int i = 0; i < count; ++i) {
        const int addr = start + i;
        const quint16 value = QRandomGenerator::global()->bounded(0, 65536);

        m_regs->setItem(i, 0, new QTableWidgetItem(QString::number(addr)));
        m_regs->setItem(i, 1, new QTableWidgetItem(QString("0x%1")
                                                       .arg(value, 4, 16, QChar('0')).toUpper()));
        m_regs->setItem(i, 2, new QTableWidgetItem(QString::number(value)));
        m_regs->setItem(i, 3, new QTableWidgetItem("保持寄存器"));
        m_regs->setItem(i, 4, new QTableWidgetItem(ts));
    }

    m_statusLabel->setText(QString("已读取 %1 个寄存器 · %2").arg(count).arg(ts));
}
