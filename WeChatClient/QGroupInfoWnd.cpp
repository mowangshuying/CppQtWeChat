#include "QGroupInfoWnd.h"
#include <QApplication>
#include "QSelfSplit.h"
#include "QGroupFriendsItemWnd.h"
#include <QScrollArea>

#include "QStyleSheetMgr.h"
#include "QWSClientMgr.h"
#include "QMainWnd.h"

QGroupInfoWnd::QGroupInfoWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    LogFunc;
    m_centerWnd = new QWidget(this);
    setObjectName("QGroupInfoWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + objectName() + ".qss";
    object.m_widget = m_centerWnd;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    setFixedWidth(265);
    setMinimumHeight(535);

    // 滚动区域
    m_scrollArea = new QScrollArea(m_centerWnd);
    m_scrollAreaWnd = new QWidget(m_centerWnd);

    m_scrollAreaWnd->setObjectName("QGroupInfoCenterWnd");
    object.m_qssFileName =
        "./stylesheet/" + m_scrollAreaWnd->objectName() + ".qss";
    object.m_widget = m_scrollAreaWnd;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    m_scrollArea->setGeometry(0, 0, width(), height());

    m_vLayout = new QVBoxLayout();
    m_scrollAreaWnd->setLayout(m_vLayout);

    m_vLayout->setContentsMargins(0, 0, 0, 0);

    //开始组装
    m_groupfriendsWnd = new QGroupFriendsWnd(this);
    m_groupfriendsWnd->setObjectName("QGroupFriendsWnd");
    m_groupfriendsWnd->setFixedHeight(249);
    m_vLayout->addWidget(m_groupfriendsWnd);

    m_simpleGroupInfoWnd = new QWidget(this);
    m_simpleGroupInfoWnd->setObjectName("QSimpleGroupInfoWnd");

    object.m_qssFileName =
        "./stylesheet/" + m_simpleGroupInfoWnd->objectName() + ".qss";
    object.m_widget = m_simpleGroupInfoWnd;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    m_vLayout1 = new QVBoxLayout();
    m_simpleGroupInfoWnd->setLayout(m_vLayout1);
    m_vLayout->addWidget(m_simpleGroupInfoWnd);

    m_groupName1 = new QLabel();
    m_groupName1->setText("群聊名称");
    m_vLayout1->addWidget(m_groupName1);

    m_groupName2 = new QEditLabel(m_centerWnd);
    m_groupName2->setFixedSize(200, 25);
    m_groupName2->setText("请在此处添加群聊名称...");
    m_vLayout1->addWidget(m_groupName2);

    m_groupNotice1 = new QLabel();
    m_groupNotice1->setText("群公告");
    m_vLayout1->addWidget(m_groupNotice1);

    m_groupNotice2 = new QEditLabel(m_centerWnd);
    m_groupNotice2->setText("请在此处设置群公告...");
    m_groupNotice2->setFixedSize(200, 25);
    m_vLayout1->addWidget(m_groupNotice2);

    // 提供分割线
    m_vLayout1->addSpacing(5);

    m_groupRemarks1 = new QLabel();
    m_groupRemarks1->setText("备注");
    m_vLayout1->addWidget(m_groupRemarks1);

    // 提供分割线
    m_vLayout1->addSpacing(5);

    m_groupRemarks2 = new QEditLabel(m_centerWnd);
    m_groupRemarks2->setText("请在此处设置群备注...");
    m_groupRemarks2->setFixedSize(200, 25);
    m_vLayout1->addWidget(m_groupRemarks2);

    // 提供分割线
    m_vLayout1->addSpacing(5);

    m_groupRoleName1 = new QLabel();
    m_groupRoleName1->setText("我在本群的昵称");
    m_vLayout1->addWidget(m_groupRoleName1);

    // 提供分割线
    m_vLayout1->addSpacing(5);

    m_groupRoleName2 = new QEditLabel(m_centerWnd);
    m_groupRoleName2->setText("请在此处设置备注...");
    m_groupRoleName2->setFixedSize(200, 25);
    ;
    m_vLayout1->addWidget(m_groupRoleName2);

    m_simpleGroupSettingWnd = new QWidget(this);
    m_simpleGroupSettingWnd->setObjectName("QSimpleGroupSettingWnd");
    object.m_qssFileName =
        "./stylesheet/" + m_simpleGroupSettingWnd->objectName() + ".qss";
    object.m_widget = m_simpleGroupSettingWnd;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    m_vLayout->addWidget(m_simpleGroupSettingWnd);
    m_vLayout2 = new QVBoxLayout();

    m_simpleGroupSettingWnd->setLayout(m_vLayout2);

    m_hLayout21 = new QHBoxLayout();
    m_vLayout2->addLayout(m_hLayout21);

    m_showGroupFriendsRoleNameLabel = new QLabel();
    m_showGroupFriendsRoleNameLabel->setText("显示群成员昵称");
    m_hLayout21->addWidget(m_showGroupFriendsRoleNameLabel);

    m_showGroupFriendsRoleNameBtn = new QSelfSwitchButton();
    m_hLayout21->addWidget(m_showGroupFriendsRoleNameBtn);
    m_hLayout21->addSpacing(40);

    //
    m_hLayout22 = new QHBoxLayout();
    m_vLayout2->addLayout(m_hLayout22);
    m_MsgFreeLabel = new QLabel();
    m_MsgFreeLabel->setText("消息免打扰");
    m_hLayout22->addWidget(m_MsgFreeLabel);

    m_MsgFreeBtn = new QSelfSwitchButton();
    // m_MsgFreeBtn->setFixedHeight(20);
    m_hLayout22->addWidget(m_MsgFreeBtn);
    m_hLayout22->addSpacing(40);

    m_hLayout23 = new QHBoxLayout();
    m_vLayout2->addLayout(m_hLayout23);
    m_topWndLabel = new QLabel();
    m_topWndLabel->setText("置顶聊天");
    m_hLayout23->addWidget(m_topWndLabel);

    m_topWndBtn = new QSelfSwitchButton();
    m_hLayout23->addWidget(m_topWndBtn);
    m_hLayout23->addSpacing(40);

    m_hLayout24 = new QHBoxLayout();
    m_vLayout2->addLayout(m_hLayout24);

    m_save2AddrBookLabel = new QLabel();
    m_save2AddrBookLabel->setText("保存到通讯录");
    m_hLayout24->addWidget(m_save2AddrBookLabel);

    m_save2AddrBookBtn = new QSelfSwitchButton();
    m_hLayout24->addWidget(m_save2AddrBookBtn);
    m_hLayout24->addSpacing(40);

    m_delexitWnd = new QWidget(this);
    m_delexitWnd->setObjectName("QGroupInfoExitWnd");
    object.m_qssFileName =
        "./stylesheet/" + m_delexitWnd->objectName() + ".qss";
    object.m_widget = m_delexitWnd;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    m_vLayout->addWidget(m_delexitWnd);
    m_vLayout3 = new QVBoxLayout();
    m_delexitWnd->setLayout(m_vLayout3);

    m_delexitBtn = new QPushButton();
    m_delexitBtn->setText("删除并退出");

    m_delexitBtn->setFixedSize(220, 30);
    m_vLayout3->addWidget(m_delexitBtn);

    m_vLayout->addStretch();

    //设置无边框
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    m_scrollArea->setWidget(m_scrollAreaWnd);

    connect(m_groupName2, SIGNAL(saveText()), this, SLOT(slotSetGroupName()));
}

