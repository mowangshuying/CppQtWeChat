#include "MainWnd.h"
#include "SelfSplit.h"
#include "json/CJsonObject.hpp"
#include "NetClientUtils.h"
#include <QApplication>
#include "CommMsgItemWnd.h"
#include "CommContactItemWnd.h"
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include "ChatMsgWnd.h"
#include "DataManager.h"
#include "ChatFileOuterWnd.h"
#include "CommGroupItemWnd.h"
#include <QPainter>
#include <cstdlib>
#include <qmath.h>
#include <QGraphicsDropShadowEffect>
#include "StyleSheetMgr.h"
#include "CustomListWidgetItem.h"

MainWnd* MainWnd::m_mainWnd = nullptr;

MainWnd::MainWnd(QWidget* p /*= nullptr*/) : FramelessWidget(p)
{
    LogFunc;
    m_centerWnd = this;
    //m_centerWnd->setObjectName("QMainWnd");
    //QString qssFileName = "./stylesheet/" + m_centerWnd->objectName() + ".qss";
    //StyleSheetMgr::getMgr()->reg(m_centerWnd->objectName(), qssFileName, m_centerWnd);

    m_hLayout = new QHBoxLayout(m_centerWnd);
    m_centerWnd->setLayout(m_hLayout);

    // 左侧工具栏部分 ----------------------------
    m_toolWnd = new ToolWnd(m_centerWnd);

    // 中间部分 ----------------------------------
    m_commMsgListWnd = new CommListWnd(m_centerWnd, CommListWnd::MsgItemWndTpye);
    m_commContactsListWnd = new CommListWnd(m_centerWnd, CommListWnd::ContactItemWndType);
    m_commGroupsListWnd = new CommListWnd(m_centerWnd, CommListWnd::GroupItemWndType);
    m_commSearchListWnd = new CommListWnd(m_centerWnd, CommListWnd::SearchItemWndType);

    m_hLayout->setContentsMargins(0, 0, 0, 0);
    m_hLayout->setSpacing(0);
    m_sMiddleLayout = new QStackedLayout(m_centerWnd);
    m_sMiddleLayout->addWidget(m_commMsgListWnd);
    m_sMiddleLayout->addWidget(m_commContactsListWnd);
    m_sMiddleLayout->addWidget(m_commGroupsListWnd);
    m_sMiddleLayout->addWidget(m_commSearchListWnd);
    m_sMiddleLayout->setContentsMargins(0, 0, 0, 0);

    m_hLayout->setSpacing(0);

    // 右边部分 ------------------------------------
    m_sRightLayout = new QStackedLayout(m_centerWnd);
    m_sRightLayout->setContentsMargins(0, 0, 0, 0);

    // 联系人信息窗口
    m_commContactInfo = new CommContactInfoWnd(m_centerWnd);
    m_sRightLayout->addWidget(m_commContactInfo);
    connect(m_commContactInfo->m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
    connect(m_commContactInfo->m_maxBtn, SIGNAL(clicked()), this, SLOT(maxWnd()));
    connect(m_commContactInfo->m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));

    // 新的朋友
    m_dealNewFriendsApplyWnd = new DealNewFriendsApplyWnd(m_centerWnd);
    connect(m_dealNewFriendsApplyWnd->m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
    connect(m_dealNewFriendsApplyWnd->m_maxBtn, SIGNAL(clicked()), this, SLOT(maxWnd()));
    connect(m_dealNewFriendsApplyWnd->m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));
    m_sRightLayout->addWidget(m_dealNewFriendsApplyWnd);
    m_commContactsListWnd->addContactsItem("./img/head2.png", "新的朋友", true);

    m_hLayout->addWidget(m_toolWnd);
    m_hLayout->addLayout(m_sMiddleLayout, 0);

    {
        /*添加分割线的示例代码*/
        SelfSplit* sp = new SelfSplit(m_centerWnd, SelfSplit::Direction_V);
        m_hLayout->addWidget(sp);
    }

    m_hLayout->setSpacing(0);
    m_hLayout->addLayout(m_sRightLayout, 1);
    m_hLayout->addStretch();

    setLayout(m_hLayout);
    connect(m_toolWnd, SIGNAL(signalToolWndPageChanged(int)), this, SLOT(slotToolWndPageChanged(int)));
    connect(m_commMsgListWnd, SIGNAL(signalCommListChanged(int)), this, SLOT(slotSesIdToIndex(int)));
    connect(m_commContactsListWnd, SIGNAL(signalContactInfoChange(QMap<QString, QString>)), m_commContactInfo, SLOT(slotContactInfoChange(QMap<QString, QString>)));
    connect(m_commContactInfo, SIGNAL(signalSendMsgBtnClick(QMap<QString, QString>)), this, SLOT(slotSendMsgBtnClick(QMap<QString, QString>)));

    connect(m_commMsgListWnd, SIGNAL(signalSearchText(QString)), this, SLOT(slotSearchText(QString)));
    connect(m_commContactsListWnd, SIGNAL(signalSearchText(QString)), this, SLOT(slotSearchText(QString)));
    connect(m_commGroupsListWnd, SIGNAL(signalSearchText(QString)), this, SLOT(slotSearchText(QString)));
    connect(m_commSearchListWnd, SIGNAL(signalSearchText(QString)), this, SLOT(slotSearchText(QString)));

    connect(m_commSearchListWnd, SIGNAL(signalCommListChanged(int)), this, SLOT(slotSesIdToIndex(int)));

    NetClientUtils::getUtils()->regMsgCall("cs_msg_sendmsg", std::bind(&MainWnd::cs_msg_sendmsg, this, std::placeholders::_1));
    NetClientUtils::getUtils()->regMsgCall("cs_msg_sendgroupmsg", std::bind(&MainWnd::cs_msg_sendgroupmsg, this, std::placeholders::_1));
    NetClientUtils::getUtils()->regMsgCall("cs_msg_update_sessionlist", std::bind(&MainWnd::cs_msg_update_sessionlist, this, std::placeholders::_1));
    NetClientUtils::getUtils()->regMsgCall("cs_msg_update_grouplist", std::bind(&MainWnd::cs_msg_update_grouplist, this, std::placeholders::_1));
    NetClientUtils::getUtils()->regMsgCall("cs_msg_update_friendlist", std::bind(&MainWnd::cs_msg_update_friendlist, this, std::placeholders::_1));

    m_networkMgr = new QNetworkAccessManager(this);
    connect(m_networkMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotReplyFinished(QNetworkReply*)));

    makeSystemTray();

    setMinimumSize(1000, 800);
    setMouseTracking(true);

    m_settingWnd = new SettingWnd();
    m_settingWnd->hide();
    connect(m_toolWnd->m_selectMoreWnd->m_settingBtn, SIGNAL(clicked()), this, SLOT(slotOnSettingBtnClick()));
}

