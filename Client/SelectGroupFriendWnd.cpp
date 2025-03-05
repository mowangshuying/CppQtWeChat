#include "SelectGroupFriendWnd.h"
#include "FindFriendOrGroupWnd.h"
#include "SelfSplit.h"
#include <QDebug>
#include <QApplication>

#include "StyleSheetMgr.h"

SelectGroupFriendWnd::SelectGroupFriendWnd(QWidget* p) : QWidget(p)
{
    LogFunc;
    m_vMainLayout = new QVBoxLayout;
    setLayout(m_vMainLayout);
    m_vMainLayout->setContentsMargins(5, 5, 5, 5);

    m_centerWnd = new QWidget;
    m_vMainLayout->addWidget(m_centerWnd);

    m_centerWnd->setObjectName("QSelectAddGroupOrAddFriendWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + m_centerWnd->objectName() + ".qss";
    object.m_widget = m_centerWnd;
    StyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);

    // setFixedSize(120, 100);

    m_vCenterLayout = new QVBoxLayout;
    m_vCenterLayout->setContentsMargins(5, 5, 5, 5);
    m_vCenterLayout->setSpacing(0);

    m_addContactsOrGroupBtn = new QPushButton("添加好友或群");
    m_createGroupBtn = new QPushButton("创建群聊");

    m_vCenterLayout->addWidget(m_addContactsOrGroupBtn);
    m_vCenterLayout->addWidget(m_createGroupBtn);

    m_centerWnd->setLayout(m_vCenterLayout);

    m_findFriendOrGroupWnd = new FindFriendOrGroupWnd();
    m_findFriendOrGroupWnd->hide();

    m_crateGroupWnd = new CreateGroupWnd();
    m_crateGroupWnd->hide();

    connect(m_addContactsOrGroupBtn, SIGNAL(clicked()), this, SLOT(slotAddContactsOrGroupBtnClick()));
    connect(m_createGroupBtn, SIGNAL(clicked()), this, SLOT(slotCreateGroup()));
}

void SelectGroupFriendWnd::slotAddContactsOrGroupBtnClick()
{
    if (m_findFriendOrGroupWnd != nullptr)
    {
        m_findFriendOrGroupWnd->showNormal();
        m_findFriendOrGroupWnd->activateWindow();
    }
}

void SelectGroupFriendWnd::slotCreateGroup()
{
    if (m_crateGroupWnd != nullptr)
    {
        m_crateGroupWnd->updateData();
        m_crateGroupWnd->show();
    }
}

bool SelectGroupFriendWnd::event(QEvent* event)
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
