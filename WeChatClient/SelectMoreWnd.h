#pragma once

#include "QPch.h"
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QEvent>

class SelectMoreWnd : public QWidget
{
    Q_OBJECT
  protected:
    bool event(QEvent* event);

  public:
    SelectMoreWnd(QWidget* p = nullptr);

  public:
    QVBoxLayout* m_vLayout;
    //¹ØÓÚ°´Å¥
    QPushButton* m_settingBtn;
};
