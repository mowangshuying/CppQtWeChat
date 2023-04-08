#pragma once

#include <qwidget.h>
#include <QPushButton>
#include <QHBoxLayout>

class QScreenShotToolBarWnd : public QWidget
{
    Q_OBJECT
public:
    QScreenShotToolBarWnd(QWidget* p = nullptr);

public:
    QHBoxLayout* m_hLayout;
    QPushButton* m_saveBtn;
    QPushButton* m_copyBtn;
    QPushButton* m_closeBtn;
};
