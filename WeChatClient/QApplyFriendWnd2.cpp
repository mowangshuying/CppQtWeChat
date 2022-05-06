#include "QApplyFriendWnd2.h"

QApplyFriendWnd2::QApplyFriendWnd2(QWidget* p /*= nullptr*/)
	: QWidget(p)
{
	setContentsMargins(0, 0, 0, 0);
	setFixedSize(460, 300);

	m_hLayout = new QHBoxLayout();
	setLayout(m_hLayout);

	m_userInfoWnd = new QWidget();
	m_vUserInfoLayout = new QVBoxLayout();

	m_headUrl = new QLabel();
	m_roleName = new QLabel();
	m_moreInfo = new QLabel();


	m_headUrl->setFixedSize(80, 80);
	QPixmap pixmap1 = QPixmap("./img/head2.png");
	QPixmap pixmap2 = pixmap1.scaled(80, 80, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	m_headUrl->setPixmap(pixmap2);

	m_roleName->setText("玩家的编号");
	m_moreInfo->setText("更多的信息...");


	m_vUserInfoLayout->addWidget(m_headUrl);
	m_vUserInfoLayout->addSpacing(20);
	m_vUserInfoLayout->addWidget(m_roleName);
	m_vUserInfoLayout->addSpacing(20);
	m_vUserInfoLayout->addWidget(m_moreInfo);
	m_vUserInfoLayout->addStretch();

	m_userInfoWnd->setLayout(m_vUserInfoLayout);

	m_userInfoWnd->setFixedWidth(130);
	m_userInfoWnd->setWindowFlags(Qt::FramelessWindowHint);
	m_userInfoWnd->setStyleSheet("background-color:#aaa;border:none");

	m_vLayout = new QVBoxLayout();
	m_inMsgLabel = new QLabel();

	m_inMsgLabel->setText("你的好友添加请求已经发送成功，正在等待对方确认");


	m_vLayout->addWidget(m_inMsgLabel);
	m_vLayout->addStretch();

	m_hLayout->addLayout(m_vLayout);
}

