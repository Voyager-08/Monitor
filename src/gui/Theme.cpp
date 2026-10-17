#include "gui/Theme.h"

#include <QApplication>
#include <QFile>
#include <QSettings>

namespace {
constexpr auto kDarkName = "深色科技风";
constexpr auto kLightName = "浅色简洁风";
}

AppTheme::AppTheme(QObject *parent)
    : QObject(parent)
{
}

AppTheme *AppTheme::instance()
{
    static AppTheme theme;
    return &theme;
}

bool AppTheme::light() const
{
    return m_light;
}

QColor AppTheme::bg() const
{
    return m_light ? QColor("#f5f6f8") : QColor("#1b1c24");
}

QColor AppTheme::card() const
{
    return m_light ? QColor("#ffffff") : QColor("#20212b");
}

QColor AppTheme::cardBorder() const
{
    return m_light ? QColor("#e3e6ec") : QColor("#2a2c37");
}

QColor AppTheme::text() const
{
    return m_light ? QColor("#2c313a") : QColor("#e6e8ee");
}

QColor AppTheme::textStrong() const
{
    return m_light ? QColor("#1b1e24") : QColor("#f1f2f6");
}

QColor AppTheme::textMuted() const
{
    return m_light ? QColor("#6b7280") : QColor("#8b90a0");
}

QColor AppTheme::textDim() const
{
    return m_light ? QColor("#9097a3") : QColor("#6b7280");
}

QColor AppTheme::track() const
{
    return m_light ? QColor("#d6dae2") : QColor("#3a3d49");
}

QColor AppTheme::successBg() const
{
    return m_light ? QColor("#e6f6ec") : QColor("#14351f");
}

QColor AppTheme::dangerBg() const
{
    return m_light ? QColor("#fdeaea") : QColor("#3a1c1c");
}

QColor AppTheme::accent() const
{
    return QColor("#3b82f6");
}

QColor AppTheme::chartGrid() const
{
    return m_light ? QColor("#e8ebf0") : QColor("#2e303c");
}

void AppTheme::setLight(bool light)
{
    if (m_light == light) {
        return;
    }
    m_light = light;
    emit changed();
}

QString currentThemeName()
{
    QSettings settings;
    const QString name = settings.value("appearance/theme", kDarkName).toString();
    return name == kLightName ? kLightName : kDarkName;
}

void applyTheme(const QString &name)
{
    const bool light = (name == kLightName);
    AppTheme::instance()->setLight(light);

    QFile file(light ? ":/qss/style_light.qss" : ":/qss/style.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qApp->setStyleSheet(QString::fromUtf8(file.readAll()));
    }

    QSettings settings;
    settings.setValue("appearance/theme", light ? kLightName : kDarkName);
}
