#include "QMainWnd.h"
#include "QSimpleSplit.h"
#include "json/CJsonObject.hpp"
#include "QWSClientMgr.h"
#include <QApplication>
#include "QCommMsgItemWnd.h"
#include "QCommContactItemWnd.h"
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include "QChatMsgWnd.h"
#include "QDataManager.h"
#include "QChatFileOuterWnd.h"
#include "QCommGroupItemWnd.h"
#include <QPainter>
#include <cstdlib>
#include <qmath.h>
#include <QGraphicsDropShadowEffect>

QMainWnd* QMainWnd::m_mainWnd = nullptr;

QMainWnd::QMainWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    setObjectName("QMainWnd");
    m_hLayout = new QHBoxLayout(this);
    m_toolWnd = new QToolWnd(this);

    m_commMsgListWnd = new QCommListWnd(this, QCommListWnd::QCommListWndEnum::MsgItemWndTpye);
    m_commContactsListWnd = new QCommListWnd(this, QCommListWnd::QCommListWndEnum::ContactItemWndType);
    m_commGroupsListWnd = new QCommListWnd(this, QCommListWnd::QCommListWndEnum::GroupItemWndType);

    m_hLayout->setContentsMargins(0, 0, 0, 0);
    m_hLayout->setSpacing(0);

    m_sLayout1 = new QStackedLayout();

    //消息列表
    m_sLayout1->addWidget(m_commMsgListWnd);
    //联系人列表
    m_sLayout1->addWidget(m_commContactsListWnd);
    //群组列表
    m_sLayout1->addWidget(m_commGroupsListWnd);
    m_sLayout1->setContentsMargins(0, 0, 0, 0);

    m_hLayout->setSpacing(0);

    m_sLayout2 = new QStackedLayout();
    m_sLayout2->setContentsMargins(0, 0, 0, 0);

    m_commContactInfo = new QCommContactInfo(this);
    m_sLayout2->addWidget(m_commContactInfo);
    connect(m_commContactInfo->m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
    connect(m_commContactInfo->m_maxBtn, SIGNAL(clicked()), this, SLOT(maxWnd()));
    connect(m_commContactInfo->m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));

    m_dealNewFriendsApplyWnd = new QDealNewFriendsApplyWnd(this);
    connect(m_dealNewFriendsApplyWnd->m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
    connect(m_dealNewFriendsApplyWnd->m_maxBtn, SIGNAL(clicked()), this, SLOT(maxWnd()));
    connect(m_dealNewFriendsApplyWnd->m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));
    m_sLayout2->addWidget(m_dealNewFriendsApplyWnd);
    m_commContactsListWnd->addContactsItem("./img/head2.png", "新的朋友", true);

    m_hLayout->addWidget(m_toolWnd);

    {
        /*添加分割线的示例代码*/
        QSimpleSplit* sp = new QSimpleSplit(this, QSimpleSplit::Direction_H);
        m_hLayout->addWidget(sp);
    }

    m_hLayout->addLayout(m_sLayout1, 0);

    {
        /*添加分割线的示例代码*/
        QSimpleSplit* sp = new QSimpleSplit(this, QSimpleSplit::Direction_V);
        m_hLayout->addWidget(sp);
    }
    m_hLayout->setSpacing(0);
    m_hLayout->addLayout(m_sLayout2, 1);
    // m_hLayout->addWidget(m_groupInfoWnd);
    m_hLayout->addStretch();

    setLayout(m_hLayout);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    //
    connect(m_toolWnd, SIGNAL(signalToolWndPageChanged(int)), this, SLOT(slotToolWndPageChanged(int)));
    connect(m_commMsgListWnd, SIGNAL(signalCommListChanged(int)), this, SLOT(slotSesIdToIndex(int)));
    connect(m_commContactsListWnd,
            SIGNAL(signalContactInfoChange(QMap<QString, QString>)),
            m_commContactInfo,
            SLOT(slotContactInfoChange(QMap<QString, QString>)));
    connect(m_commContactInfo, SIGNAL(signalSendMsgBtnClick(QMap<QString, QString>)), this, SLOT(slotSendMsgBtnClick(QMap<QString, QString>)));

    QWSClientMgr::getInstance()->regMsgCall("cs_msg_sendmsg", std::bind(&QMainWnd::cs_msg_sendmsg, this, std::placeholders::_1));
    QWSClientMgr::getInstance()->regMsgCall("cs_msg_sendgroupmsg", std::bind(&QMainWnd::cs_msg_sendgroupmsg, this, std::placeholders::_1));
    QWSClientMgr::getInstance()->regMsgCall("cs_msg_update_sessionlist", std::bind(&QMainWnd::cs_msg_update_sessionlist, this, std::placeholders::_1));
    QWSClientMgr::getInstance()->regMsgCall("cs_msg_update_grouplist", std::bind(&QMainWnd::cs_msg_update_grouplist, this, std::placeholders::_1));
    QWSClientMgr::getInstance()->regMsgCall("cs_msg_update_friendlist", std::bind(&QMainWnd::cs_msg_update_friendlist, this, std::placeholders::_1));

    m_networkMgr = new QNetworkAccessManager();
    connect(m_networkMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotReplyFinished(QNetworkReply*)));

    // 系统托盘功能
    m_systemTrayIcon = new QSystemTrayIcon(this);
    m_systemTrayIcon->setIcon(QIcon("./img/wechat.ico"));
    m_systemTrayIcon->setToolTip("QT版微信v2.0.0");
    m_systemTrayIcon->show();

    m_systemTrayIconMenu = new QMenu();
    m_systemTrayIconShowMainWndAction = new QAction();
    m_systemTrayIconShowMainWndAction->setText("打开主面板");
    m_systemTrayIconMenu->addAction(m_systemTrayIconShowMainWndAction);

    m_systemTrayIconExitAction = new QAction();
    m_systemTrayIconExitAction->setText("退出");
    m_systemTrayIconMenu->addAction(m_systemTrayIconExitAction);
    m_systemTrayIcon->setContextMenu(m_systemTrayIconMenu);

    connect(m_systemTrayIconExitAction, &QAction::triggered, this, &QMainWnd::closeWnd);
    connect(m_systemTrayIconShowMainWndAction, &QAction::triggered, this, &QMainWnd::showNormalWnd);
    connect(m_systemTrayIcon, &QSystemTrayIcon::activated, this, &QMainWnd::slotOnSystemTrayIconClick);

    if (objectName().isEmpty())
        setObjectName("QMainWnd");
    setStyleSheet("QWidget#QMainWnd{ background: transparent;}");
    setMinimumSize(800, 600);
    setMouseTracking(true);

    m_voiceTelphoneWnd = new QVoiceTelphoneWnd();
    m_voiceTelphoneWnd->hide();
}

