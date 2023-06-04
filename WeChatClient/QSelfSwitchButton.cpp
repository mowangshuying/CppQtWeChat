#include "QSelfSwitchButton.h"

#include <QPainter>

QSelfSwitchButton::QSelfSwitchButton(QWidget *parent) : QWidget(parent)
{
    LogFunc;
    setObjectName("QSwitchButton");
    m_bSwitch = false;
    m_bgColorOff = QColor(192, 192, 192);  // ¹Ø±Õ°´Å¥Ê±ºò±³¾°Îª»ÒÉ«
    m_bgColorOn = QColor(50, 205, 50);     // ¿ªÆô×´Ì¬Îª°×É«
    m_sliderColorOff = QColor(255, 255, 255);
    m_sliderColorOn = QColor(255, 255, 255);  //

    m_sliderColor = m_sliderColorOff;
    m_textColorOn = QColor(255, 255, 255);
    m_textColorOff = QColor(0, 0, 0);
    m_mouseX = 0;
    m_bPress = false;

    setFixedWidth(40);
    setFixedHeight(18);
}

QSelfSwitchButton::~QSelfSwitchButton()
{
}

void QSelfSwitchButton::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        if ((e->pos().x() - m_centerPoint.x()) * (e->pos().y() - m_centerPoint.y()) <= ((rect().height() / 2) * (rect().height() / 2)))
        {
            m_bPress = true;
            m_mouseX = e->pos().x();
            //  m_sliderColor = QColor(160, 30, 30);
            update();
        }
    }
}

void QSelfSwitchButton::mouseMoveEvent(QMouseEvent *e)
{
    if (m_bPress)
    {
        if ((e->pos().x() >= m_startPoint.x()) && (e->pos().x() <= m_endPoint.x()))
        {
            int tempX = e->pos().x();
            m_centerPoint.setX(tempX - m_mouseX + m_centerPoint.x());
            m_mouseX = m_centerPoint.x();
            update();
        }
    }
}

void QSelfSwitchButton::mouseReleaseEvent(QMouseEvent *e)
{
    m_bPress = false;
    m_sliderColor = QColor(245, 245, 245);
    if (m_centerPoint.x() >= rect().width() / 2)
    {
        m_centerPoint.setX(m_endPoint.x());
        m_sliderColor = m_sliderColorOn;
        if (!m_bSwitch)
            emit stateChange(true);
        m_bSwitch = true;
    }
    else if (m_centerPoint.x() < rect().width() / 2)
    {
        m_centerPoint.setX(m_startPoint.x());
        m_sliderColor = m_sliderColorOff;
        if (m_bSwitch)
            emit stateChange(false);
        m_bSwitch = false;
    }
    update();
}

void QSelfSwitchButton::resizeEvent(QResizeEvent *e)
{
    m_startPoint = QPoint(rect().height() / 2, rect().height() / 2);
    m_centerPoint = m_startPoint;
    m_endPoint = QPoint((rect().right() - rect().height() / 2), rect().height() / 2);
}

void QSelfSwitchButton::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawBg(painter);
    drawSlidBlock(painter);
    drawText(painter);
}

// »æÖÆ±³¾°
void QSelfSwitchButton::drawBg(QPainter &painter)
{
    painter.save();
    painter.setPen(Qt::NoPen);
    if (m_bSwitch)
    {
        painter.setBrush(QBrush(m_bgColorOn));
    }
    else
    {
        painter.setBrush(QBrush(m_bgColorOff));
    }

    // »æÖÆ°´Å¥Íâ±ß¿ò
    QPainterPath path;
    int startX = rect().height() / 2;
    int startY = rect().top();
    path.moveTo(startX, startY);
    path.arcTo(QRect(rect().left(), rect().top(), rect().height(), rect().height()), 90, 180);
    path.lineTo((rect().right() - startX), rect().height());
    path.arcTo(QRect((rect().right() - rect().height()), rect().top(), rect().height(), rect().height()), 270, 180);
    path.lineTo(startX, startY);
    painter.drawPath(path);
    painter.restore();
}

// »æÖÆ»¬¿é
void QSelfSwitchButton::drawSlidBlock(QPainter &painter)
{
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(m_sliderColor));
    painter.drawEllipse(m_centerPoint, 5, 5);
    painter.restore();
}

void QSelfSwitchButton::drawText(QPainter &painter)
{
    painter.save();
    QFont font("Microsoft YaHei", 12, 50, false);
    painter.setFont(font);
    int x, y;
    if (m_bSwitch)
    {
        painter.setPen(QPen(m_textColorOn));
        x = rect().left();
        y = rect().top();
        m_strText = "";
    }
    else
    {
        painter.setPen(QPen(m_textColorOff));
        x = rect().right() - rect().height();
        y = rect().top();
        m_strText = "";
    }
    painter.drawText(x, y, rect().height(), rect().height(), Qt::AlignCenter, m_strText);
    painter.restore();
}