#pragma once

#include <QFrame>
#include <QLabel>
#include <QWidget>
#include <QVector>
#include <QColor>

class QVBoxLayout;
class QPainter;
class QPaintEvent;

class Card : public QFrame
{
    Q_OBJECT
public:
    explicit Card(QWidget *parent = nullptr);

    void setTitle(const QString &title);
    void setSubtitle(const QString &subtitle);
    QVBoxLayout *bodyLayout() const;

private:
    QLabel *m_title = nullptr;
    QLabel *m_subtitle = nullptr;
    QVBoxLayout *m_body = nullptr;
};

class StatCard : public QFrame
{
    Q_OBJECT
public:
    explicit StatCard(const QString &title, const QString &value,
                      const QString &unit, const QColor &accent,
                      QWidget *parent = nullptr);

    void setValue(const QString &value);
    void setTrend(const QString &text, bool up);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *m_title = nullptr;
    QLabel *m_value = nullptr;
    QLabel *m_unit = nullptr;
    QLabel *m_trend = nullptr;
    QColor m_accent;
};

class LedIndicator : public QWidget
{
    Q_OBJECT
public:
    explicit LedIndicator(QWidget *parent = nullptr);

    void setColor(const QColor &color);
    QColor color() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor m_color;
};

class LineChart : public QWidget
{
    Q_OBJECT
public:
    explicit LineChart(QWidget *parent = nullptr);

    void setTitle(const QString &title);
    void setColor(const QColor &color);
    void pushValue(double value);
    void clear();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<double> m_data;
    QColor m_color;
    QString m_title;
    int m_maxPoints = 140;
};

class NavButton : public QWidget
{
    Q_OBJECT
public:
    enum IconKind {
        Dashboard,
        Serial,
        Network,
        Modbus,
        Log,
        Settings
    };

    explicit NavButton(const QString &text, IconKind kind, QWidget *parent = nullptr);

    void setChecked(bool checked);
    bool isChecked() const;

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void paintIcon(QPainter &p, const QRectF &r, const QColor &color);

    QString m_text;
    IconKind m_kind;
    bool m_checked = false;
    bool m_hovered = false;
    bool m_pressed = false;
};
