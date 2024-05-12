#include <string>
#include "DataManager.h"
#include "CommMsgItemWnd.h"

CommMsgItemWnd::CommMsgItemWnd(QWidget* p) : QWidget(p)
{
    setObjectName("QCommMsgItemWnd");
}

CommMsgItemWnd::CommMsgItemWnd(QWidget* p, const char* name, const char* msg, int64_t sesid, int64_t userid, bool bGroupMsg)
{
    setObjectName("QCommMsgItemWnd");

    m_sesId = sesid;
    m_userid = userid;
    m_isGroupMsg = bGroupMsg;

    m_vLayout = new QVBoxLayout();
    m_hLayout = new QHBoxLayout();
    m_usernameLabel = new QLabel();
    m_msgLabel = new QLabel();

    QFont font("", 10, 60);
    m_usernameLabel->setFont(font);

    m_usernameLabel->setText(name);
    m_msgLabel->setText(msg);

    m_vLayout->addSpacing(10);
    m_vLayout->addWidget(m_usernameLabel);
    m_vLayout->addWidget(m_msgLabel);
    m_vLayout->addSpacing(10);

    m_headurl = new QLabel();
    m_headurl->setFixedSize(40, 40);

    // 设置默认头像
    m_url = "./img/default.png";
    if (bGroupMsg)
        m_url = "./img/groupHead.png";

    m_headurl->setPixmap(QPixmap(m_url));

    m_hLayout->addSpacing(10);
    m_hLayout->addWidget(m_headurl);
    m_hLayout->addSpacing(15);
    m_hLayout->addLayout(m_vLayout);

    m_hLayout->setContentsMargins(0, 0, 0, 0);
    m_hLayout->setSpacing(0);

    setLayout(m_hLayout);
    setFixedHeight(65);

    requestHeadImg(userid, bGroupMsg);
}

void CommMsgItemWnd::requestHeadImg(int id, bool isGroupMsg)
{
    QString imgurl = QString("http://49.232.169.205:80/UploadDemo/img/u%1.png").arg(id);
    if (isGroupMsg)
    {
        QString imgurl = QString("http://49.232.169.205:80/UploadDemo/img/g%1.png").arg(id);
    }
    m_networkMgr = new QNetworkAccessManager();
    connect(m_networkMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotReplyFinished(QNetworkReply*)));
    m_networkMgr->get(QNetworkRequest(QUrl(imgurl)));
}

CommMsgItemWnd::~CommMsgItemWnd()
{
    LogFunc;
    if (m_networkMgr)
    {
        delete m_networkMgr;
        m_networkMgr = nullptr;
    }
}

void CommMsgItemWnd::slotReplyFinished(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        if (!m_isGroupMsg)
        {
            loadUserHeadPixmap(reply);
        }
        else
        {
            loadGroupHeadPixmap(reply);
        }
    }

    reply->deleteLater();
}

void CommMsgItemWnd::loadUserHeadPixmap(QNetworkReply* reply)
{
    QPixmap pixmap;
    pixmap.loadFromData(reply->readAll());
    pixmap = pixmap.scaled(40, 40);
    m_headurl->setPixmap(pixmap);
    DataManager::getMgr()->m_UserId2HeadImgMap[m_userid] = pixmap;
}

void CommMsgItemWnd::loadGroupHeadPixmap(QNetworkReply* reply)
{
    QPixmap pixmap;
    pixmap.loadFromData(reply->readAll());
    pixmap = pixmap.scaled(40, 40);
    m_headurl->setPixmap(pixmap);
    DataManager::getMgr()->m_GroupId2ImgMap[m_userid] = pixmap;
}
