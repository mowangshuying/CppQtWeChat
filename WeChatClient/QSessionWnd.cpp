#include <QDateTime>
#include <QSplitter>
#include <QListWidget>
#include <QScrollBar>
#include <QMimeData>
#include <QFileInfo>
#include <QHttpMultiPart>

#include "QSessionWnd.h"
#include "QChatMsgWnd.h"
#include "QSessionTopWnd.h"
#include "QSelfSplit.h"
#include "QMainWnd.h"
#include "QWSClientMgr.h"
#include "json/CJsonObject.hpp"
#include "QMessageBox.h"
#include "QChatFileInnerWnd.h"
#include "QChatFileOuterWnd.h"
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include "QStyleSheetMgr.h"

QSessionWnd::QSessionWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    setObjectName("QSessionWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + objectName() + ".qss";
    object.m_widget = this;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    setMinimumSize(640, 600);
    setWindowTitle("�Ự����");
    setAcceptDrops(true);
    setMouseTracking(true);

    m_vLayout = new QVBoxLayout(this);
    m_vLayout->setContentsMargins(0, 0, 0, 0);
    m_vLayout->setSpacing(0);
    setLayout(m_vLayout);

    // ÿ���Ự��������һ��groupInfoWnd
    m_groupInfoWnd = new QGroupInfoWnd();
    m_groupInfoWnd->hide();

    // �Ự���ڵ�top
    m_sesTopWnd = new QSessionTopWnd(this);

    // ��Ϣ����
    m_MsgWndList = new QListWidget(this);
    m_MsgWndList->setAcceptDrops(false);
    m_MsgWndList->setMinimumWidth(this->width());

    // ������Ϣ����
    m_sendTextEdit = new QSelfTextEdit(this);
    // m_sendTextEdit->setStyleSheet("border:0px;");
    m_sendTextEdit->setAcceptDrops(false);
    m_sendTextEdit->setAcceptRichText(true);

    // �Ự���ڵĹ���������
    m_sesToolBar = new QSessionToolBar();

    // ���Ͱ�ť
    m_sendTextBtn = new QPushButton(this);
    m_sendTextBtn->setFixedSize(70, 30);
    m_sendTextBtn->setText("����(S)");

    m_vLayout->addWidget(m_sesTopWnd);

    {
        /*��ӷָ��ߵ�ʾ������*/
        QSelfSplit* sp = new QSelfSplit(this);
        m_vLayout->addWidget(sp);
    }

    m_vLayout->addWidget(m_MsgWndList, 2);

    {
        /*��ӷָ��ߵ�ʾ������*/
        QSelfSplit* sp = new QSelfSplit(this);
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

    //��ť���ʱ������Ϣ
    connect(m_sendTextBtn, SIGNAL(clicked()), this, SLOT(slotSendTextBtnClick()));
    m_sendTextBtn->setStyleSheet("background-color:#1aad19;border-style: none;");

    setAttribute(Qt::WA_StyledBackground);
    // setStyleSheet("background-color:white;border:0px;");

    connect(m_sesToolBar->m_emoijWnd, SIGNAL(signalEmoijClicked(QString)), this, SLOT(slotEmoijClicked(QString)));
    connect(m_sesTopWnd->m_moreBtn, SIGNAL(clicked()), this, SLOT(slotMoreBtnClick()));
    connect(m_sesToolBar->m_voiceTelphoneBtn, &QPushButton::clicked, this, &QSessionWnd::slotVoiceTelPhoneBtnClick);
}

void QSessionWnd::slotSendTextBtnClick()
{
    QString msgText = m_sendTextEdit->toPlainText();
    //��������κ����ݲ�������
    if (msgText == "")
    {
        LogErr << "msgText is empty!";
        return;
    }

    m_sendTextEdit->setText("");
    neb::CJsonObject json;
    json.Add("sendid", QMainWnd::getMainWnd()->m_userid);
    json.Add("recvid", m_recvId);
    json.Add("sesid", m_sesId);
    json.Add("msgtext", msgText.toStdString().c_str());
    json.Add("msgtype", 0);

    //������Ƿ���Ⱥ��Ϣ
    if (m_isGroupSes == false)
    {
        sendMsgToUser(json, msgText);
        return;
    }

    //���͵���Ⱥ��Ϣ
    if (m_isGroupSes == true)
    {
        sendMsgToGroup(json, msgText);
        return;
    }
}

void QSessionWnd::slotEmoijClicked(QString str)
{
    QString tempStr = m_sendTextEdit->toPlainText() + str;
    m_sendTextEdit->setText(tempStr);
    LogDebug << tempStr;
}

void QSessionWnd::slotMoreBtnClick()
{
    //������Ӧ��λ��
    QRect rect = m_sesTopWnd->geometry();
    QPoint gPoint = m_sesTopWnd->mapToGlobal(QPoint(0, 0));
    QRect swRect = m_sesTopWnd->geometry();
    swRect.setX(gPoint.x() + m_sesTopWnd->width() - m_groupInfoWnd->width());
    swRect.setY(gPoint.y() + m_sesTopWnd->height());
    m_groupInfoWnd->setGeometry(swRect);

    // ��Զ�������Ⱥ����Ϣ׼������
    //��Զ�˷�������������
    neb::CJsonObject json;
    json.Add("groupId", m_recvId);
    QWSClientMgr::getMgr()->request("cs_msg_get_group_info", json, [this](neb::CJsonObject& msg) {
        LogDebug << "cs_msg_get_group_info msg:" << msg.ToString().c_str();

        // ��Ⱥ�����б���Ƕ������
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

    //��ʾȺ�����Ϣ��
    m_groupInfoWnd->show();
}

void QSessionWnd::slotVoiceTelPhoneBtnClick()
{
    QMainWnd* mainWnd = QMainWnd::getMainWnd();
    QVoiceTelphoneWnd* telphoneWnd = mainWnd->m_voiceTelphoneWnd;
    telphoneWnd->setRecvIdAndSesId(m_recvId, m_sesId);
    if (telphoneWnd->windowState() == Qt::WindowMinimized)
    {
        telphoneWnd->showNormal();
    }
    telphoneWnd->show();
    telphoneWnd->activateWindow();
    telphoneWnd->callPhone();
}

void QSessionWnd::dragEnterEvent(QDragEnterEvent* event)
{
    LogDebug << "dragEnterEvent";
    event->acceptProposedAction();
}

void QSessionWnd::dropEvent(QDropEvent* event)
{
    LogDebug << "dropEvent";
    //��ק�ɹ���ȡ�ļ���Ϣ
    const QMimeData* qm = event->mimeData();
    QString strFileName = qm->urls()[0].toLocalFile();
    LogDebug << "strFileName:" << strFileName;

    //�ж��Ƿ���֧�ֵ��ļ�
    QFileInfo fileInfo = QFileInfo(strFileName);
    if (fileInfo.isDir())
    {
        LogErr << "is dir";
        return;
    }

    //��ȡ�ļ���
    QString filename = fileInfo.fileName();
    QString filepath = fileInfo.filePath();
    //��ȡ�ļ���׺��
    QString suffix = fileInfo.suffix();
    //��ȡ�ļ���С
    QString fileSize = getFileSize(fileInfo);

    // �ļ���Ϣ����
    QChatFileOuterWnd* fileWnd = new QChatFileOuterWnd(nullptr, QMainWnd::getMainWnd()->m_userid, m_recvId);
    fileWnd->setFileUploadData(filename, fileSize, fileInfo.absolutePath(), fileInfo.absoluteFilePath());
    fileWnd->setFixedWidth(m_MsgWndList->width());

    QListWidgetItem* fileItem = new QListWidgetItem(m_MsgWndList);
    QSize filesize(fileWnd->width(), 120);
    fileItem->setSizeHint(filesize);
    m_MsgWndList->setItemWidget(fileItem, fileWnd);

    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDate = currentDateTime.toString("yyyy_MM_dd_hh_mm_ss_zzz_");
    QString httpHeader = QString("form-data;name=\"file\";filename=\"%1_%2_%3\"").arg(QMainWnd::getMainWnd()->m_userid).arg(currentDate).arg(filename);
    QString fileName = QString("%1_%2_%3").arg(QMainWnd::getMainWnd()->m_userid).arg(currentDate).arg(filename);

    // �ļ���
    QFile* file = new QFile(strFileName);
    file->open(QFile::ReadOnly);

    QNetworkAccessManager* pManager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl("http://49.232.169.205:80/UploadDemo/UploadServlet"));
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType, this);
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader, httpHeader);
    part.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(part);
    QNetworkReply* reply = pManager->post(request, multiPart);

    connect(reply, &QNetworkReply::uploadProgress, this, [this, fileWnd, fileName, fileSize, filename](qint64 x, qint64 y) {
        if (y != 0)
        {
            fileWnd->m_innerWnd->m_progressBar->setMinimum(0);
            fileWnd->m_innerWnd->m_progressBar->setMaximum(y);
            fileWnd->m_innerWnd->m_progressBar->setValue(x);
        }
    });

    connect(pManager, &QNetworkAccessManager::finished, this, [this, fileWnd, fileName, fileSize, filename, &file](QNetworkReply* reply) {
        // ��ȡ��Ӧ��Ϣ
        QByteArray bytes = reply->readAll();
        std::string str = bytes.toStdString();
        int i = 0;
        fileWnd->m_innerWnd->m_sendState->setText("�ѷ���");

        neb::CJsonObject json;
        json.Add("sendid", QMainWnd::getMainWnd()->m_userid);
        json.Add("recvid", m_recvId);
        json.Add("sesid", m_sesId);
        json.Add("msgtype", 1);
        neb::CJsonObject filejson;
        filejson.Add("filename_server", str);
        filejson.Add("filename_client", filename.toStdString());
        filejson.Add("filesize", fileSize.toStdString());
        json.Add("msgtext", filejson.ToString());

        // �ͷ�reply
        reply->deleteLater();

        //�ͷ��ļ��ڴ�
        file->close();
        delete file;
        file = nullptr;

        QWSClientMgr::getMgr()->request("cs_msg_sendmsg", json, [this](neb::CJsonObject& msg) { LogDebug << "after upload file recv msg from server!"; });
    });
}

