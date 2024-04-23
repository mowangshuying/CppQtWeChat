#include <QDateTime>
#include <QSplitter>
#include <QListWidget>
#include <QScrollBar>
#include <QMimeData>
#include <QFileInfo>
#include <QHttpMultiPart>

#include "SessionWnd.h"
#include "ChatMsgWnd.h"
#include "QSessionTopWnd.h"
#include "SelfSplit.h"
#include "MainWnd.h"
#include "WSClientMgr.h"
#include "json/CJsonObject.hpp"
#include "QMessageBox.h"
#include "ChatFileInnerWnd.h"
#include "ChatFileOuterWnd.h"
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include "StyleSheetMgr.h"

SessionWnd::SessionWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    LogFunc;
    setObjectName("QSessionWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + objectName() + ".qss";
    object.m_widget = this;
    StyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    setMinimumSize(640, 600);
    setWindowTitle("会话窗口");
    setAcceptDrops(true);
    setMouseTracking(true);

    m_vLayout = new QVBoxLayout(this);
    m_vLayout->setContentsMargins(0, 0, 0, 0);
    m_vLayout->setSpacing(0);
    setLayout(m_vLayout);

    // 每个会话都保存着一个groupInfoWnd
    m_groupInfoWnd = new GroupInfoWnd();
    // if (m_isGroupSes)
    //{
    //    m_groupInfoWnd->setGroupId(m_recvId);
    //}
    m_groupInfoWnd->hide();

    // 会话窗口的top
    m_sesTopWnd = new QSessionTopWnd(this);

    // 消息窗口
    m_MsgWndList = new QListWidget(this);
    m_MsgWndList->setAcceptDrops(false);
    m_MsgWndList->setMinimumWidth(this->width());

    // 发送消息窗口
    m_sendTextEdit = new SelfTextEdit(this);
    // m_sendTextEdit->setStyleSheet("border:0px;");
    m_sendTextEdit->setAcceptDrops(false);
    m_sendTextEdit->setAcceptRichText(true);

    // 会话窗口的工具栏部分
    m_sesToolBar = new SessionToolBar();

    // 发送按钮
    m_sendTextBtn = new QPushButton(this);
    m_sendTextBtn->setFixedSize(70, 30);
    m_sendTextBtn->setText("发送(S)");

    m_vLayout->addWidget(m_sesTopWnd);

    {
        /*添加分割线的示例代码*/
        SelfSplit* sp = new SelfSplit(this);
        m_vLayout->addWidget(sp);
    }

    m_vLayout->addWidget(m_MsgWndList, 2);

    {
        /*添加分割线的示例代码*/
        SelfSplit* sp = new SelfSplit(this);
        m_vLayout->addWidget(sp);
    }

    m_vLayout->addWidget(m_sesToolBar);
    m_vLayout->addWidget(m_sendTextEdit, 1);

    m_hLayout1 = new QHBoxLayout();
    m_hLayout1->addStretch();
    m_hLayout1->addWidget(m_sendTextBtn);
    m_hLayout1->setContentsMargins(0, 0, 15, 10);
    m_vLayout->addLayout(m_hLayout1);

    m_MsgWndList->setSelectionMode(QAbstractItemView::NoSelection);
    m_MsgWndList->setStyleSheet("border:2px red solid;");
    m_MsgWndList->setFocusPolicy(Qt::NoFocus);
    m_MsgWndList->setAttribute(Qt::WA_StyledBackground);
    m_MsgWndList->setAcceptDrops(true);
    m_MsgWndList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 按钮点击时候发送消息
    connect(m_sendTextBtn, SIGNAL(clicked()), this, SLOT(slotSendTextBtnClick()));
    m_sendTextBtn->setStyleSheet("background-color:#1aad19;border-style: none;");

    setAttribute(Qt::WA_StyledBackground);

    connect(m_sesToolBar->m_emoijWnd, SIGNAL(signalEmoijClicked(QString)), this, SLOT(slotEmoijClicked(QString)));
    connect(m_sesTopWnd->m_moreBtn, SIGNAL(clicked()), this, SLOT(slotMoreBtnClick()));
    connect(m_sesToolBar->m_voiceTelphoneBtn, &QPushButton::clicked, this, &SessionWnd::slotVoiceTelPhoneBtnClick);
    connect(m_groupInfoWnd, SIGNAL(signalUpdateGroupName(QString)), this, SLOT(slotUpdateGroupName(QString)));
}

