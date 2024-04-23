#include "SelectAddGroupOrAddFriendWnd.h"
#include "FindFriendOrGroupWnd.h"
#include "SelfSplit.h"
#include <QDebug>
#include <QApplication>

#include "StyleSheetMgr.h"

SelectAddGroupOrAddFriendWnd::SelectAddGroupOrAddFriendWnd(QWidget* p) : QWidget(p)
{
    LogFunc;
    m_centerWnd = new QWidget(this);

    m_centerWnd->setObjectName("QSelectAddGroupOrAddFriendWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + m_centerWnd->objectName() + ".qss";
    object.m_widget = m_centerWnd;
    StyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);

    setFixedSize(120, 100);

    m_vLayout = new QVBoxLayout(m_centerWnd);
    m_vLayout->setContentsMargins(10, 10, 10, 10);
    m_vLayout->setSpacing(0);

    m_addContactsOrGroupBtn = new QPushButton("添加好友或群");
    m_createGroupBtn = new QPushButton("创建群聊");

    m_vLayout->addWidget(m_addContactsOrGroupBtn);
    ///* {
    //     QSimpleSplit* sp = new QSimpleSplit();
    //     m_vLayout->addWidget(sp);
    // }*/
    m_vLayout->addWidget(m_createGroupBtn);

    //  setFixedHeight(m_createGroupBtn->height() +
    //  m_addContactsOrGroupBtn->height());
    m_centerWnd->setLayout(m_vLayout);

    m_findFriendOrGroupWnd = new FindFriendOrGroupWnd();
    m_findFriendOrGroupWnd->hide();

    m_crateGroupWnd = new CreateGroupWnd();
    m_crateGroupWnd->hide();

    connect(m_addContactsOrGroupBtn, SIGNAL(clicked()), this, SLOT(slotAddContactsOrGroupBtnClick()));
    connect(m_createGroupBtn, SIGNAL(clicked()), this, SLOT(slotCreateGroup()));
}

void SelectAddGroupOrAddFriendWnd::slotAddContactsOrGroupBtnClick()
{
    if (m_findFriendOrGroupWnd != nullptr)
    {
        m_findFriendOrGroupWnd->showNormal();
        m_findFriendOrGroupWnd->activateWindow();
    }
}

void SelectAddGroupOrAddFriendWnd::slotCreateGroup()
{
    if (m_crateGroupWnd != nullptr)
    {
        m_crateGroupWnd->updateData();
        m_crateGroupWnd->show();
    }
}

bool SelectAddGroupOrAddFriendWnd::event(QEvent* event)
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
