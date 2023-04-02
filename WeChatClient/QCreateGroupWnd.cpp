#include <QDebug>
#include <QScrollBar>
#include <QMouseEvent>
#include <QMessageBox>

#include "QSimpleSplit.h"
#include "QCreateGroupWnd.h"
#include "QCreateGroupListItemWithSelBtnWnd.h"
#include "QCreateGroupListItemWnd.h"
#include "QDataManager.h"
#include "QWSClientMgr.h"
#include "QMainWnd.h"

QCreateGroupWnd::QCreateGroupWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    m_vLayout = new QVBoxLayout(this);
    setLayout(m_vLayout);
    setFixedSize(600, 400);
    setWindowFlags(Qt::FramelessWindowHint);

    m_hLayout1 = new QHBoxLayout();
    m_titleLabel = new QLabel(this);
    m_minBtn = new QPushButton(this);
    m_closeBtn = new QPushButton(this);
    m_titleLabel->setText("创建群聊");
    m_minBtn->setIcon(QPixmap("./img/minWnd.png"));
    m_minBtn->setIconSize(QSize(20, 20));
    m_minBtn->setFixedSize(20, 20);
    m_closeBtn->setIcon(QPixmap("./img/closeWnd.png"));
    m_closeBtn->setIconSize(QSize(20, 20));
    m_closeBtn->setFixedSize(20, 20);
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
    m_serchEdit->setPlaceholderText("输入查找关键字`-`");

    m_vLayout1->addWidget(m_serchEdit);
    m_vLayout1->addWidget(m_listWnd1);
    m_hLayout2->addLayout(m_vLayout1);

    QSimpleSplit* sp = new QSimpleSplit(nullptr, QSimpleSplit::Direction_H);
    m_hLayout2->addWidget(sp);

    m_vLayout2 = new QVBoxLayout();
    m_hasSelLabel = new QLabel();
    m_listWnd2 = new QListWidget();
    m_groupNameEdit = new QLineEdit();

    m_hasSelLabel->setText("已选联系人:0");
    m_listWnd2->setFixedHeight(280);

    m_groupNameEdit->setPlaceholderText("请输入群名称`-`");

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

    ///
    // for (int i = 0; i < 5; i++) {
    //	QString str1 = QString("角色%1号").arg(i);
    //	//QString str1 = QString("添加新的朋友%1号")).arg(i);
    //	QString str2 = QString("0000000%1").arg(i);
    //	addListWnd1Item("./img/head2.png", str1.toLocal8Bit().data(),str2.toLocal8Bit().data());
    //}

    connect(m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));
    connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
    connect(m_comfirmBtn, SIGNAL(clicked()), this, SLOT(slot_comfirmBtnClick()));
}

void QCreateGroupWnd::mouseMoveEvent(QMouseEvent* event)
{
    if (m_bPress)
    {
        move(event->pos() - m_poPress + pos());
    }
}

void QCreateGroupWnd::mousePressEvent(QMouseEvent* event)
{
    m_bPress = true;
    m_poPress = event->pos();
}

void QCreateGroupWnd::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    m_bPress = false;
}

void QCreateGroupWnd::addListWnd1Item(const char* headUrl, int64_t friendid, const char* nickname, const char* rolename)
{
    QCreateGroupListItemWithSelBtnWnd* pListwnd1Item = new QCreateGroupListItemWithSelBtnWnd(m_listWnd1, headUrl, friendid, nickname, rolename);
    QListWidgetItem* pListItem = new QListWidgetItem(m_listWnd1);
    pListItem->setSizeHint(QSize(275, 35));
    m_listWnd1->setItemWidget(pListItem, pListwnd1Item);

    connect(pListwnd1Item, SIGNAL(signal_selRBtnClick(QMap<QString, QString>)), this, SLOT(slot_selRBtnClick(QMap<QString, QString>)));
}

void QCreateGroupWnd::addListWnd2Item(const char* headUrl, int64_t friendid, const char* nickname, const char* rolename)
{
    QCreateGroupListItemWnd* pListwnd2Item = new QCreateGroupListItemWnd(m_listWnd2, headUrl, friendid, nickname, rolename);
    QListWidgetItem* pListItem = new QListWidgetItem(m_listWnd2);
    pListItem->setSizeHint(QSize(275, 35));
    m_listWnd2->setItemWidget(pListItem, pListwnd2Item);
}

bool QCreateGroupWnd::hasThisWndByRolename(QString rolename, QListWidget* listWnd)
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
            QCreateGroupListItemWithSelBtnWnd* wnd = dynamic_cast<QCreateGroupListItemWithSelBtnWnd*>(listWnd->itemWidget(item));
            if (wnd->m_roleName->text() == rolename)
            {
                bHas = true;
                break;
            }
        }

        if (listWnd == m_listWnd2)
        {
            QCreateGroupListItemWnd* wnd = dynamic_cast<QCreateGroupListItemWnd*>(listWnd->itemWidget(item));
            if (wnd->m_roleName->text() == rolename)
            {
                bHas = true;
                break;
            }
        }
    }
    return bHas;
}

