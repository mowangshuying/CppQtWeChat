#include "QSelectAddGroupOrAddFriendWnd.h"
#include "QFindFriendOrGroupWnd.h"
#include "QSimpleSplit.h"
#include <QDebug>
#include <QApplication>

QSelectAddGroupOrAddFriendWnd::QSelectAddGroupOrAddFriendWnd(QWidget* p) : QWidget(p)
{
    setAttribute(Qt::WA_StyledBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedWidth(100);
    m_vLayout = new QVBoxLayout(this);
    m_vLayout->setContentsMargins(0, 0, 0, 0);
    m_vLayout->setSpacing(0);

    m_addContactsOrGroupBtn = new QPushButton("添加好友或群", this);
    m_createGroupBtn = new QPushButton("创建群聊", this);

    m_addContactsOrGroupBtn->setStyleSheet("border:0px;");
    m_createGroupBtn->setStyleSheet("border:0px;");

    m_vLayout->addWidget(m_addContactsOrGroupBtn);
    {
        QSimpleSplit* sp = new QSimpleSplit();
        m_vLayout->addWidget(sp);
    }
    m_vLayout->addWidget(m_createGroupBtn);

    setFixedHeight(m_createGroupBtn->height() + m_addContactsOrGroupBtn->height());
    setLayout(m_vLayout);

    m_findFriendOrGroupWnd = new QFindFriendOrGroupWnd();
    m_findFriendOrGroupWnd->hide();

    m_crateGroupWnd = new QCreateGroupWnd();
    m_crateGroupWnd->hide();

    connect(m_addContactsOrGroupBtn, SIGNAL(clicked()), this, SLOT(slot_addContactsOrGroupBtnClick()));
    connect(m_createGroupBtn, SIGNAL(clicked()), this, SLOT(slot_createGroup()));
}

void QSelectAddGroupOrAddFriendWnd::slot_addContactsOrGroupBtnClick()
{
    if (m_findFriendOrGroupWnd != nullptr)
    {
        m_findFriendOrGroupWnd->showNormal();
        m_findFriendOrGroupWnd->activateWindow();
    }
}

void QSelectAddGroupOrAddFriendWnd::slot_createGroup()
{
    if (m_crateGroupWnd != nullptr)
    {
        m_crateGroupWnd->updateData();
        m_crateGroupWnd->show();
    }
}

bool QSelectAddGroupOrAddFriendWnd::event(QEvent* event)
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
