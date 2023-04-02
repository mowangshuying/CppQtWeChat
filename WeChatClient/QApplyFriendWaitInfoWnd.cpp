#include "QApplyFriendWaitInfoWnd.h"

QApplyFriendWaitInfoWnd::QApplyFriendWaitInfoWnd(QWidget* p /*= nullptr*/)
	: QWidget(p)
{
	setContentsMargins(0, 0, 0, 0);
	setFixedSize(460, 300);

	m_vLayout = new QVBoxLayout();
	setLayout(m_vLayout);
	m_infoLabel = new QLabel();
	m_infoLabel->setText("你的好友添加请求已经发送成功，正在等待对方确认");
	m_vLayout->addWidget(m_infoLabel);
	m_vLayout->addStretch();
}