void MainWnd::makeSystemTray()
{
    // 系统托盘功能 -----------------------------------------
    m_systemTrayIcon = new QSystemTrayIcon(this);
    m_systemTrayIcon->setIcon(QIcon("./img/wechat.ico"));
    m_systemTrayIcon->setToolTip("QT版微信v2.0.0");
    m_systemTrayIcon->show();

    m_systemTrayIconMenu = new QMenu(this);
    m_systemTrayIconShowMainWndAction = new QAction(this);
    m_systemTrayIconShowMainWndAction->setText("打开主面板");
    m_systemTrayIconMenu->addAction(m_systemTrayIconShowMainWndAction);

    m_systemTrayIconExitAction = new QAction(this);
    m_systemTrayIconExitAction->setText("退出");
    m_systemTrayIconMenu->addAction(m_systemTrayIconExitAction);
    m_systemTrayIcon->setContextMenu(m_systemTrayIconMenu);

    connect(m_systemTrayIconExitAction, &QAction::triggered, this, &MainWnd::closeWnd);
    connect(m_systemTrayIconShowMainWndAction, &QAction::triggered, this, &MainWnd::showNormalWnd);
    connect(m_systemTrayIcon, &QSystemTrayIcon::activated, this, &MainWnd::slotOnSystemTrayIconClick);
}

