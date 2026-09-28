#include "pages/LogPage.h"

#include "gui/common/CommonWidgets.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QDateTime>

LogPage::LogPage(QWidget *parent)
    : QWidget(parent)
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 20, 24, 20);
    root->setSpacing(16);

    auto *card = new Card(this);
    card->setTitle("系统日志");
    card->setSubtitle("运行日志与错误记录");

    // filter bar
    m_level = new QComboBox;
    m_level->addItems({"全部级别", "调试", "信息", "警告", "错误"});
    m_keyword = new QLineEdit;
    m_keyword->setPlaceholderText("搜索关键字…");
    m_keyword->setClearButtonEnabled(true);
    auto *clearBtn = new QPushButton("清空日志", this);
    clearBtn->setObjectName("GhostButton");

    auto *filterRow = new QHBoxLayout;
    filterRow->addWidget(m_level);
    filterRow->addWidget(m_keyword, 1);
    filterRow->addWidget(clearBtn);
    card->bodyLayout()->addLayout(filterRow);

    // table
    m_table = new QTableWidget(this);
    m_table->setColumnCount(4);
    m_table->setHorizontalHeaderLabels({"时间", "级别", "来源", "内容"});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    m_table->verticalHeader()->setVisible(false);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setShowGrid(false);
    m_table->setAlternatingRowColors(true);
    card->bodyLayout()->addWidget(m_table);

    root->addWidget(card);

    // demo rows
    struct Entry { QString level, source, msg; };
    const QVector<Entry> entries = {
        {"信息", "串口服务", "COM3 串口打开成功"},
        {"信息", "TCP 服务", "客户端 192.168.1.20 已连接"},
        {"调试", "Modbus", "发送读取请求：功能码 03，起始地址 0"},
        {"警告", "心跳检测", "通道 CH-04 超时未响应，准备重连"},
        {"错误", "UDP 服务", "端口 6000 绑定失败：地址已被占用"},
        {"信息", "数据库", "日志表写入成功，共 128 条记录"},
        {"调试", "协议解析", "收到完整包，长度 32 字节"},
        {"警告", "网络", "发送队列积压超过阈值"},
    };

    int sec = 0;
    for (const auto &e : entries) {
        const QString ts = QDateTime::currentDateTime()
                               .addSecs(-(entries.size() - sec++) * 7)
                               .toString("yyyy-MM-dd HH:mm:ss");
        appendRow(ts, e.level, e.source, e.msg);
    }

    connect(m_level, &QComboBox::currentTextChanged, this, [this](const QString &) {
        applyFilter();
    });
    connect(m_keyword, &QLineEdit::textChanged, this, [this](const QString &) {
        applyFilter();
    });
    connect(clearBtn, &QPushButton::clicked, this, [this]() { m_table->setRowCount(0); });
}

void LogPage::applyFilter()
{
    const QString level = m_level->currentText();
    const QString kw = m_keyword->text().trimmed();

    for (int r = 0; r < m_table->rowCount(); ++r) {
        const QString lv = m_table->item(r, 1)->text();
        const QString content = m_table->item(r, 3)->text();
        const bool levelOk = (level == "全部级别") || (lv == level);
        const bool kwOk = kw.isEmpty() || content.contains(kw, Qt::CaseInsensitive);
        m_table->setRowHidden(r, !(levelOk && kwOk));
    }
}

void LogPage::appendRow(const QString &time, const QString &level,
                        const QString &source, const QString &msg)
{
    const int row = m_table->rowCount();
    m_table->insertRow(row);
    m_table->setItem(row, 0, new QTableWidgetItem(time));
    auto *lv = new QTableWidgetItem(level);
    QColor color = QColor("#8b90a0");
    if (level == "调试") color = QColor("#64748b");
    else if (level == "信息") color = QColor("#22c55e");
    else if (level == "警告") color = QColor("#f59e0b");
    else if (level == "错误") color = QColor("#ef4444");
    lv->setForeground(color);
    m_table->setItem(row, 1, lv);
    m_table->setItem(row, 2, new QTableWidgetItem(source));
    m_table->setItem(row, 3, new QTableWidgetItem(msg));
}
