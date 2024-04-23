#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include "DataManager.h"
#include "CommGroupItemWnd.h"

CommGroupItemWnd::CommGroupItemWnd(QWidget* p) : QWidget(p)
{
    LogFunc;
    setObjectName("QCommGroupItemWnd");
}

CommGroupItemWnd::CommGroupItemWnd(QWidget* p,
                                     const char* headUrl,
                                     const char* name,
                                     int groupId)
    : QWidget(p), m_groupId(groupId)
{
    LogFunc;
    setObjectName("QCommGroupItemWnd");
    m_hLayout = new QHBoxLayout(this);
    m_hLayout->setSpacing(0);
    setLayout(m_hLayout);
    m_groupImgUrl = new QLabel();

    m_groupImgUrl->setPixmap(QPixmap(headUrl));
    m_groupImgUrl->setFixedSize(40, 40);
    m_groupName = new QLabel();

    m_groupName->setText(name);
    m_hLayout->addWidget(m_groupImgUrl);
    m_hLayout->addSpacing(15);
    m_hLayout->addWidget(m_groupName);

    //关于请求图片信息
    QString imgurl =
        QString("http://49.232.169.205:80/UploadDemo/img/g%1.png").arg(groupId);
    m_networkMgr = new QNetworkAccessManager(this);
    connect(m_networkMgr,
            SIGNAL(finished(QNetworkReply*)),
            this,
            SLOT(slotReplyFinished(QNetworkReply*)));
    m_networkMgr->get(QNetworkRequest(QUrl(imgurl)));

    setFixedHeight(65);
}

CommGroupItemWnd::~CommGroupItemWnd()
{
    if (m_networkMgr)
    {
        m_networkMgr = nullptr;
        delete m_networkMgr;
    }
}

void CommGroupItemWnd::slotReplyFinished(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QPixmap pixmap;
        pixmap.loadFromData(reply->readAll());
        pixmap = pixmap.scaled(40, 40);
        m_groupImgUrl->setPixmap(pixmap);
        DataManager::getMgr()->m_GroupId2ImgMap[m_groupId] = pixmap;
    }
}
