#include "QSimpleLabel.h"

QSimpleLabel::QSimpleLabel(QWidget* parent /*= nullptr*/, Qt::WindowFlags f /*= Qt::WindowFlags()*/) : QLabel(parent, f)
{
}

QSimpleLabel::QSimpleLabel(const QString& text, QWidget* parent /*= nullptr*/, Qt::WindowFlags f /*= Qt::WindowFlags()*/) : QLabel(text, parent, f)
{
}

void QSimpleLabel::mousePressEvent(QMouseEvent* ev)
{
    emit clicked();
}