void QGroupInfoWnd::addGroupFriendItem(int64_t ownerId, QString nickName)
{
    // 查看列表中是否存在该用户，如果存在应该不在添加用户信息
    bool ishas = false;
    QListWidget* listWidget = m_groupfriendsWnd->m_listWnd;
    for (int i = 0; i < listWidget->count(); i++)
    {
        QListWidgetItem* item = listWidget->item(i);
        if (item == nullptr)
        {
            continue;
        }

        QGroupFriendsItemWnd* wnd =
            dynamic_cast<QGroupFriendsItemWnd*>(listWidget->itemWidget(item));
        if (wnd == nullptr)
        {
            continue;
        }

        if (wnd->m_friendId == ownerId)
        {
            ishas = true;
            break;
        }
    }

    if (ishas)
    {
        return;
    }

    QGroupFriendsItemWnd* itemWnd = new QGroupFriendsItemWnd();
    itemWnd->m_headImg->setPixmap(QPixmap("./img/groupHead.png"));
    itemWnd->m_friendId = ownerId;
    itemWnd->m_name->setText(nickName);
    m_groupfriendsWnd->addItem(itemWnd);
    itemWnd->requestHeadImg();
}

void QGroupInfoWnd::slotSetGroupName()
{
    LogDebug << "called";
    neb::CJsonObject json;
    json.Add("ownerId", QMainWnd::getMainWnd()->m_userid);
    json.Add("groupId", m_groupId);
    QString groupName = m_groupName2->getText();
    json.Add("groupName", groupName.toStdString());
    QWSClientMgr::getMgr()->request("cs_msg_set_group_name",
                                    json,
                                    [this, groupName](neb::CJsonObject& msg) {
                                        LogDebug << msg.ToString().c_str();
                                        // 更新会话窗口中的标题
                                        emit signalUpdateGroupName(groupName);
                                    });
}

bool QGroupInfoWnd::event(QEvent* event)
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