SessionWnd::~SessionWnd()
{
    if (m_groupInfoWnd)
    {
        delete m_groupInfoWnd;
        m_groupInfoWnd = nullptr;
    }
}

void SessionWnd::slotSendTextBtnClick()
{
    QString msgText = m_sendTextEdit->toPlainText();
    // 如果不含任何内容不允许发送
    if (msgText == "")
    {
        LogErr << "msgText is empty!";
        return;
    }

    m_sendTextEdit->setText("");
    neb::CJsonObject json;
    json.Add("sendid", MainWnd::getMainWnd()->m_userid);
    json.Add("sendUserName", MainWnd::getMainWnd()->m_username.toStdString());
    json.Add("recvid", m_recvId);
    json.Add("sesid", m_sesId);
    json.Add("msgtext", msgText.toStdString().c_str());
    json.Add("msgtype", 0);

    // 如果不是发送群消息
    if (m_isGroupSes == false)
    {
        sendMsgToUser(json, msgText);
        return;
    }

    // 发送的是群消息
    if (m_isGroupSes == true)
    {
        sendMsgToGroup(json, msgText);
        return;
    }
}

void SessionWnd::slotEmoijClicked(QString str)
{
    QString tempStr = m_sendTextEdit->toPlainText() + str;
    m_sendTextEdit->setText(tempStr);
    LogDebug << tempStr;
}

void SessionWnd::slotMoreBtnClick()
{
    // 设置相应的位置
    QRect rect = m_sesTopWnd->geometry();
    QPoint gPoint = m_sesTopWnd->mapToGlobal(QPoint(0, 0));
    QRect swRect = m_sesTopWnd->geometry();
    swRect.setX(gPoint.x() + m_sesTopWnd->width() - m_groupInfoWnd->width());
    swRect.setY(gPoint.y() + m_sesTopWnd->height());
    m_groupInfoWnd->setGeometry(swRect);

    // 向远端请求该群聊信息准备设置
    // 向远端服务器发送请求
    neb::CJsonObject json;
    json.Add("groupId", m_recvId);
    WSClientMgr::getMgr()->request("cs_msg_get_group_info", json, [this](neb::CJsonObject& msg) {
        LogDebug << "cs_msg_get_group_info msg:" << msg.ToString().c_str();

        // 向群好友列表中嵌入数据
        neb::CJsonObject datajson;
        if (!msg.Get("data", datajson))
        {
            return;
        }

        neb::CJsonObject friendsJson;
        if (!datajson.Get("friends", friendsJson))
        {
            return;
        }

        for (int i = 0; i < friendsJson.GetArraySize(); i++)
        {
            neb::CJsonObject friendJson;
            if (!friendsJson.Get(i, friendJson))
            {
                continue;
            }

            int64_t ownerid = -1;
            if (!friendJson.Get("ownerid", ownerid))
            {
                continue;
            }

            std::string nickname;
            if (!friendJson.Get("nickname", nickname))
            {
                continue;
            }

            m_groupInfoWnd->addGroupFriendItem(ownerid, nickname.c_str());
        }
    });

    // 显示群组的信息等
    m_groupInfoWnd->show();
}

void SessionWnd::slotVoiceTelPhoneBtnClick()
{
    MainWnd* mainWnd = MainWnd::getMainWnd();
    VoiceTelphoneWnd* telphoneWnd = mainWnd->m_voiceTelphoneWnd;
    telphoneWnd->setRecvIdAndSesId(m_recvId, m_sesId);
    if (telphoneWnd->windowState() == Qt::WindowMinimized)
    {
        telphoneWnd->showNormal();
    }
    telphoneWnd->show();
    telphoneWnd->activateWindow();
    telphoneWnd->callPhone();
}

void SessionWnd::slotUpdateGroupName(QString groupName)
{
    m_sesTopWnd->m_titleLabel->setText(groupName);
    CommListWnd* pGroupListWnd = MainWnd::getMainWnd()->m_commGroupsListWnd;
    pGroupListWnd->setGroupItemNameByGroupId(m_recvId, groupName);
    CommListWnd* pMsgListWnd = MainWnd::getMainWnd()->m_commMsgListWnd;
    pMsgListWnd->setMsgItemNameBySesId(m_sesId, groupName);
}

void SessionWnd::dragEnterEvent(QDragEnterEvent* event)
{
    LogDebug << "dragEnterEvent";
    event->acceptProposedAction();
}

