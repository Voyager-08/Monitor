#include "pages/DashboardPage.h"

#include "models/DashboardBackend.h"

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

    m_quick = new QQuickWidget(this);
    m_quick->setResizeMode(QQuickWidget::SizeRootObjectToView);
    m_quick->setClearColor(QColor("#1b1c24"));
    m_quick->rootContext()->setContextProperty("dashboard", backend);
    m_quick->setSource(QUrl(QStringLiteral("qrc:/qml/DashboardPage.qml")));

    if (m_quick->status() == QQuickWidget::Error) {
        for (const QQmlError &error : m_quick->errors())
            qWarning() << "[Dashboard QML]" << error.toString();
    }

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_quick);
}
