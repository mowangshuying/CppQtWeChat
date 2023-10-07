#pragma once

#include "QPch.h"
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QEvent>

class QSelectMoreWnd : public QWidget
{
    Q_OBJECT
  protected:
    bool event(QEvent* event);

  public:
    QSelectMoreWnd(QWidget* p = nullptr);

  public:
    QVBoxLayout* m_vLayout;
    //¹ØÓÚ°´Å¥
    QPushButton* m_settingBtn;
};
