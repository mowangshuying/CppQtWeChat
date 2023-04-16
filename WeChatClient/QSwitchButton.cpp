#include "QSwitchButton.h"

#include <QPainter>

QSwitchButton::QSwitchButton(QWidget *parent) : QWidget(parent)
{
    setObjectName("QSwitchButton");
    bSwitch = false;
    bgColorOff = QColor(192, 192, 192);
    bgColorOn = QColor(50, 205, 50);
    sliderColorOff = QColor(255, 255, 255);
    sliderColorOn = QColor(255, 255, 255);  //

    sliderColor = sliderColorOff;
    textColorOn = QColor(255, 255, 255);
    textColorOff = QColor(0, 0, 0);
    mouseX = 0;
    bPress = false;

    setFixedWidth(40);
}

QSwitchButton::~QSwitchButton()
{
}

void QSwitchButton::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        if ((e->pos().x() - centerPoint.x()) * (e->pos().y() - centerPoint.y()) <= ((rect().height() / 2) * (rect().height() / 2)))
        {
            bPress = true;
            mouseX = e->pos().x();
            sliderColor = QColor(160, 30, 30);
            update();
        }
    }
}

void QSwitchButton::mouseMoveEvent(QMouseEvent *e)
{
    if (bPress)
    {
        if ((e->pos().x() >= startPoint.x()) && (e->pos().x() <= endPoint.x()))
        {
            int tempX = e->pos().x();
            centerPoint.setX(tempX - mouseX + centerPoint.x());
            mouseX = centerPoint.x();
            update();
        }
    }
}

void QSwitchButton::mouseReleaseEvent(QMouseEvent *e)
{
    bPress = false;
    sliderColor = QColor(245, 245, 245);
    if (centerPoint.x() >= rect().width() / 2)
    {
        centerPoint.setX(endPoint.x());
        sliderColor = sliderColorOn;
        if (!bSwitch)
            emit stateChange(true);
        bSwitch = true;
    }
    else if (centerPoint.x() < rect().width() / 2)
    {
        centerPoint.setX(startPoint.x());
        sliderColor = sliderColorOff;
        if (bSwitch)
            emit stateChange(false);
        bSwitch = false;
    }
    update();
}

void QSwitchButton::resizeEvent(QResizeEvent *e)
{
    startPoint = QPoint(rect().height() / 2, rect().height() / 2);
    centerPoint = startPoint;
    endPoint = QPoint((rect().right() - rect().height() / 2), rect().height() / 2);
}

void QSwitchButton::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawBg(painter);
    drawSlidBlock(painter);
    drawText(painter);
}

// 绘制背景
void QSwitchButton::drawBg(QPainter &painter)
{
    painter.save();
    painter.setPen(Qt::NoPen);
    if (bSwitch)
    {
        painter.setBrush(QBrush(bgColorOn));
    }
    else
    {
        painter.setBrush(QBrush(bgColorOff));
    }

    // 绘制按钮外边框
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

// 绘制滑块
void QSwitchButton::drawSlidBlock(QPainter &painter)
{
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(sliderColor));
    painter.drawEllipse(centerPoint, rect().height() / 2 - 2, rect().height() / 2 - 2);
    painter.restore();
}

void QSwitchButton::drawText(QPainter &painter)
{
    painter.save();
    QFont font("Microsoft YaHei", 12, 50, false);
    painter.setFont(font);
    int x, y;
    if (bSwitch)
    {
        painter.setPen(QPen(textColorOn));
        x = rect().left();
        y = rect().top();
        strText = "";  // QString("开");
    }
    else
    {
        painter.setPen(QPen(textColorOff));
        x = rect().right() - rect().height();
        y = rect().top();
        strText = "";  // QString("关");
    }
    painter.drawText(x, y, rect().height(), rect().height(), Qt::AlignCenter, strText);
    painter.restore();
}