QString QSessionWnd::getFileSize(QFileInfo& fileInfo)
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

void QSessionWnd::resizeEvent(QResizeEvent* event)
{
    if (m_groupInfoWnd != nullptr)
    {
        int nTempHeight = height() - m_sesTopWnd->height();
        m_groupInfoWnd->setMinimumHeight(nTempHeight);
        m_groupInfoWnd->m_scrollArea->setMinimumHeight(nTempHeight);
        // return;
    }

    // ��������msgItem�Ĵ�С
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

void QSessionWnd::sendMsgToUser(neb::CJsonObject json, QString msgText)
{
    QWSClientMgr::getMgr()->request("cs_msg_sendmsg", json, [this, msgText](neb::CJsonObject& msg) {
        //��Զ�˷�����Ϣ
        QString time = QString::number(QDateTime::currentDateTime().toTime_t());
        QChatMsgWnd* msgWnd = new QChatMsgWnd(m_MsgWndList, QMainWnd::getMainWnd()->m_userid, m_recvId);
        QListWidgetItem* msgItem = new QListWidgetItem(m_MsgWndList);
        msgWnd->setFixedWidth(this->width());
        QSize msgSize = msgWnd->fontRect(msgText);
        msgItem->setSizeHint(msgSize);
        //��������Ϣ��������Ӧ��
        msgWnd->setText(msgText, time, msgSize, QChatMsgWnd::ChatMsg_Owner);
        //�������봰��
        m_MsgWndList->setItemWidget(msgItem, msgWnd);
        //�Ż��߼��������ײ�
        m_MsgWndList->scrollToBottom();
    });
}

void QSessionWnd::sendMsgToGroup(neb::CJsonObject json, QString msgText)
{
    QWSClientMgr::getMgr()->request("cs_msg_sendgroupmsg", json, [this, msgText](neb::CJsonObject& msg) {
        LogDebug << "cs_msg_sendgroupmsg:" << msg.ToString().c_str();
        //��Զ�˷�����Ϣ
        QString time = QString::number(QDateTime::currentDateTime().toTime_t());
        QChatMsgWnd* msgWnd = new QChatMsgWnd(m_MsgWndList, QMainWnd::getMainWnd()->m_userid, m_recvId);
        QListWidgetItem* msgItem = new QListWidgetItem(m_MsgWndList);
        msgWnd->setFixedWidth(this->width());
        QSize msgSize = msgWnd->fontRect(msgText);
        msgItem->setSizeHint(msgSize);
        //��������Ϣ��������Ӧ��
        msgWnd->setText(msgText, time, msgSize, QChatMsgWnd::ChatMsg_Owner);
        //�������봰��
        m_MsgWndList->setItemWidget(msgItem, msgWnd);
        //�Ż��߼��������ײ�
        m_MsgWndList->scrollToBottom();
    });
}
