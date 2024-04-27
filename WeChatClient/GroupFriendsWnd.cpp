#include "GroupFriendsWnd.h"
#include <QScrollArea>

GroupFriendsWnd::GroupFriendsWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    LogFunc;
    setObjectName("QGroupFriendsWnd");

    m_vLayout = new QVBoxLayout();
    setLayout(m_vLayout);

    m_listWnd = new QListWidget();
    m_listWnd->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_vLayout->addWidget(m_listWnd);

    setContentsMargins(0, 0, 0, 0);
    m_vLayout->setContentsMargins(0, 0, 0, 0);
    m_listWnd->setContentsMargins(0, 0, 0, 0);

    m_listWnd->setAttribute(Qt::WA_StyledBackground);
    m_listWnd->setWindowFlags(Qt::FramelessWindowHint);

    // setAttribute(Qt::WA_StyledBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    m_listWnd->setStyleSheet("border:0px;");
}

void GroupFriendsWnd::addItem(GroupFriendsItemWnd* itemWnd)
{
    QListWidgetItem* pListItem = new QListWidgetItem(m_listWnd);
    pListItem->setSizeHint(QSize(250, 50));
    // itemWnd->setFixedSize(250, 40);
    LogDebug << "list item size:" << pListItem->sizeHint() << ", item wnd:" << itemWnd->size();
    m_listWnd->setItemWidget(pListItem, itemWnd);
}
