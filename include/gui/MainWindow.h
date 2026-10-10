#pragma once

#include <QMainWindow>
#include <QVector>

#include "CommonWidgets.h"

class QStackedWidget;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QWidget *buildSidebar();
    QWidget *buildHeader();
    void addNav(const QString &text, NavButton::IconKind kind, int pageIndex);
    void setPage(int index);
    void loadGeometry();
    void saveGeometry() const;

    QStackedWidget *m_stack = nullptr;
    QLabel *m_pageTitle = nullptr;
    QLabel *m_pageSubtitle = nullptr;
    QLabel *m_clockLabel = nullptr;
    QVector<NavButton *> m_navButtons;
};