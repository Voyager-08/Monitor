#pragma once

#include <QWidget>

class LedIndicator;
class QLabel;
class QComboBox;
class QPushButton;
class QPlainTextEdit;
class QCheckBox;

class SerialPage : public QWidget
{
    Q_OBJECT
public:
    explicit SerialPage(QWidget *parent = nullptr);

private:
    void appendLog(const QString &text);

    QComboBox *m_port = nullptr;
    QComboBox *m_baud = nullptr;
    QComboBox *m_dataBits = nullptr;
    QComboBox *m_stopBits = nullptr;
    QComboBox *m_parity = nullptr;
    QPushButton *m_openBtn = nullptr;
    LedIndicator *m_led = nullptr;
    QLabel *m_statusLabel = nullptr;
    QPlainTextEdit *m_recv = nullptr;
    QPlainTextEdit *m_send = nullptr;
    QCheckBox *m_hexRecv = nullptr;
    QCheckBox *m_hexSend = nullptr;
    bool m_opened = false;
};
