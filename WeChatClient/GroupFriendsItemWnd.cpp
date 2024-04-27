#include "GroupFriendsItemWnd.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include "DataManager.h"

GroupFriendsItemWnd::GroupFriendsItemWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    setObjectName("QGroupFriendsItemWnd");
    m_hLayout = new QHBoxLayout;
    setLayout(m_hLayout);
    m_hLayout->setContentsMargins(5, 5, 5, 5);
 //   setContentsMargins(5, 5, 5, 5);

    m_headImg = new QLabel;
    m_headImg->setFixedSize(30, 30);
    m_hLayout->addWidget(m_headImg, Qt::AlignHCenter);

    m_name = new QLabel;
    m_name->setText("群友昵称");
    m_hLayout->addWidget(m_name);
    m_friendId = -1;

    setFixedHeight(40);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground);

    m_networkMgr = new QNetworkAccessManager();
}

void GroupFriendsItemWnd::requestHeadImg()
{
    // 向远端请求头像的信息
    QString imgurl = QString("http://49.232.169.205:80/UploadDemo/img/u%1.png").arg(m_friendId);
    connect(m_networkMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotReplyFinished(QNetworkReply*)));
    m_networkMgr->get(QNetworkRequest(QUrl(imgurl)));
}

void GroupFriendsItemWnd::slotReplyFinished(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError)
        return;

    QPixmap pixmap;
    pixmap.loadFromData(reply->readAll());
    pixmap = pixmap.scaled(30, 30);
    m_headImg->setPixmap(pixmap);
    DataManager::getMgr()->m_UserId2HeadImgMap[m_friendId] = pixmap;
    reply->deleteLater();
}
