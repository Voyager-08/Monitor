#pragma once

#include <QMainWindow>
#include <QVector>

#include "gui/common/CommonWidgets.h"

class QStackedWidget;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QWidget *buildSidebar();
    QWidget *buildHeader();
    void addNav(const QString &text, NavButton::IconKind kind, int pageIndex);
    void setPage(int index);

    QStackedWidget *m_stack = nullptr;
    QLabel *m_pageTitle = nullptr;
    QLabel *m_pageSubtitle = nullptr;
    QLabel *m_clockLabel = nullptr;
    QVector<NavButton *> m_navButtons;
};