MainWnd* MainWnd::getMainWnd()
{
    if (m_mainWnd == nullptr)
    {
        m_mainWnd = new MainWnd();
    }
    return m_mainWnd;
}

MainWnd::~MainWnd()
{
    qDebug() << "~QMainWnd()";
    // m_systemTrayIcon->hide();
    // delete m_systemTrayIcon;
}

void MainWnd::cs_msg_sendmsg(neb::CJsonObject& msg)
{
    int sesid = -1;
    if (!msg["data"].Get("sesid", sesid))
    {
        return;
    }

    int msgtype = -1;
    if (!msg["data"].Get("msgtype", msgtype))
    {
        return;
    }

    std::string msgtext = "";
    if (!msg["data"].Get("msgtext", msgtext))
    {
        return;
    }

    int64_t sendid = -1;
    if (!msg["data"].Get("sendid", sendid))
    {
        return;
    }

    int64_t recvid = -1;
    if (!msg["data"].Get("sendid", recvid))
    {
        return;
    }

    // 查找对应的会话
    SessionWnd* ses = nullptr;
    int count = m_sRightLayout->count();
    for (int i = 0; i < count; i++)
    {
        if (i <= 1)
        {
            continue;
        }

        QLayoutItem* item = m_sRightLayout->layout()->itemAt(i);
        SessionWnd* sesWnd = dynamic_cast<SessionWnd*>(item->widget());
        if (sesWnd->m_sesId == sesid)
        {
            ses = sesWnd;
            break;
        }
    }

    if (ses == nullptr)
    {
        LogD << "can not find ses sesId:" << sesid;
        return;
    }

    // 发送消息
    if (msgtype == 0)
    {
        ses->dealMsgTime();

        // 向会话中嵌入一条数据；
        QString time = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
        ChatMsgWnd* msgWnd = new ChatMsgWnd(ses->m_MsgWndList, sendid, MainWnd::getMainWnd()->m_username, recvid);
        QListWidgetItem* msgWndItem = new QListWidgetItem(ses->m_MsgWndList);
        msgWnd->setFixedWidth(ses->width());

        QSize msgSize = msgWnd->fontRect(msgtext.c_str());
        msgWndItem->setSizeHint(msgSize);
        // 会设置消息并调用相应的
        msgWnd->setText(msgtext.c_str(), time, msgSize, ChatMsgWnd::ChatMsg_OtherMsgText);
        // 关联项与窗口
        ses->m_MsgWndList->setItemWidget(msgWndItem, msgWnd);
        return;
    }

    // 发送文件
    if (msgtype == 1)
    {
        ses->dealMsgTime();

        neb::CJsonObject json;
        if (!json.Parse(msgtext))
        {
            return;
        }

        std::string filename_server = "";
        std::string filename_client = "";
        std::string filesize = "";

        if (!json.Get("filename_server", filename_server))
        {
            return;
        }

        if (!json.Get("filename_client", filename_client))
        {
            return;
        }

        if (!json.Get("filesize", filesize))
        {
            return;
        }

        neb::CJsonObject fsJson;
        if (!fsJson.Parse(filename_server))
        {
            return;
        }

        std::string fsStr;
        if (!fsJson.Get("headimg", fsStr))
        {
            return;
        }

        ////接收端是一个文件
        ChatFileOuterWnd* fileWnd = new ChatFileOuterWnd(nullptr, sendid, recvid);
        fileWnd->m_innerWnd->m_fileName->setText(filename_client.c_str());
        fileWnd->m_innerWnd->m_fileSize->setText(filesize.c_str());
        fileWnd->m_innerWnd->m_sendState->setText("等待下载");
        fileWnd->m_innerWnd->m_serveFilePath = fsStr.c_str();
        fileWnd->m_innerWnd->recvFileShow();
        fileWnd->setFixedWidth(ses->m_MsgWndList->width());
        QListWidgetItem* fileItem = new QListWidgetItem(ses->m_MsgWndList);
        QSize fileWndSize(fileWnd->width(), 100 + 20);
        fileItem->setSizeHint(fileWndSize);
        ses->m_MsgWndList->setItemWidget(fileItem, fileWnd);
    }

    ses->m_MsgWndList->scrollToBottom();
}

