#include "gui/common/CommonWidgets.h"

#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QLinearGradient>
#include <QStyle>

// ---------------------------------------------------------------------------
// Card
// ---------------------------------------------------------------------------
Card::Card(QWidget *parent)
    : QFrame(parent)
{
    setObjectName("Card");

    m_title = new QLabel(this);
    m_title->setObjectName("CardTitle");
    m_subtitle = new QLabel(this);
    m_subtitle->setObjectName("CardSubtitle");

    auto *head = new QVBoxLayout;
    head->setContentsMargins(0, 0, 0, 0);
    head->setSpacing(2);
    head->addWidget(m_title);
    head->addWidget(m_subtitle);
    m_subtitle->hide();

    m_body = new QVBoxLayout;
    m_body->setContentsMargins(0, 0, 0, 0);
    m_body->setSpacing(8);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 14, 16, 14);
    layout->setSpacing(10);
    layout->addLayout(head);
    layout->addLayout(m_body);
}

void Card::setTitle(const QString &title)
{
    m_title->setText(title);
}

void Card::setSubtitle(const QString &subtitle)
{
    m_subtitle->setText(subtitle);
    m_subtitle->setVisible(!subtitle.isEmpty());
}

QVBoxLayout *Card::bodyLayout() const
{
    return m_body;
}

// ---------------------------------------------------------------------------
// StatCard
// ---------------------------------------------------------------------------
StatCard::StatCard(const QString &title, const QString &value,
                   const QString &unit, const QColor &accent,
                   QWidget *parent)
    : QFrame(parent)
    , m_accent(accent)
{
    setObjectName("StatCard");
    setMinimumHeight(104);

    m_title = new QLabel(title, this);
    m_title->setObjectName("StatTitle");

    m_value = new QLabel(value, this);
    m_value->setObjectName("StatValue");

    m_unit = new QLabel(unit, this);
    m_unit->setObjectName("StatUnit");

    m_trend = new QLabel(this);
    m_trend->setObjectName("StatTrend");
    m_trend->hide();

    auto *valueRow = new QHBoxLayout;
    valueRow->setSpacing(6);
    valueRow->addWidget(m_value);
    valueRow->addWidget(m_unit);
    valueRow->addStretch();

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(18, 14, 18, 14);
    layout->setSpacing(4);
    layout->addWidget(m_title);
    layout->addLayout(valueRow);
    layout->addWidget(m_trend);
}

void StatCard::setValue(const QString &value)
{
    m_value->setText(value);
}

void StatCard::setTrend(const QString &text, bool up)
{
    if (text.isEmpty()) {
        m_trend->hide();
        return;
    }
    m_trend->setText(text);
    m_trend->setProperty("up", up);
    m_trend->style()->unpolish(m_trend);
    m_trend->style()->polish(m_trend);
    m_trend->show();
}

void StatCard::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const int barWidth = 4;
    const int barHeight = 38;
    const int y = (height() - barHeight) / 2;

    QPainterPath path;
    path.addRoundedRect(QRectF(0, y, barWidth, barHeight), barWidth / 2.0, barWidth / 2.0);
    p.fillPath(path, m_accent);

    const int r = 5;
    const int cx = width() - 24;
    const int cy = 22;
    p.setPen(Qt::NoPen);
    p.setBrush(m_accent.lighter(140));
    p.drawEllipse(QPoint(cx, cy), r, r);
    p.setBrush(m_accent);
    p.drawEllipse(QPointF(cx, cy), r / 2.0, r / 2.0);
}

// ---------------------------------------------------------------------------
// LedIndicator
// ---------------------------------------------------------------------------
LedIndicator::LedIndicator(QWidget *parent)
    : QWidget(parent)
    , m_color(Qt::gray)
{
    setFixedSize(10, 10);
}

void LedIndicator::setColor(const QColor &color)
{
    if (m_color == color)
        return;
    m_color = color;
    update();
}

QColor LedIndicator::color() const
{
    return m_color;
}

void LedIndicator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    QColor glow = m_color;
    glow.setAlpha(70);
    p.setPen(Qt::NoPen);
    p.setBrush(glow);
    p.drawEllipse(rect().adjusted(-2, -2, 2, 2));

    p.setBrush(m_color);
    p.drawEllipse(rect());
}

