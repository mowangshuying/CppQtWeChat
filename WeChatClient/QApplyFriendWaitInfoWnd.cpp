#include "QApplyFriendWaitInfoWnd.h"

QApplyFriendWaitInfoWnd::QApplyFriendWaitInfoWnd(QWidget* p /*= nullptr*/)
	: QWidget(p)
{
	setContentsMargins(0, 0, 0, 0);
	setFixedSize(460, 300);

	m_vLayout = new QVBoxLayout();
	setLayout(m_vLayout);
	m_infoLabel = new QLabel();
	m_infoLabel->setText("��ĺ�����������Ѿ����ͳɹ������ڵȴ��Է�ȷ��");
	m_vLayout->addWidget(m_infoLabel);
	m_vLayout->addStretch();
}