// QMainWnd::~QMainWnd()
//{
//    // m_systemTrayIcon->hide();
//    // delete m_systemTrayIcon;
//}

void QMainWnd::cs_msg_sendmsg(neb::CJsonObject& msg)
{
    // QMessageBox::information(nullptr, "info", msg.ToString().c_str());
    //首先获取对应的会话id；
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

    //查找对应的会话
    QSessionWnd* ses = nullptr;
    int count = m_sLayout2->count();
    for (int i = 0; i < count; i++)
    {
        if (i <= 1)
        {
            continue;
        }

        QLayoutItem* item = m_sLayout2->layout()->itemAt(i);
        QSessionWnd* sesWnd = dynamic_cast<QSessionWnd*>(item->widget());
        if (sesWnd->m_sesId == sesid)
        {
            ses = sesWnd;
            break;
        }
    }

    if (ses == nullptr)
    {
        LogDebug << "can not find ses sesId:" << sesid;
        return;
    }

    // 发送消息
    if (msgtype == 0)
    {
        //向会话中嵌入一条数据；
        QString time = QString::number(QDateTime::currentDateTime().toTime_t());
        QChatMsgWnd* msgWnd = new QChatMsgWnd(ses->m_MsgWndList, sendid, recvid);
        QListWidgetItem* msgItem = new QListWidgetItem(ses->m_MsgWndList);
        msgWnd->setFixedWidth(640);
        QSize msgSize = msgWnd->fontRect(msgtext.c_str());
        msgItem->setSizeHint(msgSize);
        //会设置消息并调用相应的
        msgWnd->setText(msgtext.c_str(), time, msgSize, QChatMsgWnd::ChatMsg_Other);
        //关联项与窗口
        ses->m_MsgWndList->setItemWidget(msgItem, msgWnd);
        return;
    }

    // 发送文件
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

        ////接收端是一个文件
        QChatFileOuterWnd* fileWnd = new QChatFileOuterWnd(nullptr, sendid, recvid);
        fileWnd->m_innerWnd->m_fileName->setText(filename_client.c_str());
        fileWnd->m_innerWnd->m_fileSize->setText(filesize.c_str());
        fileWnd->m_innerWnd->m_sendState->setText("等待下载...");
        fileWnd->m_innerWnd->m_serveFilePath = filename_server.c_str();
        fileWnd->m_innerWnd->recvFileShow();

        fileWnd->setFixedWidth(ses->m_MsgWndList->width());
        QListWidgetItem* fileItem = new QListWidgetItem(ses->m_MsgWndList);
        QSize fileWndSize(fileWnd->width(), 100 + 20);
        fileItem->setSizeHint(fileWndSize);
        ses->m_MsgWndList->setItemWidget(fileItem, fileWnd);
    }

    ses->m_MsgWndList->scrollToBottom();
}