void MainWnd::cs_msg_sendgroupmsg(neb::CJsonObject& msg)
{
    // 首先获取对应的会话id；
    int sesid = -1;
    if (!msg["data"].Get("sesid", sesid))
    {
        return;
    }

    int msgtype = -1;
    if (!msg["data"].Get("msgtype", msgtype))
    {
        return;
    }

    std::string msgtext = "";
    if (!msg["data"].Get("msgtext", msgtext))
    {
        return;
    }

    int64_t sendid = -1;
    if (!msg["data"].Get("sendid", sendid))
    {
        return;
    }

    int64_t recvid = -1;
    if (!msg["data"].Get("sendid", recvid))
    {
        return;
    }

    std::string sendUserName;
    if (!msg["data"].Get("sendUserName", sendUserName))
    {
        return;
    }

    // 查找对应的会话
    SessionWnd* ses = nullptr;
    int count = m_sRightLayout->count();
    for (int i = 0; i < count; i++)
    {
        if (i <= 1)
        {
            continue;
        }

        QLayoutItem* item = m_sRightLayout->layout()->itemAt(i);
        SessionWnd* sesWnd = dynamic_cast<SessionWnd*>(item->widget());
        if (sesWnd->m_sesId == sesid)
        {
            ses = sesWnd;
            break;
        }
    }

    if (ses != nullptr)
    {
        if (msgtype == 0)
        {
            // 向会话中嵌入一条数据；
            QString time = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
            ChatMsgWnd* msgWnd = new ChatMsgWnd(ses->m_MsgWndList, sendid, sendUserName.c_str(), recvid);
            QListWidgetItem* msgItem = new QListWidgetItem(ses->m_MsgWndList);
            msgWnd->setFixedWidth(640);
            QSize msgSize = msgWnd->fontRect(msgtext.c_str());

            //

            msgItem->setSizeHint(msgSize);
            // 会设置消息并调用相应的
            msgWnd->setText(msgtext.c_str(), time, msgSize, ChatMsgWnd::ChatMsg_OtherMsgText);
            // 关联项与窗口
            ses->m_MsgWndList->setItemWidget(msgItem, msgWnd);
        }

        if (msgtype == 1)
        {
            neb::CJsonObject json;
            if (!json.Parse(msgtext))
            {
                return;
            }

            std::string filename_server = "";
            std::string filename_client = "";
            std::string filesize = "";

            if (!json.Get("filename_server", filename_server))
            {
                return;
            }

            if (!json.Get("filename_client", filename_client))
            {
                return;
            }

            if (!json.Get("filesize", filesize))
            {
                return;
            }

            neb::CJsonObject fsJson;
            if (!fsJson.Parse(filename_server))
            {
                return;
            }

            std::string fsStr;
            if (!json.Get("img", fsStr))
            {
                return;
            }

            ////接收端是一个文件
            ChatFileOuterWnd* fileWnd = new ChatFileOuterWnd(nullptr, sendid, recvid);
            fileWnd->setFileDownLoadData(filename_client.c_str(), filesize.c_str(), "等待下载", fsStr.c_str());
            fileWnd->setFixedWidth(ses->m_MsgWndList->width());
            QListWidgetItem* fileItem = new QListWidgetItem(ses->m_MsgWndList);
            QSize fileWndSize(fileWnd->width(), 100 + 20);
            fileItem->setSizeHint(fileWndSize);
            ses->m_MsgWndList->setItemWidget(fileItem, fileWnd);
        }

        ses->m_MsgWndList->scrollToBottom();
    }
}

