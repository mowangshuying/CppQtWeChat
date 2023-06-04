#include <QDebug>
#include <QScrollBar>
#include "QMainWnd.h"

#include "QSelectAddGroupOrAddFriendWnd.h"
#include "QSelfSplit.h"
#include "QCommListWnd.h"
#include "QCommMsgItemWnd.h"
#include "QCommContactItemWnd.h"
#include "QCustomListWidgetItem.h"
#include "QCommGroupItemWnd.h"
#include "QStyleSheetMgr.h"

QCommListWnd::QCommListWnd(QWidget* p /*= nullptr*/, QCommListWndEnum wndType /*QCommMsgItemWnd_Tpye*/) : QWidget(p), m_WndType(wndType)
{
    LogFunc;
    setObjectName("QCommListWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + objectName() + ".qss";
    object.m_widget = this;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

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
        /*��ӷָ��ߵ�ʾ������*/
        QSelfSplit* sp = new QSelfSplit(this);
        m_vLayout->addWidget(sp);
    }

    m_listWidget = new QListWidget();
    m_listWidget->setFixedWidth(255);
    // m_listWidget->setAttribute(Qt::WA_StyledBackground);
    m_listWidget->setWindowFlags(Qt::FramelessWindowHint);
    m_listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_vLayout->addWidget(m_listWidget);

    m_selectWnd = new QSelectAddGroupOrAddFriendWnd(nullptr);
    m_selectWnd->hide();

    m_searchEdit->installEventFilter(this);

    setFixedWidth(255);
    // setObjectName("QCommListWnd");
    // setStyleSheet("#QCommListWnd{background:white;border:0px;}");
    setAttribute(Qt::WA_StyledBackground);
    // setWindowFlags(Qt::FramelessWindowHint);

    // m_listWidget->setStyleSheet("border:0px;");
    connect(m_listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slotOnCurrentItemClicked(QListWidgetItem*)));
    connect(m_startGroupBtn, SIGNAL(clicked()), this, SLOT(slotOnStartGroupBtnClicked()));
    // setStyleSheet("background-color:red;");
}

void QCommListWnd::slotOnCurrentItemClicked(QListWidgetItem* item)
{
    // �Զ����б�
    // ���������б���Ϣ����ϵ���б���Ϣ�б�Ⱥ���б�
    QCustomListWidgetItem* pCustItem = dynamic_cast<QCustomListWidgetItem*>(item);
    LogDebug << "sesid:" << pCustItem->sesId();

    //��ǰ���������ϵ���б��е�ĳһ��Ŀ
    if (m_WndType == QCommListWndEnum::ContactItemWndType)
    {
        QCommContactItemWnd* wnd = dynamic_cast<QCommContactItemWnd*>(m_listWidget->itemWidget(pCustItem));
        if (wnd->m_bNewFriend)
        {
            QMainWnd::getMainWnd()->m_dealNewFriendsApplyWnd->setFriendApplyList();
            //�л����ڵ��µ����Ѵ���
            // m_sLayout2��λ��Ϊ1�Ĵ���Ϊ�µ����ѵĴ���
            QMainWnd::getMainWnd()->m_sRightLayout->setCurrentIndex(1);
        }
        else
        {
            // ������Ϣ
            // ����ϵ�˵����ֺ͸���ϵ�˵�id��Ϣ
            QMap<QString, QString> infoMap;
            infoMap["name"] = wnd->getContactItemName();
            infoMap["friendid"] = QString::number(wnd->getFriendId(), 10);

            //�л���չʾ��ϵ����Ϣ��ҳ��
            QMainWnd::getMainWnd()->m_sRightLayout->setCurrentIndex(0);

            //��ϵ�˵���Ϣ�ı�
            emit signalContactInfoChange(infoMap);
        }
        return;
    }

    //��ǰ���������Ϣ�б��е�ĳһ��
    if (m_WndType == QCommListWndEnum::MsgItemWndTpye || m_WndType == QCommListWndEnum::SearchItemWndType)
    {
        QCommMsgItemWnd* wnd = dynamic_cast<QCommMsgItemWnd*>(m_listWidget->itemWidget(pCustItem));
        qint64 sesid = wnd->m_sesId;
        signalCommListChanged(sesid);
        return;
    }

    //��ǰ���������Ⱥ�б��е�ĳһ��
    if (m_WndType == QCommListWndEnum::GroupItemWndType)
    {
        //�жϵ�ǰ�������һ��
        QCommGroupItemWnd* wnd = dynamic_cast<QCommGroupItemWnd*>(m_listWidget->itemWidget(pCustItem));
        //��ȡ��Ⱥ��id
        int groupid = wnd->m_groupId;
        //�ұ߹�������Ҫ��ת��msg��һ��
        QMainWnd::getMainWnd()->m_toolWnd->m_msgBtn->click();
        //���ҵ���Ϣ�б������ڵ�λ��
        auto msgListWidget = QMainWnd::getMainWnd()->m_commMsgListWnd->m_listWidget;

        for (int i = 0; i < msgListWidget->count(); i++)
        {
            QListWidgetItem* pitem = msgListWidget->item(i);
            QCommMsgItemWnd* pWnd = dynamic_cast<QCommMsgItemWnd*>(msgListWidget->itemWidget(pitem));
            if (pWnd->m_isGroupMsg && pWnd->m_userid == groupid)
            {
                msgListWidget->setCurrentItem(pitem);
                QMainWnd::getMainWnd()->slotSesIdToIndex(pWnd->m_sesId);
                break;
            }
        }
        return;
    }
}

