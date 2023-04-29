#pragma once
#include "QPch.h"
#include <QWidget>

class QSettingWnd : public QWidget
{
public:
    QSettingWnd(QWidget* p = nullptr);

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

public:
    bool m_leftBtnPressed = false;
    QPoint m_LeftBtnPressPoint;
};