void MainWnd::cs_msg_update_sessionlist(neb::CJsonObject& msg)
{
    requestSessionList();
}

void MainWnd::cs_msg_update_grouplist(neb::CJsonObject& msg)
{
    requestGroupList();
}

void MainWnd::cs_msg_update_friendlist(neb::CJsonObject& msg)
{
    requestFriendList();
}

void MainWnd::requestHeadImg()
{
    QString imgurl = QString("http://49.232.169.205:80/UploadDemo/img/u%1.png").arg(m_userid);
    m_networkMgr->get(QNetworkRequest(QUrl(imgurl)));
}

void MainWnd::requestFriendList()
{
    //
    neb::CJsonObject json;
    json.Add("ownerid", MainWnd::getMainWnd()->m_userid);
    NetClientUtils::getUtils()->request("cs_msg_get_friendslist", json, [this](neb::CJsonObject& msg) {
        if (!msg["data"].IsArray())
        {
            return;
        }

        for (int i = 0; i < msg["data"].GetArraySize(); i++)
        {
            neb::CJsonObject json1;
            if (!msg["data"].Get(i, json1))
            {
                continue;
            }

            int friendid = -1;
            if (!json1.Get("friendid", friendid))
            {
                continue;
            }

            std::string friendusername = "";
            if (!json1.Get("friendusername", friendusername))
            {
                continue;
            }

            m_commContactsListWnd->addContactsItem("./img/head2.png", friendusername.c_str(), false, friendid);
        }
    });
}

void MainWnd::requestSessionList()
{
    // 向远端请求会话列表
    neb::CJsonObject json;
    json.Add("ownerid", MainWnd::getMainWnd()->getMainWnd()->m_userid);
    NetClientUtils::getUtils()->request("cs_msg_get_sessionlist", json, [this](neb::CJsonObject& msg) {
        // QMessageBox::information(nullptr, "info",
        // msg.ToString().c_str());
        LogD << "msg:" << msg.ToString().c_str();
        // 向会话列表中添加一些数据
        if (!msg["data"].IsArray())
        {
            return;
        }

        for (int i = 0; i < msg["data"].GetArraySize(); i++)
        {
            neb::CJsonObject json1;
            if (!msg["data"].Get(i, json1))
            {
                continue;
            }

            int sesid = -1;
            if (!json1.Get("id", sesid))
            {
                continue;
            }

            std::string recentlymsg;
            if (!json1.Get("recentlymsg", recentlymsg))
            {
                continue;
            }

            std::string recvusername;
            if (!json1.Get("recvusername", recvusername))
            {
                continue;
            }

            int64_t sendid = -1;
            int64_t recvid = -1;

            //
            if (!json1.Get("sendid", sendid))
            {
                continue;
            }

            if (!json1.Get("recvid", recvid))
            {
                continue;
            }

            int isgroupses = -1;
            if (!json1.Get("isgroupses", isgroupses))
            {
                continue;
            }

            bool isGroup = false;
            if (isgroupses == 1)
            {
                isGroup = true;
            }

            m_commMsgListWnd->addMsgItem(recvusername.c_str(), recentlymsg.c_str(), sesid, recvid, isGroup);

            if (!hasSessionWndBySessionId(sesid))
            {
                SessionWnd* sesWnd = new SessionWnd();
                sesWnd->setTitle(recvusername.c_str());
                sesWnd->m_sesId = sesid;
                sesWnd->m_recvId = recvid;
                sesWnd->m_isGroupSes = isgroupses;

                // 不是群组会话，应该隐藏more按钮
                if (!isgroupses)
                {
                    sesWnd->m_sesTopWnd->m_moreBtn->hide();
                }
                else
                {
                    sesWnd->m_groupInfoWnd->setGroupName(recvusername.c_str());
                    sesWnd->m_groupInfoWnd->setGroupId(recvid);
                }

                connect(sesWnd->m_sesTopWnd->m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
                connect(sesWnd->m_sesTopWnd->m_maxBtn, SIGNAL(clicked()), this, SLOT(maxWnd()));
                connect(sesWnd->m_sesTopWnd->m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));
                m_sRightLayout->addWidget(sesWnd);
            }
        }
    });
}

