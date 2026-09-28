#include "gui/MainWindow.h"

#include "gui/common/CommonWidgets.h"
#include "pages/DashboardPage.h"
#include "pages/SerialPage.h"
#include "pages/NetworkPage.h"
#include "pages/ModbusPage.h"
#include "pages/LogPage.h"
#include "pages/SettingsPage.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QStackedWidget>
#include <QTimer>
#include <QDateTime>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Monitor 监控上位机");
    resize(1280, 800);
    setMinimumSize(1080, 680);

    m_stack = new QStackedWidget(this);

    auto *central = new QWidget(this);
    central->setObjectName("AppRoot");
    auto *rootLayout = new QHBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    rootLayout->addWidget(buildSidebar());

    auto *right = new QWidget(central);
    auto *rightLayout = new QVBoxLayout(right);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);
    rightLayout->addWidget(buildHeader());

    m_stack->addWidget(new DashboardPage(this));   // 0
    m_stack->addWidget(new SerialPage(this));      // 1
    m_stack->addWidget(new NetworkPage(this));     // 2
    m_stack->addWidget(new ModbusPage(this));      // 3
    m_stack->addWidget(new LogPage(this));         // 4
    m_stack->addWidget(new SettingsPage(this));    // 5

    rightLayout->addWidget(m_stack, 1);
    rootLayout->addWidget(right, 1);

    setCentralWidget(central);

    // status bar
    auto *statusBar = new QStatusBar(this);
    statusBar->setObjectName("StatusBar");
    auto *leftStatus = new QLabel(" 就绪", statusBar);
    leftStatus->setObjectName("StatusBarText");
    statusBar->addWidget(leftStatus);
    statusBar->addPermanentWidget(new QLabel("Monitor v0.1.0", statusBar));
    setStatusBar(statusBar);

    // clock (m_clockLabel 已在 buildHeader() 中创建)
    auto updateClock = [this]() {
        if (m_clockLabel)
            m_clockLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    };
    updateClock();
    auto *clockTimer = new QTimer(this);
    connect(clockTimer, &QTimer::timeout, this, updateClock);
    clockTimer->start(1000);

    setPage(0);
}

QWidget *MainWindow::buildSidebar()
{
    auto *sidebar = new QFrame(this);
    sidebar->setObjectName("Sidebar");
    sidebar->setFixedWidth(220);

    auto *layout = new QVBoxLayout(sidebar);
    layout->setContentsMargins(14, 20, 14, 16);
    layout->setSpacing(4);

    // logo
    auto *logo = new QLabel("MONITOR", sidebar);
    logo->setObjectName("LogoTitle");
    auto *logoSub = new QLabel("监控上位机", sidebar);
    logoSub->setObjectName("LogoSubtitle");

    auto *logoRow = new QHBoxLayout;
    logoRow->setSpacing(10);
    auto *logoBadge = new QLabel("M", sidebar);
    logoBadge->setObjectName("LogoBadge");
    logoBadge->setFixedSize(36, 36);
    logoBadge->setAlignment(Qt::AlignCenter);
    auto *logoText = new QVBoxLayout;
    logoText->setSpacing(0);
    logoText->addWidget(logo);
    logoText->addWidget(logoSub);
    logoRow->addWidget(logoBadge);
    logoRow->addLayout(logoText);
    logoRow->addStretch();

    layout->addLayout(logoRow);
    layout->addSpacing(24);

    auto *sectionLabel = new QLabel("监控", sidebar);
    sectionLabel->setObjectName("SidebarSection");
    layout->addWidget(sectionLabel);
    layout->addSpacing(4);

    addNav("仪表盘", NavButton::Dashboard, 0);
    addNav("串口", NavButton::Serial, 1);
    addNav("网络", NavButton::Network, 2);
    addNav("Modbus", NavButton::Modbus, 3);
    addNav("日志", NavButton::Log, 4);

    layout->addSpacing(16);
    auto *sectionLabel2 = new QLabel("系统", sidebar);
    sectionLabel2->setObjectName("SidebarSection");
    layout->addWidget(sectionLabel2);
    layout->addSpacing(4);

    addNav("设置", NavButton::Settings, 5);

    layout->addStretch();

    // footer status
    auto *footer = new QFrame(sidebar);
    footer->setObjectName("SidebarFooter");
    auto *footerLayout = new QHBoxLayout(footer);
    footerLayout->setContentsMargins(12, 10, 12, 10);
    auto *dot = new LedIndicator(footer);
    dot->setColor(QColor("#22c55e"));
    auto *footerText = new QLabel("系统运行正常", footer);
    footerText->setObjectName("FooterText");
    footerLayout->addWidget(dot);
    footerLayout->addWidget(footerText);
    footerLayout->addStretch();
    layout->addWidget(footer);

    return sidebar;
}

void MainWindow::addNav(const QString &text, NavButton::IconKind kind, int pageIndex)
{
    auto *btn = new NavButton(text, kind, this);
    m_navButtons.append(btn);
    connect(btn, &NavButton::clicked, this, [this, pageIndex]() { setPage(pageIndex); });

    if (auto *sidebar = findChild<QFrame *>("Sidebar")) {
        auto *layout = sidebar->layout();
        layout->addWidget(btn);
    }
}

QWidget *MainWindow::buildHeader()
{
    auto *header = new QFrame(this);
    header->setObjectName("HeaderBar");
    header->setFixedHeight(64);

    auto *layout = new QHBoxLayout(header);
    layout->setContentsMargins(24, 0, 24, 0);
    layout->setSpacing(12);

    auto *titleCol = new QVBoxLayout;
    titleCol->setSpacing(0);
    m_pageTitle = new QLabel("仪表盘", header);
    m_pageTitle->setObjectName("PageTitle");
    m_pageSubtitle = new QLabel("系统总览与实时监控", header);
    m_pageSubtitle->setObjectName("PageSubtitle");
    titleCol->addWidget(m_pageTitle);
    titleCol->addWidget(m_pageSubtitle);
    layout->addLayout(titleCol);
    layout->addStretch();

    // 时钟标签在 buildHeader() 中直接创建，解决 nullptr 问题
    m_clockLabel = new QLabel(header);
    m_clockLabel->setObjectName("ClockLabel");
    layout->addWidget(m_clockLabel);

    return header;
}

void MainWindow::setPage(int index)
{
    m_stack->setCurrentIndex(index);

    static const QStringList titles = {
        "仪表盘", "串口通信", "网络通信", "Modbus 协议", "系统日志", "设置"
    };
    static const QStringList subtitles = {
        "系统总览与实时监控",
        "串口参数配置与数据收发",
        "TCP / UDP 服务配置与连接管理",
        "Modbus RTU / TCP 寄存器读写",
        "运行日志与错误记录",
        "应用与通信参数配置",
    };

    m_pageTitle->setText(titles.value(index));
    m_pageSubtitle->setText(subtitles.value(index));

    for (int i = 0; i < m_navButtons.size(); ++i)
        m_navButtons.at(i)->setChecked(i == index);
}