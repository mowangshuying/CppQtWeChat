#include "QSelfLabel.h"

QSelfLabel::QSelfLabel(QWidget* parent /*= nullptr*/, Qt::WindowFlags wndFlags /*= Qt::WindowFlags()*/) : QLabel(parent, wndFlags)
{
    LogFunc;
    setObjectName("QSelfLabel");
}

QSelfLabel::QSelfLabel(const QString& text, QWidget* parent /*= nullptr*/, Qt::WindowFlags wndFlags /*= Qt::WindowFlags()*/) : QLabel(text, parent, wndFlags)
{
    LogFunc;
    setObjectName("QSelfLabel");
}

void QSelfLabel::mousePressEvent(QMouseEvent* ev)
{
    emit clicked();
}
