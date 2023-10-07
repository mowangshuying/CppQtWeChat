#include "QGroupFriendsItemWnd.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include "QDataManager.h"

QGroupFriendsItemWnd::QGroupFriendsItemWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    LogFunc;
    setObjectName("QGroupFriendsItemWnd");
    m_hLayout = new QHBoxLayout();
    setLayout(m_hLayout);
    m_hLayout->setContentsMargins(0, 0, 0, 0);
    setContentsMargins(5, 5, 5, 5);

    m_headImg = new QLabel();
    m_headImg->setFixedSize(30, 30);
    m_hLayout->addWidget(m_headImg);

    m_name = new QLabel();
    m_name->setText("群友昵称");
    m_hLayout->addWidget(m_name);
    m_friendId = -1;

    setFixedHeight(40);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground);

    m_networkMgr = new QNetworkAccessManager();
}

void QGroupFriendsItemWnd::requestHeadImg()
{
    //向远端请求头像的信息
    QString imgurl = QString("http://49.232.169.205:80/UploadDemo/img/u%1.png").arg(m_friendId);
    connect(m_networkMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotReplyFinished(QNetworkReply*)));
    m_networkMgr->get(QNetworkRequest(QUrl(imgurl)));
}

void QGroupFriendsItemWnd::slotReplyFinished(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QPixmap pixmap;
        pixmap.loadFromData(reply->readAll());
        pixmap = pixmap.scaled(30, 30);
        m_headImg->setPixmap(pixmap);
        QDataManager::getMgr()->m_UserId2HeadImgMap[m_friendId] = pixmap;

        reply->deleteLater();
    }
}
