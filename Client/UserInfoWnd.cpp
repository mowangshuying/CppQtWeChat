#include "UserInfoWnd.h"
#include "SelfSplit.h"
#include <QApplication>
#include <QPixmap>
#include "DataManager.h"
#include "MainWnd.h"
#include <QGraphicsDropShadowEffect>
#include "StyleSheetMgr.h"

UserInfoWnd::UserInfoWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    LogFunc;
    m_vMainLayout = new QVBoxLayout;
    setLayout(m_vMainLayout);

    m_vMainLayout->setContentsMargins(0, 0, 0, 0);

    m_centerWnd = new QWidget;
    m_vMainLayout->addWidget(m_centerWnd);

    m_centerWnd->setObjectName("QUserInfoWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + m_centerWnd->objectName() + ".qss";
    object.m_widget = m_centerWnd;
    StyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    // 设置为无边框及背景透明
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    setFixedSize(280, 190);

    m_vCenterLayout = new QVBoxLayout;
    m_centerWnd->setLayout(m_vCenterLayout);

    m_vCenterLayout->addSpacing(25);

    m_hTopLayout = new QHBoxLayout;
    m_vInfoLayout = new QVBoxLayout;

    QFont ft;
    ft.setPointSize(15);

    m_usernameLabel = new QLabel;
    m_userIdLabel = new QLabel;

    m_usernameLabel->setObjectName("usernameLabel");
    m_userIdLabel->setObjectName("userIdLabel");

    m_usernameLabel->setFont(ft);
    ft.setPointSize(12);
    m_userIdLabel->setFont(ft);

    m_usernameLabel->setText("xxxxxx");
    m_userIdLabel->setText("用户id:xxxxxx");

    m_vInfoLayout->addWidget(m_usernameLabel);
    m_vInfoLayout->addWidget(m_userIdLabel);
    m_vInfoLayout->addStretch();

    // 头像标签
    m_headLabel = new QLabel;
    m_headLabel->setFixedSize(60, 60);
    m_headLabel->setPixmap(QPixmap("./img/head2.png"));

    // 修改头像按钮
    m_changeHeadImgBtn = new QPushButton;
    m_changeHeadImgBtn->setText("修改头像");
    m_changeHeadImgBtn->setFixedSize(120, 40);

    m_hTopLayout->addWidget(m_headLabel);
    m_hTopLayout->addLayout(m_vInfoLayout);
    m_vCenterLayout->addLayout(m_hTopLayout);

    // 添加sp;
    m_vCenterLayout->addSpacing(10);
    m_vCenterLayout->addWidget(new SelfSplit(nullptr, SelfSplit::Direction_H));

    m_hBtnLayout = new QHBoxLayout;
    m_hBtnLayout->addWidget(m_changeHeadImgBtn, Qt::AlignCenter);

    m_vCenterLayout->addSpacing(15);

    m_vCenterLayout->addLayout(m_hBtnLayout);
    m_vCenterLayout->addStretch();
}

bool UserInfoWnd::event(QEvent* event)
{
    if (event->type() == QEvent::ActivationChange)
    {
        if (QApplication::activeWindow() != this)
        {
            this->hide();
        }
    }

    return QWidget::event(event);
}