// ---------------------------------------------------------------------------
// LineChart
// ---------------------------------------------------------------------------
LineChart::LineChart(QWidget *parent)
    : QWidget(parent)
    , m_color(QColor("#3b82f6"))
{
    setMinimumHeight(220);
    for (int i = 0; i < 40; ++i)
        m_data.append(50.0);
}

void LineChart::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void LineChart::setColor(const QColor &color)
{
    m_color = color;
    update();
}

void LineChart::pushValue(double value)
{
    m_data.append(value);
    while (m_data.size() > m_maxPoints)
        m_data.removeFirst();
    update();
}

void LineChart::clear()
{
    m_data.clear();
    update();
}

void LineChart::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const QRectF area = rect();
    const QRectF plot = area.adjusted(10, 30, 10, 14);

    p.setPen(QColor("#8b90a0"));
    QFont f = font();
    f.setPointSize(9);
    p.setFont(f);
    p.drawText(QRectF(plot.left(), 6, plot.width(), 18),
               Qt::AlignLeft | Qt::AlignVCenter, m_title);

    // grid
    p.setPen(QPen(QColor("#2e303c"), 1));
    const int rows = 5;
    for (int i = 0; i <= rows; ++i) {
        const qreal y = plot.top() + plot.height() * i / rows;
        p.drawLine(QPointF(plot.left(), y), QPointF(plot.right(), y));
    }

    if (m_data.size() < 2)
        return;

    const double min = 0.0;
    const double max = 100.0;
    auto mapX = [&](int i) {
        return plot.left() + plot.width() * i / (m_maxPoints - 1.0);
    };
    auto mapY = [&](double v) {
        return plot.bottom() - (v - min) / (max - min) * plot.height();
    };

    QPolygonF line;
    QPolygonF fill;
    for (int i = 0; i < m_data.size(); ++i) {
        const qreal x = mapX(m_maxPoints - m_data.size() + i);
        const qreal y = mapY(m_data.at(i));
        line << QPointF(x, y);
        fill << QPointF(x, y);
    }

    QLinearGradient grad(plot.left(), plot.top(), plot.left(), plot.bottom());
    QColor c1 = m_color;
    c1.setAlpha(90);
    QColor c2 = m_color;
    c2.setAlpha(0);
    grad.setColorAt(0, c1);
    grad.setColorAt(1, c2);

    fill << QPointF(mapX(m_maxPoints - 1), plot.bottom());
    fill << QPointF(mapX(0), plot.bottom());
    p.setPen(Qt::NoPen);
    p.setBrush(grad);
    p.drawPolygon(fill);

    p.setPen(QPen(m_color, 2));
    p.drawPolyline(line);
}

// ---------------------------------------------------------------------------
// NavButton
// ---------------------------------------------------------------------------
NavButton::NavButton(const QString &text, IconKind kind, QWidget *parent)
    : QWidget(parent)
    , m_text(text)
    , m_kind(kind)
{
    setFixedHeight(42);
    setCursor(Qt::PointingHandCursor);
}

void NavButton::setChecked(bool checked)
{
    if (m_checked == checked)
        return;
    m_checked = checked;
    update();
}

bool NavButton::isChecked() const
{
    return m_checked;
}

void NavButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const QColor textColor = m_checked ? QColor("#3b82f6") : QColor("#9aa0b0");
    const QColor bg = m_hovered ? QColor("#23242e") : QColor(Qt::transparent);

    if (m_hovered) {
        p.setPen(Qt::NoPen);
        p.setBrush(bg);
        p.drawRoundedRect(rect(), 8, 8);
    }

    // active indicator bar
    if (m_checked) {
        p.setPen(Qt::NoPen);
        p.setBrush(QColor("#3b82f6"));
        p.drawRoundedRect(QRectF(0, 10, 3, height() - 20), 1.5, 1.5);
    }

    // icon
    const qreal iconSize = 18;
    const QRectF iconRect(16, (height() - iconSize) / 2.0, iconSize, iconSize);
    paintIcon(p, iconRect, textColor);

    // text
    QFont f = font();
    f.setPointSize(9);
    f.setWeight(m_checked ? QFont::DemiBold : QFont::Normal);
    p.setFont(f);
    p.setPen(textColor);
    p.drawText(QRectF(44, 0, width() - 44, height()),
               Qt::AlignLeft | Qt::AlignVCenter, m_text);
}

