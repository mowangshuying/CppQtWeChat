#pragma once

#include <qwidget.h>
#include <QPushButton>
#include <QHBoxLayout>

class ScreenShotToolBarWnd : public QWidget
{
    Q_OBJECT
  public:
    ScreenShotToolBarWnd(QWidget* p = nullptr);

  public:
    QHBoxLayout* m_hLayout;
    QPushButton* m_saveBtn;
    QPushButton* m_copyBtn;
    QPushButton* m_closeBtn;
};
