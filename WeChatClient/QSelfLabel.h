#pragma once

#include "QPch.h"

#include <QLabel>

class QSelfLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QSelfLabel(QWidget* parent = nullptr, Qt::WindowFlags wndFlags = Qt::WindowFlags());
    explicit QSelfLabel(const QString& text, QWidget* parent = nullptr, Qt::WindowFlags wndFlags = Qt::WindowFlags());
signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* ev);
};