void SessionWnd::dropEvent(QDropEvent* event)
{
    LogDebug << "dropEvent";
    // 拖拽成功获取文件信息
    const QMimeData* qm = event->mimeData();
    QString strFileName = qm->urls()[0].toLocalFile();
    LogDebug << "strFileName:" << strFileName;

    // 判断是否是支持的文件
    QFileInfo fileInfo = QFileInfo(strFileName);
    if (fileInfo.isDir())
    {
        LogErr << "is dir";
        return;
    }

    // 获取文件名
    QString filename = fileInfo.fileName();
    QString filepath = fileInfo.filePath();
    // 获取文件后缀名
    QString suffix = fileInfo.suffix();
    // 获取文件大小
    QString fileSize = getFileSize(fileInfo);

    // 文件信息窗口
    ChatFileOuterWnd* fileWnd = new ChatFileOuterWnd(nullptr, MainWnd::getMainWnd()->m_userid, m_recvId);
    fileWnd->setFileUploadData(filename, fileSize, fileInfo.absolutePath(), fileInfo.absoluteFilePath());
    fileWnd->setFixedWidth(m_MsgWndList->width());

    QListWidgetItem* fileItem = new QListWidgetItem(m_MsgWndList);
    QSize filesize(fileWnd->width(), 120);
    fileItem->setSizeHint(filesize);
    m_MsgWndList->setItemWidget(fileItem, fileWnd);

    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDate = currentDateTime.toString("yyyy_MM_dd_hh_mm_ss_zzz_");
    QString httpHeader = QString("form-data;name=\"file\";filename=\"%1_%2_%3\"").arg(MainWnd::getMainWnd()->m_userid).arg(currentDate).arg(filename);
    QString fileName = QString("%1_%2_%3").arg(MainWnd::getMainWnd()->m_userid).arg(currentDate).arg(filename);

    // 文件名
    QFile* file = new QFile(strFileName);
    file->open(QFile::ReadOnly);

    QNetworkAccessManager* pManager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl("HTTP_FILE_SERVER_ADDR"));
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType, this);
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader, httpHeader);
    part.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(part);
    QNetworkReply* reply = pManager->post(request, multiPart);

    connect(reply, &QNetworkReply::uploadProgress, this, [this, fileWnd, fileName, fileSize, filename](qint64 x, qint64 y) {
        if (y == 0)
            return;

        fileWnd->setProgressBarMin(0);
        fileWnd->setProgressBarMax(y);
        fileWnd->setProgressBarValue(x);
    });

    connect(pManager, &QNetworkAccessManager::finished, this, [this, fileWnd, fileName, fileSize, filename, &file](QNetworkReply* reply) {
        // 获取响应信息
        QByteArray bytes = reply->readAll();
        std::string str = bytes.toStdString();
        int i = 0;
        fileWnd->m_innerWnd->m_sendState->setText("已发送");

        neb::CJsonObject json;
        json.Add("sendid", MainWnd::getMainWnd()->m_userid);
        json.Add("recvid", m_recvId);
        json.Add("sesid", m_sesId);
        json.Add("msgtype", 1);
        neb::CJsonObject filejson;
        filejson.Add("filename_server", str);
        filejson.Add("filename_client", filename.toStdString());
        filejson.Add("filesize", fileSize.toStdString());
        json.Add("msgtext", filejson.ToString());

        // 释放reply
        reply->deleteLater();

        // 释放文件内存
        file->close();
        delete file;
        file = nullptr;

        WSClientMgr::getMgr()->request("cs_msg_sendmsg", json, [this](neb::CJsonObject& msg) { LogDebug << "after upload file recv msg from server!"; });
    });
}

QString SessionWnd::getFileSize(QFileInfo& fileInfo)
{
    float size = fileInfo.size() * 100 / 1024 * 0.01;
    QString fileSize;
    if (size < 1024)
    {
        fileSize = QString("%1 K").arg(size);
    }
    else
    {
        size = fileInfo.size() * 100 / (1024 * 1024) * 0.01;
        fileSize = QString("%1 M").arg(size);
    }

    return fileSize;
}

