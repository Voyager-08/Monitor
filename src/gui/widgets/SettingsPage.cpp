#include "gui/widgets/SettingsPage.h"

#include "gui/widgets/CommonWidgets.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>

SettingsPage::SettingsPage(QWidget *parent)
    : QWidget(parent)
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 20, 24, 20);
    root->setSpacing(16);

    auto *grid = new QHBoxLayout;
    grid->setSpacing(16);

    auto makeFormCard = [&](const QString &title, const QString &subtitle, Card *&out) {
        out = new Card(this);
        out->setTitle(title);
        out->setSubtitle(subtitle);
        out->setMinimumWidth(360);
        grid->addWidget(out);
    };

    // appearance
    Card *appCard = nullptr;
    makeFormCard("外观", "界面与显示设置", appCard);
    auto *theme = new QComboBox;
    theme->addItems({"深色科技风", "浅色简洁风"});
    auto *lang = new QComboBox;
    lang->addItems({"简体中文", "English"});
    auto *appForm = new QFormLayout;
    appForm->setSpacing(10);
    appForm->setLabelAlignment(Qt::AlignRight);
    auto appLabel = [&](const QString &t) {
        auto *l = new QLabel(t, this);
        l->setObjectName("FieldLabel");
        return l;
    };
    appForm->addRow(appLabel("主题"), theme);
    appForm->addRow(appLabel("语言"), lang);
    appCard->bodyLayout()->addLayout(appForm);
    appCard->bodyLayout()->addStretch();

    // communication
    Card *commCard = nullptr;
    makeFormCard("通信", "连接可靠性设置", commCard);
    auto *reconnect = new QCheckBox("断线自动重连", this);
    reconnect->setChecked(true);
    auto *heartbeat = new QSpinBox;
    heartbeat->setRange(1, 120);
    heartbeat->setValue(5);
    heartbeat->setSuffix(" 秒");
    auto *timeout = new QSpinBox;
    timeout->setRange(100, 10000);
    timeout->setValue(3000);
    timeout->setSuffix(" ms");
    auto *commForm = new QFormLayout;
    commForm->setSpacing(10);
    commForm->setLabelAlignment(Qt::AlignRight);
    auto commLabel = [&](const QString &t) {
        auto *l = new QLabel(t, this);
        l->setObjectName("FieldLabel");
        return l;
    };
    commForm->addRow(commLabel("重连策略"), reconnect);
    commForm->addRow(commLabel("心跳间隔"), heartbeat);
    commForm->addRow(commLabel("通信超时"), timeout);
    commCard->bodyLayout()->addLayout(commForm);
    commCard->bodyLayout()->addStretch();

    root->addLayout(grid);

    // about
    auto *aboutCard = new Card(this);
    aboutCard->setTitle("关于");
    aboutCard->setSubtitle("应用信息");
    auto *name = new QLabel("Monitor 监控上位机", this);
    name->setObjectName("AboutName");
    auto *ver = new QLabel("版本 v0.1.0 · 构建日期 2026-09-28", this);
    ver->setObjectName("AboutDetail");
    auto *desc = new QLabel("基于 Qt 6 Widgets · 支持串口 / TCP / UDP / Modbus 通信", this);
    desc->setObjectName("AboutDetail");
    auto *saveBtn = new QPushButton("保存设置", this);
    saveBtn->setObjectName("PrimaryButton");
    saveBtn->setFixedWidth(120);

    aboutCard->bodyLayout()->addWidget(name);
    aboutCard->bodyLayout()->addWidget(ver);
    aboutCard->bodyLayout()->addWidget(desc);
    aboutCard->bodyLayout()->addSpacing(8);
    aboutCard->bodyLayout()->addWidget(saveBtn, 0, Qt::AlignLeft);

    root->addWidget(aboutCard);
}
