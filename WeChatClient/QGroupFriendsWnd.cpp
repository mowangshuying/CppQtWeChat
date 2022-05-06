#include "QGroupFriendsWnd.h"
#include <QScrollArea>

QGroupFriendsWnd::QGroupFriendsWnd(QWidget* p /*= nullptr*/)
	: QWidget(p)
{
	m_vLayout = new QVBoxLayout();
	setLayout(m_vLayout);

	m_listWnd = new QListWidget();
	m_vLayout->addWidget(m_listWnd);

	setContentsMargins(0, 0, 0, 0);
	m_vLayout->setContentsMargins(0, 0, 0, 0);
	m_listWnd->setContentsMargins(0, 0, 0, 0);

	m_listWnd->setAttribute(Qt::WA_StyledBackground);
	m_listWnd->setWindowFlags(Qt::FramelessWindowHint);

	setAttribute(Qt::WA_StyledBackground);
	setWindowFlags(Qt::FramelessWindowHint);
	m_listWnd->setStyleSheet("border:0px");
}

void QGroupFriendsWnd::addItem(QGroupFriendsItemWnd* itemWnd)
{
	QListWidgetItem* pListItem = new QListWidgetItem(m_listWnd);
	pListItem->setSizeHint(QSize(250,40));
	m_listWnd->setItemWidget(pListItem, itemWnd);
}

