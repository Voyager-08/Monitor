#pragma once

#include <QWidget>

class QComboBox;
class QCheckBox;
class QSpinBox;
class QLineEdit;
class QPushButton;

class SettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsPage(QWidget *parent = nullptr);
};