void NavButton::paintIcon(QPainter &p, const QRectF &r, const QColor &color)
{
    p.save();
    p.setRenderHint(QPainter::Antialiasing, true);
    QPen pen(color, 1.6);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    p.setPen(pen);
    p.setBrush(Qt::NoBrush);

    const qreal x = r.x(), y = r.y(), w = r.width(), h = r.height();

    switch (m_kind) {
    case Dashboard: {
        const qreal s = w * 0.42;
        p.drawRoundedRect(QRectF(x, y, s, s), 2.5, 2.5);
        p.drawRoundedRect(QRectF(x + w * 0.58, y, s, s), 2.5, 2.5);
        p.drawRoundedRect(QRectF(x, y + h * 0.58, s, s), 2.5, 2.5);
        p.drawRoundedRect(QRectF(x + w * 0.58, y + h * 0.58, s, s), 2.5, 2.5);
        break;
    }
    case Serial: {
        p.drawRoundedRect(QRectF(x + w * 0.2, y, w * 0.6, h * 0.55), 2, 2);
        p.drawLine(QPointF(x + w * 0.3, y + h * 0.55), QPointF(x + w * 0.3, y + h));
        p.drawLine(QPointF(x + w * 0.7, y + h * 0.55), QPointF(x + w * 0.7, y + h));
        break;
    }
    case Network: {
        p.drawEllipse(QRectF(x + w * 0.08, y + h * 0.08, w * 0.84, h * 0.84));
        const QPointF c(x + w * 0.5, y + h * 0.5);
        p.drawLine(c, QPointF(x + w * 0.5, y + h * 0.08));
        p.drawLine(c, QPointF(x + w * 0.08, y + h * 0.5));
        p.drawLine(c, QPointF(x + w * 0.92, y + h * 0.5));
        break;
    }
    case Modbus: {
        p.drawRoundedRect(QRectF(x + w * 0.25, y + h * 0.25, w * 0.5, h * 0.5), 2, 2);
        for (int i = 0; i < 4; ++i) {
            const qreal py = y + h * (0.32 + i * 0.12);
            p.drawLine(QPointF(x, py), QPointF(x + w * 0.25, py));
            p.drawLine(QPointF(x + w * 0.75, py), QPointF(x + w, py));
        }
        break;
    }
    case Log: {
        const qreal cy[3] = {y + h * 0.2, y + h * 0.5, y + h * 0.8};
        for (int i = 0; i < 3; ++i) {
            p.drawEllipse(QPointF(x + w * 0.15, cy[i]), 1.1, 1.1);
            p.drawLine(QPointF(x + w * 0.32, cy[i]), QPointF(x + w, cy[i]));
        }
        break;
    }
    case Settings: {
        for (int i = 0; i < 3; ++i) {
            const qreal ly = y + h * (0.2 + i * 0.3);
            p.drawLine(QPointF(x, ly), QPointF(x + w, ly));
            const qreal cx = i == 0 ? x + w * 0.35 : (i == 1 ? x + w * 0.65 : x + w * 0.5);
            p.drawEllipse(QPointF(cx, ly), 2.2, 2.2);
        }
        break;
    }
    }

    p.restore();
}

void NavButton::enterEvent(QEnterEvent *event)
{
    m_hovered = true;
    update();
    QWidget::enterEvent(event);
}

void NavButton::leaveEvent(QEvent *event)
{
    m_hovered = false;
    update();
    QWidget::leaveEvent(event);
}

void NavButton::mousePressEvent(QMouseEvent *event)
{
    m_pressed = true;
    update();
    QWidget::mousePressEvent(event);
}

void NavButton::mouseReleaseEvent(QMouseEvent *event)
{
    m_pressed = false;
    update();
    if (rect().contains(event->pos()))
        emit clicked();
    QWidget::mouseReleaseEvent(event);
}
