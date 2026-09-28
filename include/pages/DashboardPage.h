#pragma once

#include <QWidget>

class QQuickWidget;

class DashboardPage : public QWidget
{
    Q_OBJECT
public:
    explicit DashboardPage(QWidget *parent = nullptr);

private:
    QQuickWidget *m_quick = nullptr;
};
