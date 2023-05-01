#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include "QDataManager.h"
#include "QCommContactItemWnd.h"

QCommContactItemWnd::QCommContactItemWnd(QWidget* p) : QWidget(p)
{
    setObjectName("QCommContactItemWnd");
}

QCommContactItemWnd::QCommContactItemWnd(QWidget* p, const char* headUrl, const char* name, bool bNewFriend /* = false*/, int friendid /* = -1*/)
    : QWidget(p), m_bNewFriend(bNewFriend), m_friendId(friendid)
{
    setObjectName("QCommContactItemWnd");
    m_hLayout = new QHBoxLayout(this);
    m_hLayout->setSpacing(0);
    setLayout(m_hLayout);
    m_headUrl = new QLabel();

    m_headUrl->setPixmap(QPixmap(headUrl));
    m_headUrl->setFixedSize(40, 40);
    m_name = new QLabel();

    m_name->setText(name);
    m_hLayout->addWidget(m_headUrl);
    m_hLayout->addSpacing(15);
    m_hLayout->addWidget(m_name);

    //向远端请求头像的信息
    QString imgurl = QString("http://49.232.169.205:80/UploadDemo/img/u%1.png").arg(friendid);
    m_networkMgr = new QNetworkAccessManager();
    connect(m_networkMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotReplyFinished(QNetworkReply*)));
    m_networkMgr->get(QNetworkRequest(QUrl(imgurl)));

    if (name != NULL && !bNewFriend)
    {
        QDataManager::getMgr()->m_FriendId2NameMap[friendid] = name;
    }

    setFixedHeight(65);

    LogDebug << "w:" << this->width() << "h:" << this->height();
}

void QCommContactItemWnd::slotReplyFinished(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QPixmap pixmap;
        pixmap.loadFromData(reply->readAll());
        pixmap = pixmap.scaled(40, 40);
        m_headUrl->setPixmap(pixmap);
        QDataManager::getMgr()->m_UserId2HeadImgMap[m_friendId] = pixmap;
    }
}
