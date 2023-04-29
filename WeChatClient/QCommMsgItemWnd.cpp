#include <string>
#include "QDataManager.h"
#include "QCommMsgItemWnd.h"

QCommMsgItemWnd::QCommMsgItemWnd(QWidget* p) : QWidget(p)
{
    setObjectName("QCommMsgItemWnd");
}

QCommMsgItemWnd::QCommMsgItemWnd(QWidget* p, const char* name, const char* msg, int64_t sesid, int64_t userid, bool isGroppMsg)
{
    setObjectName("QCommMsgItemWnd");

    m_sesId = sesid;
    m_userid = userid;
    m_isGroupMsg = isGroppMsg;

    m_vLayout = new QVBoxLayout();
    m_hLayout = new QHBoxLayout();
    m_name = new QLabel();
    m_msg = new QLabel();

    QFont font("", 10, 60);
    m_name->setFont(font);

    m_name->setText(name);
    m_msg->setText(msg);

    m_vLayout->addSpacing(10);
    m_vLayout->addWidget(m_name);
    m_vLayout->addWidget(m_msg);
    m_vLayout->addSpacing(10);

    m_headurl = new QLabel();
    m_headurl->setFixedSize(40, 40);

    m_url = "./img/default.png";
    if (isGroppMsg)
    {
        m_url = "./img/groupHead.png";
    }
    m_headurl->setPixmap(QPixmap(m_url));

    m_hLayout->addSpacing(10);
    m_hLayout->addWidget(m_headurl);
    m_hLayout->addSpacing(15);
    m_hLayout->addLayout(m_vLayout);

    m_hLayout->setContentsMargins(0, 0, 0, 0);
    m_hLayout->setSpacing(0);

    setLayout(m_hLayout);
    setFixedHeight(65);

    // ./img/groupHead.png
    // ./img/head1.png

    QString imgurl = QString("http://49.232.169.205:80/UploadDemo/img/u%1.png").arg(userid);
    if (isGroppMsg)
    {
        QString imgurl = QString("http://49.232.169.205:80/UploadDemo/img/g%1.png").arg(userid);
    }
    m_networkMgr = new QNetworkAccessManager();
    connect(m_networkMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotReplyFinished(QNetworkReply*)));
    m_networkMgr->get(QNetworkRequest(QUrl(imgurl)));
}

void QCommMsgItemWnd::slotReplyFinished(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        //如果不是群组消息，好友之间的消息
        if (!m_isGroupMsg)
        {
            QPixmap pixmap;
            pixmap.loadFromData(reply->readAll());
            pixmap = pixmap.scaled(40, 40);
            m_headurl->setPixmap(pixmap);
            QDataManager::getMgr()->m_UserId2HeadImgMap[m_userid] = pixmap;
        }
        else
        {
            QPixmap pixmap;
            pixmap.loadFromData(reply->readAll());
            pixmap = pixmap.scaled(40, 40);
            m_headurl->setPixmap(pixmap);
            QDataManager::getMgr()->m_GroupId2ImgMap[m_userid] = pixmap;
        }
    }
}
