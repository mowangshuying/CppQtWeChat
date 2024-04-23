#include "QPch.h"
#include "ScreenShotToolBarWnd.h"

ScreenShotToolBarWnd::ScreenShotToolBarWnd(QWidget* p /*= nullptr*/)
{
    LogFunc;
    setObjectName("QScreenShotToolBarWnd");

    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("background-color:rgb(234,238,245);");
    m_hLayout = new QHBoxLayout();
    m_hLayout->setContentsMargins(7, 7, 7, 7);
    setLayout(m_hLayout);
    setFixedHeight(30);

    m_saveBtn = new QPushButton();
    m_saveBtn->setFixedSize(16, 16);
    m_saveBtn->setIconSize(QSize(16, 16));
    m_saveBtn->setIcon(QPixmap("./img/saveShot.png"));

    m_closeBtn = new QPushButton();
    m_closeBtn->setFixedSize(16, 16);
    m_closeBtn->setIconSize(QSize(16, 16));
    m_closeBtn->setIcon(QPixmap("./img/closeShot.png"));

    m_copyBtn = new QPushButton();
    m_copyBtn->setFixedSize(16, 16);
    m_copyBtn->setIconSize(QSize(16, 16));
    m_copyBtn->setIcon(QPixmap("./img/copyShot.png"));

    m_hLayout->addWidget(m_saveBtn);
    m_hLayout->addWidget(m_closeBtn);
    m_hLayout->addWidget(m_copyBtn);
}
