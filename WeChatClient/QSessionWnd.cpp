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
#include "QSimpleSplit.h"
#include "QMainWnd.h"
#include "QWSClientMgr.h"
#include "json/CJsonObject.hpp"
#include "QMessageBox.h"
#include "QChatFileInnerWnd.h"
#include "QChatFileOuterWnd.h"
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

QSessionWnd::QSessionWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    setMinimumSize(640, 600);
    setWindowTitle("�Ự����");
    setAcceptDrops(true);
    setMouseTracking(true);

    m_vLayout = new QVBoxLayout();
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
    m_sendTextEdit = new QTextEdit(this);
    m_sendTextEdit->setStyleSheet("border:0px;");
    m_sendTextEdit->setAcceptDrops(false);

    // �Ự���ڵĹ���������
    m_sesToolBar = new QSessionToolBar();

    // ���Ͱ�ť
    m_sendTextBtn = new QPushButton(this);
    m_sendTextBtn->setFixedSize(70, 30);
    m_sendTextBtn->setText("����(S)");

    m_vLayout->addWidget(m_sesTopWnd);

    {
        /*��ӷָ��ߵ�ʾ������*/
        QSimpleSplit* sp = new QSimpleSplit(this);
        m_vLayout->addWidget(sp);
    }

    m_vLayout->addWidget(m_MsgWndList, 2);

    {
        /*��ӷָ��ߵ�ʾ������*/
        QSimpleSplit* sp = new QSimpleSplit(this);
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
    //��ť���ʱ������Ϣ
    connect(m_sendTextBtn, SIGNAL(clicked()), this, SLOT(slot_sendTextBtnClick()));
    m_sendTextBtn->setStyleSheet("background-color:#1aad19;border-style: none;");

    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet("background-color:white;border:0px;");

    connect(m_sesToolBar->m_emoijWnd, SIGNAL(signal_emoijClicked(QString)), this, SLOT(slot_emoijClicked(QString)));
    connect(m_sesTopWnd->m_moreBtn, SIGNAL(clicked()), this, SLOT(slot_moreBtnClick()));
}