void QMainWnd::cs_msg_sendgroupmsg(neb::CJsonObject& msg)
{
    //首先获取对应的会话id；
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

    //查找对应的会话
    QSessionWnd* ses = nullptr;
    int count = m_sLayout2->count();
    for (int i = 0; i < count; i++)
    {
        if (i <= 1)
        {
            continue;
        }

        QLayoutItem* item = m_sLayout2->layout()->itemAt(i);
        QSessionWnd* sesWnd = dynamic_cast<QSessionWnd*>(item->widget());
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
            //向会话中嵌入一条数据；
            QString time = QString::number(QDateTime::currentDateTime().toTime_t());
            QChatMsgWnd* msgWnd = new QChatMsgWnd(ses->m_MsgWndList, sendid, recvid);
            QListWidgetItem* msgItem = new QListWidgetItem(ses->m_MsgWndList);
            msgWnd->setFixedWidth(640);
            QSize msgSize = msgWnd->fontRect(msgtext.c_str());

            //

            msgItem->setSizeHint(msgSize);
            //会设置消息并调用相应的
            msgWnd->setText(msgtext.c_str(), time, msgSize, QChatMsgWnd::ChatMsg_Other);
            //关联项与窗口
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

            ////接收端是一个文件
            QChatFileOuterWnd* fileWnd = new QChatFileOuterWnd(nullptr, sendid, recvid);
            fileWnd->m_innerWnd->m_fileName->setText(filename_client.c_str());
            fileWnd->m_innerWnd->m_fileSize->setText(filesize.c_str());
            fileWnd->m_innerWnd->m_sendState->setText("等待下载...");
            fileWnd->m_innerWnd->m_serveFilePath = filename_server.c_str();
            fileWnd->m_innerWnd->recvFileShow();
            //

            fileWnd->setFixedWidth(ses->m_MsgWndList->width());
            QListWidgetItem* fileItem = new QListWidgetItem(ses->m_MsgWndList);
            QSize fileWndSize(fileWnd->width(), 100 + 20);
            fileItem->setSizeHint(fileWndSize);
            ses->m_MsgWndList->setItemWidget(fileItem, fileWnd);
        }

        ses->m_MsgWndList->scrollToBottom();
    }
}

void QMainWnd::cs_msg_update_sessionlist(neb::CJsonObject& msg)
{
    requestSessionList();
}

void QMainWnd::cs_msg_update_grouplist(neb::CJsonObject& msg)
{
    requestGroupList();
}

void QMainWnd::cs_msg_update_friendlist(neb::CJsonObject& msg)
{
    requestFriendList();
}

void QMainWnd::requestHeadImg()
{
    QString imgurl = QString("http://49.232.169.205:80/UploadDemo/img/u%1.png").arg(m_userid);
    m_networkMgr->get(QNetworkRequest(QUrl(imgurl)));
}