bool MainWnd::hasSessionWndBySessionId(int sesid)
{
    bool bHas = false;
    int count = m_sRightLayout->count();
    for (int i = 0; i < count; i++)
    {
        if (i <= 1)
        {
            continue;
        }

        QLayoutItem* item = m_sRightLayout->layout()->itemAt(i);
        SessionWnd* sesWnd = dynamic_cast<SessionWnd*>(item->widget());
        if (sesWnd->m_sesId == sesid)
        {
            bHas = true;
            break;
        }
    }
    return bHas;
}

void MainWnd::requestGroupList()
{
    neb::CJsonObject json;
    json.Add("ownerid", MainWnd::getMainWnd()->getMainWnd()->m_userid);
    NetClientUtils::getUtils()->request("cs_msg_get_groupList", json, [this](neb::CJsonObject& msg) {
        LogD << "requestGroupList:" << msg.ToString().c_str();
        // 先判断传入是否是msg["data"]是否是array
        if (!msg["data"].IsArray())
        {
            return;
        }

        for (int i = 0; i < msg["data"].GetArraySize(); i++)
        {
            //
            neb::CJsonObject tempJson;
            if (!msg["data"].Get(i, tempJson))
            {
                continue;
            }

            // 获取groupname，获取群组的groupid
            //  groupname
            std::string groupname = "";
            if (!tempJson.Get("groupname", groupname))
            {
                continue;
            }

            // 获取到groupid
            int groupid = -1;
            if (!tempJson.Get("groupid", groupid))
            {
                continue;
            }

            m_commGroupsListWnd->addGroupItem("./img/groupHead.png", groupname.c_str(), groupid);
        }
    });
}

void MainWnd::closeWnd()
{
    m_systemTrayIcon->hide();
    qApp->quit();
}

void MainWnd::minWnd()
{
    showMinimized();
}

void MainWnd::maxWnd()
{
    if (windowState() == Qt::MaximumSize)
    {
        showNormal();
    }
    else
    {
        showMaximized();
    }
}

void MainWnd::showNormalWnd()
{
    if (windowState() == Qt::WindowMinimized)
    {
        showNormal();
    }
}

void MainWnd::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_bLeftBtnPress)
        return;

    move(event->pos() - m_leftBtnPressPoint + pos());
}

void MainWnd::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    // 鼠标左键按下
    m_bLeftBtnPress = true;
    m_leftBtnPressPoint = event->pos();
}

void MainWnd::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    m_bLeftBtnPress = false;
}

void MainWnd::slotSesIdToIndex(int sesid)
{
    int layoutId = 0;
    if (sesid != 0)
    {
        int count = m_sRightLayout->count();
        for (int i = 0; i < count; i++)
        {
            if (i <= 1)
            {
                continue;
            }

            QLayoutItem* item = m_sRightLayout->layout()->itemAt(i);
            SessionWnd* sesWnd = dynamic_cast<SessionWnd*>(item->widget());
            if (sesWnd->m_sesId == sesid)
            {
                layoutId = i;
                break;
            }
        }
    }

    if (sesid != 0)
    {
        m_lastSesId = layoutId;
    }

    m_sRightLayout->setCurrentIndex(layoutId);
}

