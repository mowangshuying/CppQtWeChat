#include "QSelectMoreWnd.h"
#include <QApplication>

QSelectMoreWnd::QSelectMoreWnd(QWidget* p) : QWidget(p)
{
    LogFunc;
    setObjectName("QSelectMoreWnd");
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground);
    setFixedSize(135, 40);

    m_vLayout = new QVBoxLayout();
    setLayout(m_vLayout);
    setStyleSheet("background-color:#2e2e2e;");

    m_settingBtn = new QPushButton("ÉèÖÃ");
    m_vLayout->addWidget(m_settingBtn);
    m_settingBtn->setStyleSheet(
        "background-color:#2e2e2e;color:white;border:0px;");
}

bool QSelectMoreWnd::event(QEvent* event)
{
    if (event->type() == QEvent::ActivationChange &&
        QApplication::activeWindow() != this)
    {
        this->hide();
    }
    return QWidget::event(event);
}