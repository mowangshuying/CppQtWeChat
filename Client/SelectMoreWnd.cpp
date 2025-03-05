#include "SelectMoreWnd.h"
#include <QApplication>

SelectMoreWnd::SelectMoreWnd(QWidget* p) : QWidget(p)
{
    LogFunc;
    setObjectName("QSelectMoreWnd");
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground);
    setFixedSize(135, 40);

    m_vLayout = new QVBoxLayout();
    setLayout(m_vLayout);
    setStyleSheet("background-color:#2e2e2e;");

    m_settingBtn = new QPushButton("设置");
    m_vLayout->addWidget(m_settingBtn);
    m_settingBtn->setStyleSheet("background-color:#2e2e2e;color:white;border:0px;");
}

bool SelectMoreWnd::event(QEvent* event)
{
    if (event->type() == QEvent::ActivationChange && QApplication::activeWindow() != this)
    {
        this->hide();
    }
    return QWidget::event(event);
}