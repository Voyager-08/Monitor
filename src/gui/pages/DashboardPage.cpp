#include "gui/pages/DashboardPage.h"

#include "gui/viewmodels/DashboardBackend.h"
#include "gui/Theme.h"

#include <QQuickWidget>
#include <QQmlContext>
#include <QQmlError>
#include <QVBoxLayout>
#include <QUrl>
#include <QDebug>

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent)
{
    auto *backend = new DashboardBackend(this);
    AppTheme *theme = AppTheme::instance();

    m_quick = new QQuickWidget(this);
    m_quick->setResizeMode(QQuickWidget::SizeRootObjectToView);
    m_quick->setClearColor(theme->bg());
    m_quick->rootContext()->setContextProperty("dashboard", backend);
    m_quick->rootContext()->setContextProperty("appTheme", theme);
    m_quick->setSource(QUrl(QStringLiteral("qrc:/qml/DashboardPage.qml")));

    connect(theme, &AppTheme::changed, this, [this]() {
        m_quick->setClearColor(AppTheme::instance()->bg());
    });

    if (m_quick->status() == QQuickWidget::Error) {
        for (const QQmlError &error : m_quick->errors())
            qWarning() << "[Dashboard QML]" << error.toString();
    }

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_quick);
}