void SessionWnd::resizeEvent(QResizeEvent* event)
{
    if (m_groupInfoWnd != nullptr)
    {
        int nTempHeight = height() - m_sesTopWnd->height();
        m_groupInfoWnd->setFixedHeight(nTempHeight);
        m_groupInfoWnd->m_centerWnd->setFixedHeight(nTempHeight);
        m_groupInfoWnd->m_scrollArea->setFixedHeight(nTempHeight);
    }

    // 重新设置msgItem的大小
    if (m_MsgWndList != nullptr)
    {
        int count = m_MsgWndList->count();
        for (int i = 0; i < count; i++)
        {
            QListWidgetItem* pitem = m_MsgWndList->item(i);
            QWidget* pWnd = m_MsgWndList->itemWidget(pitem);

            QSize tmpSize = pitem->sizeHint();
            tmpSize.setWidth(m_MsgWndList->width());
            pitem->setSizeHint(tmpSize);

            pWnd->setFixedWidth(m_MsgWndList->width());
        }
    }
}

void SessionWnd::sendMsgToUser(neb::CJsonObject json, QString msgText)
{
    // QString userName = QMainWnd::getMainWnd()->m_username;
    WSClientMgr::getMgr()->request("cs_msg_sendmsg", json, [this, msgText](neb::CJsonObject& msg) {
        dealMsgTime();
        // 向远端发送消息
        QString time = QString::number(QDateTime::currentDateTime().toTime_t());
        ChatMsgWnd* msgWnd = new ChatMsgWnd(m_MsgWndList, MainWnd::getMainWnd()->m_userid, MainWnd::getMainWnd()->m_username, m_recvId);
        QListWidgetItem* msgItem = new QListWidgetItem(m_MsgWndList);
        msgWnd->setFixedWidth(this->width());

        QSize msgSize = msgWnd->fontRect(msgText);
        msgItem->setSizeHint(msgSize);
        // 会设置消息并调用相应的
        msgWnd->setText(msgText, time, msgSize, ChatMsgWnd::ChatMsg_OwnerMsgText);
        // 关联项与窗口
        m_MsgWndList->setItemWidget(msgItem, msgWnd);
        // 优化逻辑滑动到底部
        m_MsgWndList->scrollToBottom();
    });
}

void SessionWnd::sendMsgToGroup(neb::CJsonObject json, QString msgText)
{
    WSClientMgr::getMgr()->request("cs_msg_sendgroupmsg", json, [this, msgText](neb::CJsonObject& msg) {
        dealMsgTime();

        LogDebug << "cs_msg_sendgroupmsg:" << msg.ToString().c_str();
        // 向远端发送消息
        QString time = QString::number(QDateTime::currentDateTime().toTime_t());
        ChatMsgWnd* msgWnd = new ChatMsgWnd(m_MsgWndList, MainWnd::getMainWnd()->m_userid, MainWnd::getMainWnd()->m_username, m_recvId);
        QListWidgetItem* msgItem = new QListWidgetItem(m_MsgWndList);
        msgWnd->setFixedWidth(this->width());
        QSize msgSize = msgWnd->fontRect(msgText);
        msgItem->setSizeHint(msgSize);
        // 会设置消息并调用相应的
        msgWnd->setText(msgText, time, msgSize, ChatMsgWnd::ChatMsg_OwnerMsgText);
        // 关联项与窗口
        m_MsgWndList->setItemWidget(msgItem, msgWnd);
        // 优化逻辑滑动到底部
        m_MsgWndList->scrollToBottom();
    });
}

void SessionWnd::dealMsgTime()
{
    bool bShowTime = false;
    if (m_MsgWndList->count() > 0)
    {
        QDateTime nowDateTime = QDateTime::currentDateTime();
        int64_t distanceS = m_lastMsgDateTime.secsTo(nowDateTime);
        LogDebug << "distanceS:" << distanceS;

        m_lastMsgDateTime = QDateTime::currentDateTime();
        if (distanceS >= 60 * 5)
        {
            bShowTime = true;
        }
    }
    else
    {
        m_lastMsgDateTime = QDateTime::currentDateTime();
        bShowTime = true;
    }

    if (bShowTime)
    {
        ChatMsgWnd* timeMsgWnd = new ChatMsgWnd(m_MsgWndList);
        QListWidgetItem* timeListItem = new QListWidgetItem(m_MsgWndList);

        QSize size = QSize(this->width(), 40);
        timeMsgWnd->resize(size);
        timeListItem->setSizeHint(size);

        QString curTimeStr = m_lastMsgDateTime.toString("yyyy/MM/dd hh:mm");
        timeMsgWnd->setText(curTimeStr, curTimeStr, size, ChatMsgWnd::ChatMsg_Time);
        m_MsgWndList->setItemWidget(timeListItem, timeMsgWnd);
    }
}
