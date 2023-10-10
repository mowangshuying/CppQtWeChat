#pragma once

#include "QPch.h"

#include <QLabel>

// 自定义Label控件，Label被点击时候发出信号
class QSelfLabel : public QLabel
{
    Q_OBJECT
    // 构造函数与析构函数
  public:
    explicit QSelfLabel(QWidget* parent = nullptr,
                        Qt::WindowFlags wndFlags = Qt::WindowFlags());
    explicit QSelfLabel(const QString& text,
                        QWidget* parent = nullptr,
                        Qt::WindowFlags wndFlags = Qt::WindowFlags());
  signals:
    void clicked();

  protected:
    void mousePressEvent(QMouseEvent* ev) override;
};