void QMainWnd::requestFriendList()
{
    //
    neb::CJsonObject json;
    json.Add("ownerid", QMainWnd::getInstance()->m_userid);
    QWSClientMgr::getInstance()->request("cs_msg_get_friendslist", json, [this](neb::CJsonObject& msg) {
        // QMessageBox::information(nullptr, "info", msg.ToString().c_str());
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

void QMainWnd::requestSessionList()
{
    //向远端请求会话列表
    neb::CJsonObject json;
    json.Add("ownerid", QMainWnd::getInstance()->getInstance()->m_userid);
    QWSClientMgr::getInstance()->request("cs_msg_get_sessionlist", json, [this](neb::CJsonObject& msg) {
        // QMessageBox::information(nullptr, "info", msg.ToString().c_str());
        LogDebug << "msg:" << msg.ToString().c_str();
        //向会话列表中添加一些数据
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
                QSessionWnd* sesWnd = new QSessionWnd();
                sesWnd->m_sesTopWnd->m_titleLabel->setText(recvusername.c_str());
                sesWnd->m_sesId = sesid;
                sesWnd->m_recvId = recvid;
                sesWnd->m_isGroupSes = isgroupses;

                // 不是群组会话，应该隐藏more按钮
                if (!isgroupses)
                {
                    sesWnd->m_sesTopWnd->m_moreBtn->hide();
                }

                connect(sesWnd->m_sesTopWnd->m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
                connect(sesWnd->m_sesTopWnd->m_maxBtn, SIGNAL(clicked()), this, SLOT(maxWnd()));
                connect(sesWnd->m_sesTopWnd->m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));
                m_sLayout2->addWidget(sesWnd);
            }
        }
    });
}

bool QMainWnd::hasSessionWndBySessionId(int sesid)
{
    bool bHas = false;
    int count = m_sLayout2->count();
    for (int i = 0; i < count; i++)
    {
        if (i <= 1)
        {
            continue;
        }

        QLayoutItem* item = m_sLayout2->layout()->itemAt(i);
        QSessionWnd* sesWnd = dynamic_cast<QSessionWnd*>(item->widget());
        if (sesWnd->m_sesId == sesid)
        {
            bHas = true;
            break;
        }
    }
    return bHas;
}

void QMainWnd::requestGroupList()
{
    neb::CJsonObject json;
    json.Add("ownerid", QMainWnd::getInstance()->getInstance()->m_userid);
    QWSClientMgr::getInstance()->request("cs_msg_get_groupList", json, [this](neb::CJsonObject& msg) {
        LogDebug << "requestGroupList:" << msg.ToString().c_str();
        //先判断传入是否是msg["data"]是否是array
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

            //获取groupname，获取群组的groupid
            // groupname
            std::string groupname = "";
            if (!tempJson.Get("groupname", groupname))
            {
                continue;
            }

            //获取到groupid
            int groupid = -1;
            if (!tempJson.Get("groupid", groupid))
            {
                continue;
            }

            m_commGroupsListWnd->addGroupItem("./img/groupHead.png", groupname.c_str(), groupid);
        }
    });
}

// 参考资料：https://blog.csdn.net/tormi21c/article/details/124237553?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522168042672816800215064844%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=168042672816800215064844&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~baidu_landing_v2~default-3-124237553-null-null.142^v80^pc_new_rank,201^v4^add_ask,239^v2^insert_chatgpt&utm_term=qt%20%E9%BC%A0%E6%A0%87%E6%8B%96%E5%8A%A8%E7%AA%97%E5%8F%A3%E6%94%BE%E5%A4%A7%E7%BC%A9%E5%B0%8F&spm=1018.2226.3001.4187
void QMainWnd::UpdateBorderArea(QPoint pos)
{
    m_borderArea = BorderArea::BorderAreaNone;
    int offset = 5;

    int x = pos.x();
    int y = pos.y();
    int w = this->width();
    int h = this->height();

    if (x >= -offset && x < offset && y >= -offset && y < offset)
    {
        m_borderArea = BorderArea::BorderAreaTopLeft;
        return;
    }

    if (x > w - offset && x <= w + offset && y >= -offset && y < offset)
    {
        m_borderArea = BorderArea::BorderAreaTopRight;
        return;
    }

    if (x >= -offset && x < offset && y > h - offset && y <= h + offset)
    {
        m_borderArea = BorderArea::BorderAreaBottomLeft;
        return;
    }

    if (x > w - offset && x <= w + offset && y > h - offset && y <= h + offset)
    {
        m_borderArea = BorderArea::BorderAreaBottomRight;
        return;
    }

    if (y <= offset && x > offset && x < w - offset)
    {
        m_borderArea = BorderArea::BorderAreaTop;
        return;
    }

    if (x <= offset && y > offset && y < h - offset)
    {
        m_borderArea = BorderArea::BorderAreaLeft;
        return;
    }

    if (x >= w - offset && y > offset && y < h - offset)
    {
        m_borderArea = BorderArea::BorderAreaRight;
        return;
    }

    if (y >= h - offset && x > offset && x < w - offset)
    {
        m_borderArea = BorderArea::BorderAreaBottom;
        return;
    }
}

