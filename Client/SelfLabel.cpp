#include "SelfLabel.h"

SelfLabel::SelfLabel(QWidget* parent /*= nullptr*/, Qt::WindowFlags wndFlags /*= Qt::WindowFlags()*/) : QLabel(parent, wndFlags)
{
    LogFunc;
    setObjectName("QSelfLabel");
}

SelfLabel::SelfLabel(const QString& text, QWidget* parent /*= nullptr*/, Qt::WindowFlags wndFlags /*= Qt::WindowFlags()*/) : QLabel(text, parent, wndFlags)
{
    LogFunc;
    setObjectName("QSelfLabel");
}

void SelfLabel::mousePressEvent(QMouseEvent* ev)
{
    emit clicked();
}