void QCommListWnd::slotOnStartGroupBtnClicked()
{
    // LogDebug << "onStartGroupBtnClicked()";
    QRect rect = m_startGroupBtn->geometry();
    LogDebug << "rect" << rect << endl;
    LogDebug << " the pos of startGroupBtn: " << m_startGroupBtn->pos();
    QPoint gPoint = m_startGroupBtn->mapToGlobal(QPoint(0, 0));

    /*ѡ��Ĵ���ֻ���������һ��*/
    QRect swRect = m_selectWnd->geometry();
    swRect.setX(gPoint.x() - m_selectWnd->width() + m_startGroupBtn->width());
    swRect.setY(gPoint.y() + m_startGroupBtn->height() + 5);
    m_selectWnd->setGeometry(swRect);
    m_selectWnd->show();
}

bool QCommListWnd::eventFilter(QObject* target, QEvent* event)
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

void QCommListWnd::addMsgItem(const char* name, const char* msg, qint64 sesid, int64_t userid, bool isGroupMsg)
{
    if (hasMsgItemBySesId(sesid))
    {
        LogDebug << "has same ses sesid = " << sesid;
        return;
    }

    LogDebug << "msg:" << msg;
    QCommMsgItemWnd* pMsgItem = new QCommMsgItemWnd(m_listWidget, name, msg, sesid, userid, isGroupMsg);
    QListWidgetItem* pListItem = new QCustomListWidgetItem(m_listWidget);
    pMsgItem->setFixedWidth(this->width() - 5);
    pListItem->setSizeHint(QSize(this->width() - 5, 65));
    m_listWidget->setItemWidget(pListItem, pMsgItem);
}

void QCommListWnd::addContactsItem(const char* headUrl, const char* name, bool isNewFriend /*= false*/, int friendid /* = -1*/)
{
    if (hasContactsItemByFriendId(friendid))
    {
        LogDebug << "has same friend friendId = " << friendid;
        return;
    }
    QCommContactItemWnd* pMsgItem = new QCommContactItemWnd(m_listWidget, headUrl, name, isNewFriend, friendid);
    QListWidgetItem* pListItem = new QCustomListWidgetItem(m_listWidget);
    pMsgItem->setFixedWidth(this->width() - 5);
    pListItem->setSizeHint(QSize(this->width() - 5, 65));
    m_listWidget->setItemWidget(pListItem, pMsgItem);
}

