#include "QDataManager.h"
#include "QChatFileOuterWnd.h"

QChatFileOuterWnd::QChatFileOuterWnd(QWidget* p /*= nullptr*/, int64_t sendid /* = -1*/, int64_t recvid /* = -1*/) : QWidget(p)
{
    setObjectName("QChatFileOuterWnd");

    m_hLayout = new QHBoxLayout();
    m_vLayout = new QVBoxLayout();
    setLayout(m_hLayout);
    setMinimumHeight(120);

    m_headUrl = new QLabel();
    m_headUrl->setFixedSize(30, 30);
    m_vLayout->addWidget(m_headUrl);
    m_vLayout->addStretch();

    m_innerWnd = new QChatFileInnerWnd();

    if (sendid != QDataManager::getMgr()->m_userid)
    {
        m_hLayout->setSpacing(0);
        m_hLayout->addSpacing(10);
        //设置玩家的头像
        m_hLayout->addLayout(m_vLayout);
        m_hLayout->addSpacing(5);
        m_hLayout->addWidget(m_innerWnd);
        m_hLayout->addStretch();
        QPixmap pixmap = QDataManager::getMgr()->m_UserId2HeadImgMap[sendid];
        pixmap = pixmap.scaled(30, 30);
        m_headUrl->setPixmap(pixmap);
    }
    else
    {
        // m_hLayout->setSpacing(0);
        m_hLayout->addStretch();
        m_hLayout->addWidget(m_innerWnd);
        m_hLayout->addSpacing(5);
        m_hLayout->addLayout(m_vLayout);
        m_hLayout->addSpacing(10);

        QPixmap pixmap = QDataManager::getMgr()->m_UserId2HeadImgMap[sendid];
        pixmap = pixmap.scaled(30, 30);
        m_headUrl->setPixmap(pixmap);
    }
}