void QCreateGroupWnd::delThisWndByRolename(QString rolename, QListWidget* listWnd)
{
    for (int i = 0; i < listWnd->count(); i++)
    {
        QListWidgetItem* item = listWnd->item(i);
        if (listWnd == m_listWnd1)
        {
            QCreateGroupListItemWithSelBtnWnd* wnd = dynamic_cast<QCreateGroupListItemWithSelBtnWnd*>(listWnd->itemWidget(item));
            if (wnd->m_roleName->text() == rolename)
            {
                listWnd->takeItem(i);
                break;
            }
        }

        if (listWnd == m_listWnd2)
        {
            QCreateGroupListItemWnd* wnd = dynamic_cast<QCreateGroupListItemWnd*>(listWnd->itemWidget(item));
            if (wnd->m_roleName->text() == rolename)
            {
                // listWnd->removeItemWidget(item);
                listWnd->takeItem(i);
                break;
            }
        }
    }
}

void QCreateGroupWnd::updateData()
{
    //向右侧界面中添加相应数据
    auto itMap = QDataManager::getInstance()->m_FriendId2NameMap.begin();
    auto itMapEnd = QDataManager::getInstance()->m_FriendId2NameMap.end();
    for (; itMap != itMapEnd; itMap++)
    {
        if (hasThisWndByRolename(itMap->second, m_listWnd1) == false)
        {
            addListWnd1Item("./img/head2.png", itMap->first, QString::number(itMap->first).toStdString().c_str(), itMap->second.toStdString().c_str());
        }
    }

    //右侧界面清除选中的状态
    {
        int count = m_listWnd1->count();
        for (int index = 0; index < count; index++)
        {
            QListWidgetItem* item = m_listWnd1->item(index);
            if (item == nullptr)
            {
                continue;
            }

            QCreateGroupListItemWithSelBtnWnd* wnd = dynamic_cast<QCreateGroupListItemWithSelBtnWnd*>(m_listWnd1->itemWidget(item));
            if (wnd == nullptr)
            {
                continue;
            }

            //清除选中的状态
            wnd->m_selRBtn->setChecked(false);
        }
    }

    //移除左侧界面中的内容
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

void QCreateGroupWnd::closeWnd()
{
    hide();
}

void QCreateGroupWnd::minWnd()
{
    showMinimized();
}

void QCreateGroupWnd::slot_selRBtnClick(QMap<QString, QString> map)
{
    // qDebug() << "map" << map;
    if (map["isSel"] == "true")
    {
        if (hasThisWndByRolename(map["rolename"], m_listWnd2) == false)
        {
            addListWnd2Item(map["headimg"].toLocal8Bit().data(),
                            map["friendid"].toInt(),
                            map["nickname"].toLocal8Bit().data(),
                            map["rolename"].toLocal8Bit().data());
        }
    }
    else
    {
        delThisWndByRolename(map["rolename"], m_listWnd2);
    }

    //获取m_listwnd2中含有的项目数量，修改lable中的内容
    QString str = QString("已选联系人:%1").arg(m_listWnd2->count());
    m_hasSelLabel->setText(str);
}

void QCreateGroupWnd::slot_comfirmBtnClick()
{
    //先直接隐藏创建群聊的对话框
    hide();

    //获取当前玩家id，获取群的名称
    int ownerid = QDataManager::getInstance()->m_userid;
    QString groupname = m_groupNameEdit->text();
    // qDebug() << "ownerid:" << ownerid << "groupname:" << groupname;
    if (groupname == "")
    {
        // qDebug() << "groupname is empty()";
        QMessageBox::information(nullptr, "info", "请输入正确的群组名称");
        return;
    }
    //获取群的成员信息
    std::vector<int64_t> groupfriendIdVct;
    for (int i = 0; i < m_listWnd2->count(); i++)
    {
        QListWidgetItem* item = m_listWnd2->item(i);
        if (item == NULL)
        {
            continue;
        }

        QCreateGroupListItemWnd* wnd = dynamic_cast<QCreateGroupListItemWnd*>(m_listWnd2->itemWidget(item));
        if (wnd == NULL)
        {
            continue;
        }
        qDebug() << "groupfrindIdVct add " << wnd->m_friendid;
        groupfriendIdVct.push_back(wnd->m_friendid);
    }
    //填充数据
    neb::CJsonObject json;
    json.Add("createid", ownerid);
    json.Add("groupname", groupname.toStdString());
    json.AddEmptySubArray("groupfriends");
    for (int i = 0; i < groupfriendIdVct.size(); i++)
    {
        json["groupfriends"].Add(groupfriendIdVct[i]);
    }
    //向远端服务器发送请求
    QWSClientMgr::getInstance()->request("cs_msg_create_group", json, [this, groupname](neb::CJsonObject& msg) {
        qDebug() << "msg:" << msg.ToString().c_str();
        //现在服务端仅仅返回 createid，groupname,groupid,groupfriends
        int groupid = -1;
        if (!msg["data"].Get("groupid", groupid))
        {
            qDebug() << "msg[\"data\"] can not find groupid!";
            return;
        }
        //接收到创建群组成功的时候，向列表中嵌入一条数据
        QMainWnd::getInstance()->m_commGroupsListWnd->addGroupItem("./img/groupHead.png", groupname.toStdString().c_str(), groupid);
    });
}
