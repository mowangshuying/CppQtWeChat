#include "QSimpleLabel.h"

QSimpleLabel::QSimpleLabel(QWidget* parent /*= nullptr*/, Qt::WindowFlags f /*= Qt::WindowFlags()*/) : QLabel(parent, f)
{
    setObjectName("QSimpleLabel");
}

QSimpleLabel::QSimpleLabel(const QString& text, QWidget* parent /*= nullptr*/, Qt::WindowFlags f /*= Qt::WindowFlags()*/) : QLabel(text, parent, f)
{
    setObjectName("QSimpleLabel");
}

void QSimpleLabel::mousePressEvent(QMouseEvent* ev)
{
    emit clicked();
}