void QMainWnd::UpdateCursor()
{
    switch (m_borderArea)
    {
        case BorderArea::BorderAreaNone:
            setCursor(Qt::ArrowCursor);
            break;
        case BorderArea::BorderAreaTop:
        case BorderArea::BorderAreaBottom:
            setCursor(Qt::SizeVerCursor);
            break;

        case BorderArea::BorderAreaLeft:
        case BorderArea::BorderAreaRight:
            setCursor(Qt::SizeHorCursor);
            break;

        case BorderArea::BorderAreaTopLeft:
        case BorderArea::BorderAreaBottomRight:
            setCursor(Qt::SizeFDiagCursor);
            break;

        case BorderArea::BorderAreaTopRight:
        case BorderArea::BorderAreaBottomLeft:
            setCursor(Qt::SizeBDiagCursor);
            break;
        default:
            break;
    }
}

void QMainWnd::UpdateWindowByBorderArea()
{
    switch (m_borderArea)
    {
        case QMainWnd::BorderAreaNone:
            break;
        case QMainWnd::BorderAreaTop:

            break;
        case QMainWnd::BorderAreaBottom:
            break;
        case QMainWnd::BorderAreaLeft:
            break;
        case QMainWnd::BorderAreaRight:
            break;
        case QMainWnd::BorderAreaTopLeft:
            break;
        case QMainWnd::BorderAreaTopRight:
            break;
        case QMainWnd::BorderAreaBottomLeft:
            break;
        case QMainWnd::BorderAreaBottomRight:
            break;
        default:
            break;
    }
}

void QMainWnd::closeWnd()
{
    m_systemTrayIcon->hide();
    qApp->quit();
}

void QMainWnd::minWnd()
{
    showMinimized();
}

void QMainWnd::maxWnd()
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

void QMainWnd::showNormalWnd()
{
    if (windowState() == Qt::WindowMinimized)
    {
        showNormal();
    }
}

void QMainWnd::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_bLeftBtnPress)
    {
        UpdateBorderArea(event->pos());
        UpdateCursor();
        return;
    }

    if (windowState() == Qt::WindowMaximized)
    {
        return;
    }

    // 鼠标移动的调试信息
    LogDebug << "[mouseMoveEvent and event->pos]: x:" << event->pos().x() << "y:" << event->pos().y();
    LogDebug << "[mouseMoveEvent and m_poPress]: x:" << m_leftBtnPressPoint.x() << "y:" << m_leftBtnPressPoint.y();
    LogDebug << "[mouseMoveEvent and pos()]: x:" << pos().x() << "y:" << pos().y();
    LogDebug << "[mouseMoveEvent distance]:x:" << (event->pos() - m_leftBtnPressPoint).x();

    if (m_borderArea == BorderArea::BorderAreaNone)
    {
        move(event->pos() - m_leftBtnPressPoint + pos());
    }
    else
    {
        adjustWndSizeByMouseMove(event);
    }
}

void QMainWnd::adjustWndSizeByMouseMove(QMouseEvent* event)
{
    if (m_borderArea == BorderArea::BorderAreaRight)
    {
        LogDebug << "mini size: w:" << minimumSize().width() << "h:" << minimumSize().height();
        QPoint distancePoint = event->pos() - m_leftBtnPressPoint;
        int wndW = width() + distancePoint.x();
        if (wndW < 950)
        {
            return;
        }
        setFixedWidth(wndW);
        m_leftBtnPressPoint = event->pos();
        return;
    }
    else if (m_borderArea == BorderArea::BorderAreaLeft)
    {
        QRect wndRect = rect();
        QPoint distancePoint = event->pos() - m_leftBtnPressPoint;

        int gWndX = pos().x() + wndRect.x() + distancePoint.x();
        int gWndY = pos().y();
        int wndW = width() - distancePoint.x();
        int wndH = height();

        LogDebug << "[BorderArea::BorderAreaLeft]:"
                 << "rect.x() = " << wndRect.x() << "distance.x() = " << distancePoint.x() << "width = " << width() << "height = " << height();

        // 小于最小宽度不允许继续缩放
        if (wndW < 950)
        {
            return;
        }

        setGeometry(gWndX, gWndY, wndW, wndH);
        setFixedWidth(wndW);
        m_leftBtnPressPoint = event->pos();
        m_leftBtnPressPoint.setX(m_leftBtnPressPoint.x() - distancePoint.x());
        return;
    }
    else if (m_borderArea == BorderArea::BorderAreaTop)
    {
        QRect wndRect = rect();
        QPoint distancePoint = event->pos() - m_leftBtnPressPoint;

        int gWndX = pos().x();
        int gWndY = pos().y() + wndRect.y() + distancePoint.y();
        int wndW = width();
        int wndH = height() - distancePoint.y();
        if (wndH < 600)
        {
            return;
        }
        setGeometry(gWndX, gWndY, wndW, wndH);
        setFixedHeight(wndH);
        m_leftBtnPressPoint = event->pos();
        m_leftBtnPressPoint.setY(m_leftBtnPressPoint.y() - distancePoint.y());
    }
    else if (m_borderArea == BorderArea::BorderAreaBottom)
    {
        QPoint distancePoint = event->pos() - m_leftBtnPressPoint;
        int wndH = height() + distancePoint.y();
        if (wndH < 600)
        {
            return;
        }
        setFixedHeight(wndH);
        m_leftBtnPressPoint = event->pos();
        return;
    }
}

