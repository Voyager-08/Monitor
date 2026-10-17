#pragma once

#include <QWidget>
#include <QString>
#include <QColor>
#include <QMap>

class LedIndicator;
class QLabel;
class QComboBox;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QPlainTextEdit;

class NetworkPage : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkPage(QWidget *parent = nullptr);

private:
    struct ModeState
    {
        bool running = false;
        QString status = QStringLiteral("未连接");
        QColor led = QColor("#8b90a0");
    };

    QString idleText() const;
    QString runningText() const;
    void saveState();
    void applyState();

    QComboBox *m_mode = nullptr;
    QLineEdit *m_ip = nullptr;
    QSpinBox *m_port = nullptr;
    QPushButton *m_toggleBtn = nullptr;
    LedIndicator *m_led = nullptr;
    QLabel *m_statusLabel = nullptr;
    QPlainTextEdit *m_log = nullptr;
    bool m_running = false;
    QString m_currentMode;
    QMap<QString, ModeState> m_states;
};
