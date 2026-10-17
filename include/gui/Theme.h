#pragma once

#include <QObject>
#include <QColor>
#include <QString>

class AppTheme : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool light READ light NOTIFY changed)
    Q_PROPERTY(QColor bg READ bg NOTIFY changed)
    Q_PROPERTY(QColor card READ card NOTIFY changed)
    Q_PROPERTY(QColor cardBorder READ cardBorder NOTIFY changed)
    Q_PROPERTY(QColor text READ text NOTIFY changed)
    Q_PROPERTY(QColor textStrong READ textStrong NOTIFY changed)
    Q_PROPERTY(QColor textMuted READ textMuted NOTIFY changed)
    Q_PROPERTY(QColor textDim READ textDim NOTIFY changed)
    Q_PROPERTY(QColor track READ track NOTIFY changed)
    Q_PROPERTY(QColor successBg READ successBg NOTIFY changed)
    Q_PROPERTY(QColor dangerBg READ dangerBg NOTIFY changed)
    Q_PROPERTY(QColor accent READ accent NOTIFY changed)
    Q_PROPERTY(QColor chartGrid READ chartGrid NOTIFY changed)

public:
    static AppTheme *instance();

    bool light() const;
    QColor bg() const;
    QColor card() const;
    QColor cardBorder() const;
    QColor text() const;
    QColor textStrong() const;
    QColor textMuted() const;
    QColor textDim() const;
    QColor track() const;
    QColor successBg() const;
    QColor dangerBg() const;
    QColor accent() const;
    QColor chartGrid() const;

    void setLight(bool light);

signals:
    void changed();

private:
    explicit AppTheme(QObject *parent = nullptr);

    bool m_light = false;
};

QString currentThemeName();
void applyTheme(const QString &name);