void QCommListWnd::addGroupItem(const char* headUrl, const char* name, int groupid)
{
    if (hasGroupItemByGroupId(groupid))
    {
        LogDebug << "has same group groupid = " << groupid;
        return;
    }

    QCommGroupItemWnd* pGroupItem = new QCommGroupItemWnd(m_listWidget, headUrl, name, groupid);
    QListWidgetItem* pListItem = new QCustomListWidgetItem(m_listWidget);
    pGroupItem->setFixedWidth(this->width() - 5);
    pListItem->setSizeHint(QSize(this->width() - 5, 65));
    m_listWidget->setItemWidget(pListItem, pGroupItem);
}

bool QCommListWnd::hasMsgItemBySesId(int64_t sesid)
{
    bool bHas = false;
    int count = m_listWidget->count();
    for (int i = 0; i < count; i++)
    {
        QCustomListWidgetItem* pitem = (QCustomListWidgetItem*)m_listWidget->item(i);
        QCommMsgItemWnd* pWnd = dynamic_cast<QCommMsgItemWnd*>(m_listWidget->itemWidget(pitem));
        if (pWnd && pWnd->m_sesId == sesid)
        {
            bHas = true;
            break;
        }
    }
    return bHas;
}

bool QCommListWnd::hasGroupItemByGroupId(int64_t groupid)
{
    //�ж�Ⱥ���б����Ƿ��д�Ⱥ��
    int count = m_listWidget->count();
    bool bHas = false;
    for (int i = 0; i < count; i++)
    {
        QListWidgetItem* pitem = m_listWidget->item(i);
        QCommGroupItemWnd* pWnd = dynamic_cast<QCommGroupItemWnd*>(m_listWidget->itemWidget(pitem));
        if (pWnd && pWnd->m_groupId == groupid)
        {
            bHas = true;
            break;
        }
    }
    return bHas;
}

bool QCommListWnd::hasContactsItemByFriendId(int64_t friendId)
{
    bool bHas = false;
    int count = m_listWidget->count();
    for (int i = 0; i < count; i++)
    {
        QListWidgetItem* pitem = m_listWidget->item(i);
        QCommContactItemWnd* pWnd = dynamic_cast<QCommContactItemWnd*>(m_listWidget->itemWidget(pitem));
        if (pWnd->m_friendId == friendId)
        {
            bHas = true;
        }
    }
    return bHas;
}

void QCommListWnd::setGroupItemNameByGroupId(int64_t grouId, QString groupName)
{
    int count = m_listWidget->count();
    bool bHas = false;
    for (int i = 0; i < count; i++)
    {
        QListWidgetItem* pitem = m_listWidget->item(i);
        QCommGroupItemWnd* pWnd = dynamic_cast<QCommGroupItemWnd*>(m_listWidget->itemWidget(pitem));
        if (pWnd->m_groupId == grouId)
        {
            QString rawGroupName = pWnd->m_groupName->text();
            pWnd->m_groupName->setText(groupName);
            LogDebug << "update groupName:" << rawGroupName << " -> " << pWnd->m_groupName->text();
            break;
        }
    }
}

void QCommListWnd::setMsgItemNameBySesId(int64_t sesId, QString msgItemName)
{
    int count = m_listWidget->count();
    for (int i = 0; i < count; i++)
    {
        QListWidgetItem* pitem = m_listWidget->item(i);
        QCommMsgItemWnd* pWnd = dynamic_cast<QCommMsgItemWnd*>(m_listWidget->itemWidget(pitem));
        if (pWnd->m_sesId == sesId)
        {
            pWnd->m_name->setText(msgItemName);
            break;
        }
    }
}
