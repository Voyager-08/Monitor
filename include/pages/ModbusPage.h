#pragma once

#include <QWidget>

class QComboBox;
class QSpinBox;
class QCheckBox;
class QPushButton;
class QTableWidget;
class QTimer;
class QLabel;

class ModbusPage : public QWidget
{
    Q_OBJECT
public:
    explicit ModbusPage(QWidget *parent = nullptr);

private:
    void pollOnce();

    QComboBox *m_transport = nullptr;
    QSpinBox *m_slave = nullptr;
    QComboBox *m_func = nullptr;
    QSpinBox *m_start = nullptr;
    QSpinBox *m_count = nullptr;
    QCheckBox *m_autoPoll = nullptr;
    QPushButton *m_readBtn = nullptr;
    QTableWidget *m_regs = nullptr;
    QLabel *m_statusLabel = nullptr;
    QTimer *m_timer = nullptr;
};