void QSessionWnd::slot_sendTextBtnClick()
{
    QString msgText = m_sendTextEdit->toPlainText();

    //��������κ����ݲ�������
    if (msgText == "")
    {
        return;
    }

    m_sendTextEdit->setText("");

    neb::CJsonObject json;
    json.Add("sendid", QMainWnd::getInstance()->m_userid);
    json.Add("recvid", m_recvId);
    json.Add("sesid", m_sesId);
    json.Add("msgtext", msgText.toStdString().c_str());
    json.Add("msgtype", 0);

    //������Ƿ���Ⱥ��Ϣ
    if (m_isGroupSes == false)
    {
        QWSClientMgr::getInstance()->request("cs_msg_sendmsg", json, [this, msgText](neb::CJsonObject& msg) {
            //��Զ�˷�����Ϣ
            QString time = QString::number(QDateTime::currentDateTime().toTime_t());
            QChatMsgWnd* msgWnd = new QChatMsgWnd(m_MsgWndList, QMainWnd::getInstance()->m_userid, m_recvId);
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

    //���͵���Ⱥ��Ϣ
    if (m_isGroupSes == true)
    {
        QWSClientMgr::getInstance()->request("cs_msg_sendgroupmsg", json, [this, msgText](neb::CJsonObject& msg) {
            LogDebug << "cs_msg_sendgroupmsg:msg=" << msg.ToString().c_str();
            //��Զ�˷�����Ϣ
            QString time = QString::number(QDateTime::currentDateTime().toTime_t());
            QChatMsgWnd* msgWnd = new QChatMsgWnd(m_MsgWndList, QMainWnd::getInstance()->m_userid, m_recvId);
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
}

void QSessionWnd::slot_emoijClicked(QString str)
{
    QString tempStr = m_sendTextEdit->toPlainText() + str;
    m_sendTextEdit->setText(tempStr);
}

void QSessionWnd::slot_moreBtnClick()
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
    QWSClientMgr::getInstance()->request("cs_msg_get_group_info", json, [this](neb::CJsonObject& msg) {
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

            // �鿴�б����Ƿ���ڸ��û����������Ӧ�ò�������û���Ϣ
            bool ishas = false;
            QListWidget* listWidget = m_groupInfoWnd->m_groupfriendsWnd->m_listWnd;
            for (int i = 0; i < listWidget->count(); i++)
            {
                QListWidgetItem* item = listWidget->item(i);
                if (item == NULL)
                {
                    continue;
                }

                QGroupFriendsItemWnd* wnd = dynamic_cast<QGroupFriendsItemWnd*>(listWidget->itemWidget(item));
                if (wnd == NULL)
                {
                    continue;
                }

                if (wnd->m_friendId == ownerid)
                {
                    ishas = true;
                    break;
                }
            }

            if (ishas)
            {
                continue;
            }

            QGroupFriendsItemWnd* itemWnd = new QGroupFriendsItemWnd();
            itemWnd->m_headImg->setPixmap(QPixmap("./img/groupHead.png"));
            itemWnd->m_friendId = ownerid;
            itemWnd->m_name->setText(QString(nickname.c_str()));
            m_groupInfoWnd->m_groupfriendsWnd->addItem(itemWnd);
            itemWnd->requestHeadImg();
        }
    });

    //��ʾȺ�����Ϣ��
    m_groupInfoWnd->show();
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
    QFileInfo fileinfo = QFileInfo(strFileName);

    if (!fileinfo.isDir())
    {
        //��ȡ�ļ���
        QString filename = fileinfo.fileName();
        QString filepath = fileinfo.filePath();

        //��ȡ�ļ���׺��
        QString suffix = fileinfo.suffix();
        //��ȡ�ļ���С
        float size = fileinfo.size() * 100 / 1024 * 0.01;
        LogDebug << "filename:" << filename << "suffix:" << suffix << "size:" << size << "kb";

        QString sizeStr;
        if (size < 1024)
        {
            sizeStr = QString("%1 K").arg(size);
        }
        else
        {
            size = fileinfo.size() * 100 / (1024 * 1024) * 0.01;
            sizeStr = QString("%1 M").arg(size);
        }

        //{//�������ڲ��Եģ��Ժ�Ҫɾ����
        QChatFileOuterWnd* fileWnd = new QChatFileOuterWnd(nullptr, QMainWnd::getInstance()->m_userid, m_recvId);
        fileWnd->m_innerWnd->m_fileName->setText(filename);
        fileWnd->m_innerWnd->m_fileSize->setText(sizeStr);
        // fileWnd->m_innerWnd->m_fileDir = strFileName;
        fileWnd->m_innerWnd->m_fileFullDir = fileinfo.absolutePath();
        fileWnd->m_innerWnd->m_fileFullpath = fileinfo.absoluteFilePath();
        fileWnd->m_innerWnd->sendFileShow();

        fileWnd->setFixedWidth(m_MsgWndList->width());
        QListWidgetItem* fileItem = new QListWidgetItem(m_MsgWndList);
        QSize filesize(fileWnd->width(), 100 + 20);
        fileItem->setSizeHint(filesize);
        m_MsgWndList->setItemWidget(fileItem, fileWnd);
        //}

        QNetworkAccessManager* pManager = new QNetworkAccessManager(this);
        QNetworkRequest request;
        request.setUrl(QUrl("http://139.9.93.17:8081/upload"));
        QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType, this);
        QHttpPart part;

        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy_MM_dd_hh_mm_ss_zzz_");
        QString uploadfilestr = QString("form-data;name=\"file\";filename=\"%1_%2_%3\"").arg(QMainWnd::getInstance()->m_userid).arg(current_date).arg(filename);
        QString uploadfilestr2 = QString("%1_%2_%3").arg(QMainWnd::getInstance()->m_userid).arg(current_date).arg(filename);
        part.setHeader(QNetworkRequest::ContentDispositionHeader, uploadfilestr);

        QFile* file = new QFile(strFileName);
        file->open(QFile::ReadOnly);
        part.setBodyDevice(file);
        file->setParent(multiPart);
        multiPart->append(part);
        QNetworkReply* reply = pManager->post(request, multiPart);

        connect(reply, &QNetworkReply::uploadProgress, this, [this, fileWnd, uploadfilestr2, sizeStr, filename](qint64 x, qint64 y) {
            if (y != 0)
            {
                fileWnd->m_innerWnd->m_progressBar->setMinimum(0);
                fileWnd->m_innerWnd->m_progressBar->setMaximum(y);
                fileWnd->m_innerWnd->m_progressBar->setValue(x);
            }
        });

        connect(pManager, &QNetworkAccessManager::finished, this, [this, fileWnd, uploadfilestr2, sizeStr, filename](QNetworkReply* reply) {
            // ��ȡ��Ӧ��Ϣ
            QByteArray bytes = reply->readAll();
            std::string str = bytes.toStdString();
            int i = 0;
            fileWnd->m_innerWnd->m_sendState->setText("�ѷ���");
            neb::CJsonObject json;
            json.Add("sendid", QMainWnd::getInstance()->m_userid);
            json.Add("recvid", m_recvId);
            json.Add("sesid", m_sesId);
            json.Add("msgtype", 1);

            neb::CJsonObject filejson;
            filejson.Add("filename_server", str);
            filejson.Add("filename_client", filename.toStdString());
            filejson.Add("filesize", sizeStr.toStdString());
            json.Add("msgtext", filejson.ToString());

            QWSClientMgr::getInstance()->request("cs_msg_sendmsg", json, [this](neb::CJsonObject& msg) {
                LogDebug << "after upload file recv msg from server!";
            });
        });
    }
}

void QSessionWnd::resizeEvent(QResizeEvent* event)
{
    if (m_groupInfoWnd != nullptr)
    {
        int nTempHeight = height() - m_sesTopWnd->height();
        m_groupInfoWnd->setMinimumHeight(nTempHeight);
        m_groupInfoWnd->m_scrollArea->setMinimumHeight(nTempHeight);
    }
}
