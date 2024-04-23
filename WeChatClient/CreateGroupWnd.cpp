#include <QDebug>
#include <QScrollBar>
#include <QMouseEvent>
#include <QMessageBox>

#include "SelfSplit.h"
#include "CreateGroupWnd.h"
#include "CreateGroupListItemWithSelBtnWnd.h"
#include "CreateGroupListItemWnd.h"
#include "DataManager.h"
#include "WSClientMgr.h"
#include "MainWnd.h"
#include "StyleSheetMgr.h"

CreateGroupWnd::CreateGroupWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    LogFunc;
    // setObjectName("QCreateGroupWnd");

    m_centerWnd = new QWidget(this);
    // m_centerWnd->setFixedSize(460, 360);
    m_centerWnd->setObjectName("QCreateGroupWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + m_centerWnd->objectName() + ".qss";
    object.m_widget = m_centerWnd;
    StyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    m_vLayout = new QVBoxLayout(m_centerWnd);
    m_vLayout->setContentsMargins(10, 10, 10, 10);

    m_centerWnd->setLayout(m_vLayout);
    m_centerWnd->setFixedSize(640, 420);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    m_titleLabel = new QLabel(m_centerWnd);
    m_minBtn = new QPushButton(m_centerWnd);
    m_closeBtn = new QPushButton(m_centerWnd);

    m_titleLabel->setText("创建群聊");
    m_minBtn->setIcon(QPixmap("./img/minBtn_.png"));
    m_minBtn->setIconSize(QSize(20, 20));
    m_minBtn->setFixedSize(20, 20);
    // m_minBtn->setStyleSheet("border:0px;");

    m_closeBtn->setIcon(QPixmap("./img/closeBtn_.png"));
    m_closeBtn->setIconSize(QSize(20, 20));
    m_closeBtn->setFixedSize(20, 20);
    //   m_closeBtn->setStyleSheet("border:0px");

    // 标题栏
    m_hLayout1 = new QHBoxLayout();
    m_hLayout1->addWidget(m_titleLabel);
    m_hLayout1->addStretch();
    m_hLayout1->addWidget(m_minBtn);
    m_hLayout1->addWidget(m_closeBtn);

    m_vLayout->addLayout(m_hLayout1);

    m_hLayout2 = new QHBoxLayout();
    m_vLayout1 = new QVBoxLayout();
    m_serchEdit = new QLineEdit();
    m_listWnd1 = new QListWidget();

    m_listWnd1->setContentsMargins(0, 0, 0, 0);
    m_listWnd1->setFixedHeight(300);

    m_serchEdit->setFixedWidth(280);
    m_serchEdit->setPlaceholderText("输入查找关键字:");

    m_vLayout1->addWidget(m_serchEdit);
    m_vLayout1->addWidget(m_listWnd1);
    m_hLayout2->addLayout(m_vLayout1);

    SelfSplit* sp = new SelfSplit(nullptr, SelfSplit::Direction_H);
    m_hLayout2->addWidget(sp);

    m_vLayout2 = new QVBoxLayout();
    m_hasSelLabel = new QLabel();
    m_listWnd2 = new QListWidget();
    m_groupNameEdit = new QLineEdit();

    m_hasSelLabel->setText("已选联系人:0");
    m_listWnd2->setFixedHeight(280);

    m_groupNameEdit->setPlaceholderText("请输入群名称:");

    m_hLayout3 = new QHBoxLayout();
    m_comfirmBtn = new QPushButton();

    m_comfirmBtn->setText("确认");

    m_hLayout3->addStretch();
    m_hLayout3->addWidget(m_comfirmBtn);

    m_vLayout2->addWidget(m_hasSelLabel);
    m_vLayout2->addWidget(m_listWnd2);
    m_vLayout2->addStretch();
    m_vLayout2->addWidget(m_groupNameEdit);
    m_vLayout2->addLayout(m_hLayout3);

    m_hLayout2->addLayout(m_vLayout2);
    m_vLayout->addLayout(m_hLayout2);

    connect(m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));
    connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
    connect(m_comfirmBtn, SIGNAL(clicked()), this, SLOT(slotComfirmBtnClick()));
}

