#pragma once

#include <QWidget>

class LedIndicator;
class QLabel;
class QComboBox;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QTableWidget;
class QPlainTextEdit;

class NetworkPage : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkPage(QWidget *parent = nullptr);

private:
    QComboBox *m_mode = nullptr;
    QLineEdit *m_ip = nullptr;
    QSpinBox *m_port = nullptr;
    QPushButton *m_toggleBtn = nullptr;
    LedIndicator *m_led = nullptr;
    QLabel *m_statusLabel = nullptr;
    QTableWidget *m_clients = nullptr;
    QPlainTextEdit *m_log = nullptr;
    bool m_running = false;
};
