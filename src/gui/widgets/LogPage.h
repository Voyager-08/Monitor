#pragma once

#include <QWidget>

class QComboBox;
class QLineEdit;
class QPushButton;
class QTableWidget;

class LogPage : public QWidget
{
    Q_OBJECT
public:
    explicit LogPage(QWidget *parent = nullptr);

private:
    void applyFilter();
    void appendRow(const QString &time, const QString &level,
                   const QString &source, const QString &msg);

    QComboBox *m_level = nullptr;
    QLineEdit *m_keyword = nullptr;
    QTableWidget *m_table = nullptr;
};