void CreateGroupWnd::mouseMoveEvent(QMouseEvent* event)
{
    if (m_bPress)
    {
        move(event->pos() - m_poPress + pos());
    }
}

void CreateGroupWnd::mousePressEvent(QMouseEvent* event)
{
    m_bPress = true;
    m_poPress = event->pos();
}

void CreateGroupWnd::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    m_bPress = false;
}

void CreateGroupWnd::addListWnd1Item(const char* headUrl, int64_t friendid, const char* nickname, const char* rolename)
{
    CreateGroupListItemWithSelBtnWnd* pListwnd1Item = new CreateGroupListItemWithSelBtnWnd(m_listWnd1, headUrl, friendid, nickname, rolename);
    QListWidgetItem* pListItem = new QListWidgetItem(m_listWnd1);
    pListItem->setSizeHint(QSize(275, 35));
    m_listWnd1->setItemWidget(pListItem, pListwnd1Item);

    connect(pListwnd1Item, SIGNAL(signalSelRBtnClick(QMap<QString, QString>)), this, SLOT(slotSelRBtnClick(QMap<QString, QString>)));
}

void CreateGroupWnd::addListWnd2Item(const char* headUrl, int64_t friendid, const char* nickname, const char* rolename)
{
    CreateGroupListItemWnd* pListwnd2Item = new CreateGroupListItemWnd(m_listWnd2, headUrl, friendid, nickname, rolename);
    QListWidgetItem* pListItem = new QListWidgetItem(m_listWnd2);
    pListItem->setSizeHint(QSize(275, 35));
    m_listWnd2->setItemWidget(pListItem, pListwnd2Item);
}

bool CreateGroupWnd::hasThisWndByRolename(QString rolename, QListWidget* listWnd)
{
    bool bHas = false;
    if (listWnd == nullptr)
    {
        return bHas;
    }

    for (int i = 0; i < listWnd->count(); i++)
    {
        QListWidgetItem* item = listWnd->item(i);
        if (listWnd == m_listWnd1)
        {
            CreateGroupListItemWithSelBtnWnd* wnd = dynamic_cast<CreateGroupListItemWithSelBtnWnd*>(listWnd->itemWidget(item));
            if (wnd->m_roleName->text() == rolename)
            {
                bHas = true;
                break;
            }
        }

        if (listWnd == m_listWnd2)
        {
            CreateGroupListItemWnd* wnd = dynamic_cast<CreateGroupListItemWnd*>(listWnd->itemWidget(item));
            if (wnd->m_roleName->text() == rolename)
            {
                bHas = true;
                break;
            }
        }
    }
    return bHas;
}

void CreateGroupWnd::delThisWndByRolename(QString rolename, QListWidget* listWnd)
{
    for (int i = 0; i < listWnd->count(); i++)
    {
        QListWidgetItem* item = listWnd->item(i);
        if (listWnd == m_listWnd1)
        {
            CreateGroupListItemWithSelBtnWnd* wnd = dynamic_cast<CreateGroupListItemWithSelBtnWnd*>(listWnd->itemWidget(item));
            if (wnd->m_roleName->text() == rolename)
            {
                listWnd->takeItem(i);
                break;
            }
        }

        if (listWnd == m_listWnd2)
        {
            CreateGroupListItemWnd* wnd = dynamic_cast<CreateGroupListItemWnd*>(listWnd->itemWidget(item));
            if (wnd->m_roleName->text() == rolename)
            {
                // listWnd->removeItemWidget(item);
                listWnd->takeItem(i);
                break;
            }
        }
    }
}

