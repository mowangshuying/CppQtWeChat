#include <QDebug>
#include <QScrollBar>
#include "MainWnd.h"

#include "SelectAddGroupOrAddFriendWnd.h"
#include "SelfSplit.h"
#include "CommListWnd.h"
#include "CommMsgItemWnd.h"
#include "CommContactItemWnd.h"
#include "CustomListWidgetItem.h"
#include "CommGroupItemWnd.h"
#include "StyleSheetMgr.h"

CommListWnd::CommListWnd(QWidget* p /*= nullptr*/,
                           QCommListWndEnum wndType /*QCommMsgItemWnd_Tpye*/)
    : QWidget(p), m_WndType(wndType)
{
    LogFunc;
    setObjectName("QCommListWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + objectName() + ".qss";
    object.m_widget = this;
    StyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    m_vLayout = new QVBoxLayout(this);
    m_vLayout->setContentsMargins(0, 0, 0, 0);
    m_vLayout->setSpacing(0);
    setLayout(m_vLayout);
    setMouseTracking(true);

    m_hLayout = new QHBoxLayout(this);
    m_hLayout->setContentsMargins(0, 0, 0, 0);
    m_hLayout->setSpacing(0);
    m_searchEdit = new QLineEdit(this);
    m_startGroupBtn = new QPushButton(this);
    m_searchEdit->setFixedHeight(25);
    m_startGroupBtn->setFixedSize(25, 25);
    m_startGroupBtn->setIcon(QPixmap("./img/startGroupBtn.png"));
    m_startGroupBtn->setIconSize(QSize(25, 25));

    m_hLayout->addSpacing(10);
    m_hLayout->addWidget(m_searchEdit);
    m_hLayout->addSpacing(10);
    m_hLayout->addWidget(m_startGroupBtn);
    m_hLayout->addSpacing(10);
    m_vLayout->addSpacing(25);
    m_vLayout->addLayout(m_hLayout);
    m_vLayout->addSpacing(15);

    {
        /*添加分割线的示例代码*/
        SelfSplit* sp = new SelfSplit(this);
        m_vLayout->addWidget(sp);
    }

    m_listWidget = new QListWidget();
    m_listWidget->setFixedWidth(255);
    // m_listWidget->setAttribute(Qt::WA_StyledBackground);
    m_listWidget->setWindowFlags(Qt::FramelessWindowHint);
    m_listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_vLayout->addWidget(m_listWidget);

    m_selectWnd = new SelectAddGroupOrAddFriendWnd(nullptr);
    m_selectWnd->hide();

    m_searchEdit->installEventFilter(this);

    setFixedWidth(255);
    // setObjectName("QCommListWnd");
    // setStyleSheet("#QCommListWnd{background:white;border:0px;}");
    setAttribute(Qt::WA_StyledBackground);
    // setWindowFlags(Qt::FramelessWindowHint);

    // m_listWidget->setStyleSheet("border:0px;");
    connect(m_listWidget,
            SIGNAL(itemClicked(QListWidgetItem*)),
            this,
            SLOT(slotOnCurrentItemClicked(QListWidgetItem*)));
    connect(m_startGroupBtn,
            SIGNAL(clicked()),
            this,
            SLOT(slotOnStartGroupBtnClicked()));
    // setStyleSheet("background-color:red;");
}

void CommListWnd::slotOnCurrentItemClicked(QListWidgetItem* item)
{
    // 自定义列表
    // 包含如下列表信息：联系人列表、消息列表、群聊列表
    CustomListWidgetItem* pCustItem =
        dynamic_cast<CustomListWidgetItem*>(item);
    LogDebug << "sesid:" << pCustItem->sesId();

    //当前点击的是联系人列表中的某一项目
    if (m_WndType == QCommListWndEnum::ContactItemWndType)
    {
        CommContactItemWnd* wnd = dynamic_cast<CommContactItemWnd*>(
            m_listWidget->itemWidget(pCustItem));
        if (wnd->m_bNewFriend)
        {
            MainWnd::getMainWnd()
                ->m_dealNewFriendsApplyWnd->setFriendApplyList();
            //切换窗口到新的朋友窗口
            // m_sLayout2中位置为1的窗口为新的朋友的窗口
            MainWnd::getMainWnd()->m_sRightLayout->setCurrentIndex(1);
        }
        else
        {
            // 设置信息
            // 该联系人的名字和该联系人的id信息
            QMap<QString, QString> infoMap;
            infoMap["name"] = wnd->getContactItemName();
            infoMap["friendid"] = QString::number(wnd->getFriendId(), 10);

            //切换到展示联系人信息的页面
            MainWnd::getMainWnd()->m_sRightLayout->setCurrentIndex(0);

            //联系人的信息改变
            emit signalContactInfoChange(infoMap);
        }
        return;
    }

    //当前点击的是消息列表中的某一项
    if (m_WndType == QCommListWndEnum::MsgItemWndTpye ||
        m_WndType == QCommListWndEnum::SearchItemWndType)
    {
        CommMsgItemWnd* wnd =
            dynamic_cast<CommMsgItemWnd*>(m_listWidget->itemWidget(pCustItem));
        qint64 sesid = wnd->m_sesId;
        signalCommListChanged(sesid);
        return;
    }

    //当前点击的我是群列表中的某一项
    if (m_WndType == QCommListWndEnum::GroupItemWndType)
    {
        //判断当前点击是那一项
        CommGroupItemWnd* wnd = dynamic_cast<CommGroupItemWnd*>(
            m_listWidget->itemWidget(pCustItem));
        //获取到群的id
        int groupid = wnd->m_groupId;
        //右边工具栏需要跳转到msg那一栏
        MainWnd::getMainWnd()->m_toolWnd->m_msgBtn->click();
        //先找到消息列表中所在的位置
        auto msgListWidget =
            MainWnd::getMainWnd()->m_commMsgListWnd->m_listWidget;

        for (int i = 0; i < msgListWidget->count(); i++)
        {
            QListWidgetItem* pitem = msgListWidget->item(i);
            CommMsgItemWnd* pWnd = dynamic_cast<CommMsgItemWnd*>(
                msgListWidget->itemWidget(pitem));
            if (pWnd->m_isGroupMsg && pWnd->m_userid == groupid)
            {
                msgListWidget->setCurrentItem(pitem);
                MainWnd::getMainWnd()->slotSesIdToIndex(pWnd->m_sesId);
                break;
            }
        }
        return;
    }
}

void CommListWnd::slotOnStartGroupBtnClicked()
{
    // LogDebug << "onStartGroupBtnClicked()";
    QRect rect = m_startGroupBtn->geometry();
    LogDebug << "rect" << rect << endl;
    LogDebug << " the pos of startGroupBtn: " << m_startGroupBtn->pos();
    QPoint gPoint = m_startGroupBtn->mapToGlobal(QPoint(0, 0));

    /*选择的窗口只能允许出现一个*/
    QRect swRect = m_selectWnd->geometry();
    swRect.setX(gPoint.x() - m_selectWnd->width() + m_startGroupBtn->width());
    swRect.setY(gPoint.y() + m_startGroupBtn->height() + 5);
    m_selectWnd->setGeometry(swRect);
    m_selectWnd->show();
}

bool CommListWnd::eventFilter(QObject* target, QEvent* event)
{
    if (target == m_searchEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent* tmpKeyEvent = (QKeyEvent*)event;
        if (tmpKeyEvent->key() == Qt::Key_Return)
        {
            LogDebug << "Press Enter Key searchText = " << m_searchEdit->text();
            emit signalSearchText(m_searchEdit->text());
            return true;
        }
    }

    return QWidget::eventFilter(target, event);
}

void CommListWnd::addMsgItem(const char* name,
                              const char* msg,
                              qint64 sesid,
                              int64_t userid,
                              bool isGroupMsg)
{
    if (hasMsgItemBySesId(sesid))
    {
        LogDebug << "has same ses sesid = " << sesid;
        return;
    }

    LogDebug << "msg:" << msg;
    CommMsgItemWnd* pMsgItem =
        new CommMsgItemWnd(m_listWidget, name, msg, sesid, userid, isGroupMsg);
    QListWidgetItem* pListItem = new CustomListWidgetItem(m_listWidget);
    pMsgItem->setFixedWidth(this->width() - 5);
    pListItem->setSizeHint(QSize(this->width() - 5, 65));
    m_listWidget->setItemWidget(pListItem, pMsgItem);
}

void CommListWnd::addContactsItem(const char* headUrl,
                                   const char* name,
                                   bool isNewFriend /*= false*/,
                                   int friendid /* = -1*/)
{
    if (hasContactsItemByFriendId(friendid))
    {
        LogDebug << "has same friend friendId = " << friendid;
        return;
    }
    CommContactItemWnd* pMsgItem = new CommContactItemWnd(
        m_listWidget, headUrl, name, isNewFriend, friendid);
    QListWidgetItem* pListItem = new CustomListWidgetItem(m_listWidget);
    pMsgItem->setFixedWidth(this->width() - 5);
    pListItem->setSizeHint(QSize(this->width() - 5, 65));
    m_listWidget->setItemWidget(pListItem, pMsgItem);
}

void CommListWnd::addGroupItem(const char* headUrl,
                                const char* name,
                                int groupid)
{
    if (hasGroupItemByGroupId(groupid))
    {
        LogDebug << "has same group groupid = " << groupid;
        return;
    }

    CommGroupItemWnd* pGroupItem =
        new CommGroupItemWnd(m_listWidget, headUrl, name, groupid);
    QListWidgetItem* pListItem = new CustomListWidgetItem(m_listWidget);
    pGroupItem->setFixedWidth(this->width() - 5);
    pListItem->setSizeHint(QSize(this->width() - 5, 65));
    m_listWidget->setItemWidget(pListItem, pGroupItem);
}

bool CommListWnd::hasMsgItemBySesId(int64_t sesid)
{
    bool bHas = false;
    int count = m_listWidget->count();
    for (int i = 0; i < count; i++)
    {
        CustomListWidgetItem* pitem =
            (CustomListWidgetItem*)m_listWidget->item(i);
        CommMsgItemWnd* pWnd =
            dynamic_cast<CommMsgItemWnd*>(m_listWidget->itemWidget(pitem));
        if (pWnd && pWnd->m_sesId == sesid)
        {
            bHas = true;
            break;
        }
    }
    return bHas;
}

bool CommListWnd::hasGroupItemByGroupId(int64_t groupid)
{
    //判断群聊列表中是否含有此群聊
    int count = m_listWidget->count();
    bool bHas = false;
    for (int i = 0; i < count; i++)
    {
        QListWidgetItem* pitem = m_listWidget->item(i);
        CommGroupItemWnd* pWnd =
            dynamic_cast<CommGroupItemWnd*>(m_listWidget->itemWidget(pitem));
        if (pWnd && pWnd->m_groupId == groupid)
        {
            bHas = true;
            break;
        }
    }
    return bHas;
}

bool CommListWnd::hasContactsItemByFriendId(int64_t friendId)
{
    bool bHas = false;
    int count = m_listWidget->count();
    for (int i = 0; i < count; i++)
    {
        QListWidgetItem* pitem = m_listWidget->item(i);
        CommContactItemWnd* pWnd =
            dynamic_cast<CommContactItemWnd*>(m_listWidget->itemWidget(pitem));
        if (pWnd->m_friendId == friendId)
        {
            bHas = true;
        }
    }
    return bHas;
}

void CommListWnd::setGroupItemNameByGroupId(int64_t grouId, QString groupName)
{
    int count = m_listWidget->count();
    bool bHas = false;
    for (int i = 0; i < count; i++)
    {
        QListWidgetItem* pitem = m_listWidget->item(i);
        CommGroupItemWnd* pWnd =
            dynamic_cast<CommGroupItemWnd*>(m_listWidget->itemWidget(pitem));
        if (pWnd->m_groupId == grouId)
        {
            QString rawGroupName = pWnd->m_groupName->text();
            pWnd->m_groupName->setText(groupName);
            LogDebug << "update groupName:" << rawGroupName << " -> "
                     << pWnd->m_groupName->text();
            break;
        }
    }
}

void CommListWnd::setMsgItemNameBySesId(int64_t sesId, QString msgItemName)
{
    int count = m_listWidget->count();
    for (int i = 0; i < count; i++)
    {
        QListWidgetItem* pitem = m_listWidget->item(i);
        CommMsgItemWnd* pWnd =
            dynamic_cast<CommMsgItemWnd*>(m_listWidget->itemWidget(pitem));
        if (pWnd->m_sesId == sesId)
        {
            pWnd->m_name->setText(msgItemName);
            break;
        }
    }
}