void MainWnd::slotSendMsgBtnClick(QMap<QString, QString> infoMap)
{
    bool has = false;
    int sesid = -1;
    int count = m_commMsgListWnd->m_listWidget->count();
    for (int i = 0; i < count; i++)
    {
        QListWidgetItem* pitem = m_commMsgListWnd->m_listWidget->item(i);
        CommMsgItemWnd* pWnd = dynamic_cast<CommMsgItemWnd*>(m_commMsgListWnd->m_listWidget->itemWidget(pitem));
        QString namestr = pWnd->m_usernameLabel->text();
        if (namestr == infoMap["name"])
        {
            m_commMsgListWnd->m_listWidget->setCurrentItem(pitem);
            sesid = pWnd->m_sesId;
            has = true;
            break;
        }
    }

    if (has)
    {
        m_lastSesId = sesid;
        m_toolWnd->m_msgBtn->click();
        slotSesIdToIndex(sesid);
    }
}

void MainWnd::slotToolWndPageChanged(int page)
{
    m_sMiddleLayout->setCurrentIndex(page);
    if (page == 0)
    {
        requestSessionList();
        if (m_lastSesId != -1)
        {  // 上次的会话窗口
            slotSesIdToIndex(m_lastSesId);
        }
        else
        {
            // 设置背景图片打开
            m_commContactInfo->showBgPng();
        }
    }

    if (page == 1)
    {  // 请求好友信息
        requestFriendList();
        m_commContactInfo->showBgPng();
        slotSesIdToIndex(0);
    }

    if (page == 2)
    {
        // 预留位置
    }
}

void MainWnd::slotReplyFinished(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QPixmap pixmap;
        pixmap.loadFromData(reply->readAll());
        pixmap = pixmap.scaled(40, 40);
        m_toolWnd->m_headImg = pixmap;
        DataManager::getMgr()->m_UserId2HeadImgMap[m_userid] = m_toolWnd->m_headImg;
        m_toolWnd->m_headUrlLabel->setPixmap(m_toolWnd->m_headImg);
    }
}

void MainWnd::slotOnSystemTrayIconClick(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Unknown:
        {
            LogD << "unknown message";
        }
        break;

        case QSystemTrayIcon::Context:
        {
            // 右键菜单
            LogD << "context";
        }
        break;
        case QSystemTrayIcon::DoubleClick:
        {
            LogD << "double click";
        }
        break;
        case QSystemTrayIcon::Trigger:
        {
            // 最小化重新显示窗口
            if (windowState() == Qt::WindowMinimized)
            {
                showNormal();
            }
            LogD << "trigger";
        }
        break;
        case QSystemTrayIcon::MiddleClick:
        {
            LogD << "midlle click";
        }
        break;
        default:
            break;
    }
}

void MainWnd::slotOnSettingBtnClick()
{
    m_settingWnd->show();
    // m_settingWnd->update();
}

void MainWnd::slotSearchText(QString searchText)
{
    LogD << " searchText = " << searchText;
    m_sMiddleLayout->setCurrentIndex(3);
    // 清空列表项
    int count = m_commSearchListWnd->m_listWidget->count();
    for (int i = 0; i < count; i++)
    {
        QListWidgetItem* pitem = m_commSearchListWnd->m_listWidget->item(i);
        CommMsgItemWnd* pWnd = dynamic_cast<CommMsgItemWnd*>(m_commMsgListWnd->m_listWidget->itemWidget(pitem));
        if (pWnd)
        {
            delete pWnd;
            pWnd = nullptr;
        }
    }

    m_commSearchListWnd->m_listWidget->clear();
    count = m_commMsgListWnd->m_listWidget->count();
    for (int i = 0; i < count; i++)
    {
        QListWidgetItem* pitem = m_commMsgListWnd->m_listWidget->item(i);
        CommMsgItemWnd* pWnd = dynamic_cast<CommMsgItemWnd*>(m_commMsgListWnd->m_listWidget->itemWidget(pitem));
        if (pWnd->m_usernameLabel->text().contains(searchText))
        {
            // 创建item并添加到
            m_commSearchListWnd->addMsgItem(pWnd->m_usernameLabel->text().toStdString().c_str(), pWnd->m_msgLabel->text().toStdString().c_str(), pWnd->m_sesId, pWnd->m_userid, pWnd->m_isGroupMsg);
        }
    }
}
