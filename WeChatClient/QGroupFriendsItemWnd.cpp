#include "QGroupFriendsItemWnd.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include "QDataManager.h"

QGroupFriendsItemWnd::QGroupFriendsItemWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    setObjectName("QGroupFriendsItemWnd");

    m_hLayout = new QHBoxLayout();
    setLayout(m_hLayout);
    m_hLayout->setContentsMargins(0, 0, 0, 0);
    setContentsMargins(5, 5, 5, 5);
    // setWindowFlags(Qt::FramelessWindowHint);
    m_headImg = new QLabel();
    m_headImg->setFixedSize(30, 30);
    m_hLayout->addWidget(m_headImg);

    m_name = new QLabel();
    m_name->setText("Ⱥ���ǳ�");
    m_hLayout->addWidget(m_name);
    m_friendId = -1;

    setFixedHeight(40);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground);
}

void QGroupFriendsItemWnd::requestHeadImg()
{
    //��Զ������ͷ�����Ϣ
    QString imgurl = QString("http://49.232.169.205:80/UploadDemo/img/u%1.png").arg(m_friendId);
    m_networkMgr = new QNetworkAccessManager();
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
    }
}