void QMainWnd::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    // 鼠标左键按下
    m_bLeftBtnPress = true;
    m_leftBtnPressPoint = event->pos();
    UpdateBorderArea(event->pos());
    UpdateCursor();
    LogDebug << "left button: x:" << m_leftBtnPressPoint.x() << "y:" << m_leftBtnPressPoint.y();
}

void QMainWnd::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    m_bLeftBtnPress = false;
}

void QMainWnd::slotSesIdToIndex(int sesid)
{
    int layoutId = 0;
    if (sesid != 0)
    {
        int count = m_sLayout2->count();
        for (int i = 0; i < count; i++)
        {
            if (i <= 1)
            {
                continue;
            }

            QLayoutItem* item = m_sLayout2->layout()->itemAt(i);
            QSessionWnd* sesWnd = dynamic_cast<QSessionWnd*>(item->widget());
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

    m_sLayout2->setCurrentIndex(layoutId);
}

void QMainWnd::slotSendMsgBtnClick(QMap<QString, QString> infoMap)
{
    bool has = false;
    int sesid = -1;
    int count = m_commMsgListWnd->m_listWidget->count();
    for (int i = 0; i < count; i++)
    {
        QListWidgetItem* pitem = m_commMsgListWnd->m_listWidget->item(i);
        QCommMsgItemWnd* pWnd = dynamic_cast<QCommMsgItemWnd*>(m_commMsgListWnd->m_listWidget->itemWidget(pitem));
        QString namestr = pWnd->m_name->text();
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

void QMainWnd::slotToolWndPageChanged(int page)
{
    m_sLayout1->setCurrentIndex(page);
    if (page == 0)
    {
        requestSessionList();
        if (m_lastSesId != -1)
        {  //上次的会话窗口
            slotSesIdToIndex(m_lastSesId);
        }
        else
        {
            //设置背景图片打开
            m_commContactInfo->showBgPng();
        }
    }

    if (page == 1)
    {  //请求好友信息
        requestFriendList();
        m_commContactInfo->showBgPng();
        slotSesIdToIndex(0);
    }

    if (page == 2)
    {
        //预留位置
    }
}

void QMainWnd::slotReplyFinished(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QPixmap pixmap;
        pixmap.loadFromData(reply->readAll());
        pixmap = pixmap.scaled(40, 40);
        m_toolWnd->m_headImg = pixmap;
        QDataManager::getInstance()->m_UserId2HeadImgMap[m_userid] = m_toolWnd->m_headImg;
        m_toolWnd->m_headUrlLabel->setPixmap(m_toolWnd->m_headImg);
    }
}

void QMainWnd::slotOnSystemTrayIconClick(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Unknown:
        {
            LogDebug << "unknown message";
        }
        break;

        case QSystemTrayIcon::Context:
        {
            // 右键菜单
            LogDebug << "context";
        }
        break;
        case QSystemTrayIcon::DoubleClick:
        {
            LogDebug << "double click";
        }
        break;
        case QSystemTrayIcon::Trigger:
        {
            // 最小化重新显示窗口
            if (windowState() == Qt::WindowMinimized)
            {
                showNormal();
            }
            LogDebug << "trigger";
        }
        break;
        case QSystemTrayIcon::MiddleClick:
        {
            LogDebug << "midlle click";
        }
        break;
        default:
            break;
    }
}