void CreateGroupWnd::updateData()
{
    // 向右侧界面中添加相应数据
    auto itMap = DataManager::getMgr()->m_FriendId2NameMap.begin();
    auto itMapEnd = DataManager::getMgr()->m_FriendId2NameMap.end();
    for (; itMap != itMapEnd; itMap++)
    {
        if (hasThisWndByRolename(itMap->second, m_listWnd1) == false)
        {
            addListWnd1Item("./img/head2.png", itMap->first, QString::number(itMap->first).toStdString().c_str(), itMap->second.toStdString().c_str());
        }
    }

    // 右侧界面清除选中的状态
    {
        int count = m_listWnd1->count();
        for (int index = 0; index < count; index++)
        {
            QListWidgetItem* item = m_listWnd1->item(index);
            if (item == nullptr)
            {
                continue;
            }

            CreateGroupListItemWithSelBtnWnd* wnd = dynamic_cast<CreateGroupListItemWithSelBtnWnd*>(m_listWnd1->itemWidget(item));
            if (wnd == nullptr)
            {
                continue;
            }

            // 清除选中的状态
            wnd->m_selRBtn->setChecked(false);
        }
    }

    // 移除左侧界面中的内容
    {
        QListWidgetItem* item;
        int count = m_listWnd2->count();
        for (int index = 0; index < count; index++)
        {
            item = m_listWnd2->takeItem(0);
            delete item;
        }
    }

    m_hasSelLabel->setText("已选联系人:0");
    m_serchEdit->setPlaceholderText("输入查找关键字`-`");
    m_groupNameEdit->setPlaceholderText("请输入群名称`-`");
    m_serchEdit->setText("");
    m_groupNameEdit->setText("");
}

void CreateGroupWnd::closeWnd()
{
    hide();
}

void CreateGroupWnd::minWnd()
{
    showMinimized();
}

void CreateGroupWnd::slotSelRBtnClick(QMap<QString, QString> map)
{
    // LogDebug << "map" << map;
    if (map["isSel"] == "true")
    {
        if (hasThisWndByRolename(map["rolename"], m_listWnd2) == false)
        {
            addListWnd2Item(map["headimg"].toLocal8Bit().data(), map["friendid"].toInt(), map["nickname"].toLocal8Bit().data(), map["rolename"].toLocal8Bit().data());
        }
    }
    else
    {
        delThisWndByRolename(map["rolename"], m_listWnd2);
    }

    // 获取m_listwnd2中含有的项目数量，修改lable中的内容
    QString str = QString("已选联系人:%1").arg(m_listWnd2->count());
    m_hasSelLabel->setText(str);
}

void CreateGroupWnd::slotComfirmBtnClick()
{
    // 先直接隐藏创建群聊的对话框
    hide();

    // 获取当前玩家id，获取群的名称
    int ownerid = DataManager::getMgr()->m_userid;
    QString groupname = m_groupNameEdit->text();
    // LogDebug << "ownerid:" << ownerid << "groupname:" << groupname;
    if (groupname == "")
    {
        // LogDebug << "groupname is empty()";
        QMessageBox::information(nullptr, "info", "请输入正确的群组名称");
        return;
    }
    // 获取群的成员信息
    std::vector<int64_t> groupfriendIdVct;
    for (int i = 0; i < m_listWnd2->count(); i++)
    {
        QListWidgetItem* item = m_listWnd2->item(i);
        if (item == NULL)
        {
            continue;
        }

        CreateGroupListItemWnd* wnd = dynamic_cast<CreateGroupListItemWnd*>(m_listWnd2->itemWidget(item));
        if (wnd == NULL)
        {
            continue;
        }
        LogDebug << "groupfrindIdVct add " << wnd->m_friendid;
        groupfriendIdVct.push_back(wnd->m_friendid);
    }
    // 填充数据
    neb::CJsonObject json;
    json.Add("createid", ownerid);
    json.Add("groupname", groupname.toStdString());
    json.AddEmptySubArray("groupfriends");
    for (int i = 0; i < groupfriendIdVct.size(); i++)
    {
        json["groupfriends"].Add(groupfriendIdVct[i]);
    }
    // 向远端服务器发送请求
    WSClientMgr::getMgr()->request("cs_msg_create_group", json, [this, groupname](neb::CJsonObject& msg) {
        LogDebug << "msg:" << msg.ToString().c_str();
        // 现在服务端仅仅返回 createid，groupname,groupid,groupfriends
        int groupid = -1;
        if (!msg["data"].Get("groupid", groupid))
        {
            LogDebug << "msg[\"data\"] can not find groupid!";
            return;
        }
        // 接收到创建群组成功的时候，向列表中嵌入一条数据
        MainWnd::getMainWnd()->m_commGroupsListWnd->addGroupItem("./img/groupHead.png", groupname.toStdString().c_str(), groupid);
    });
}
