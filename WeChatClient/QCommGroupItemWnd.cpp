#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include "QDataManager.h"
#include "QCommGroupItemWnd.h"

QCommGroupItemWnd::QCommGroupItemWnd(QWidget* p)
	: QWidget(p)
{
	
}

QCommGroupItemWnd::QCommGroupItemWnd(QWidget* p, const char* headUrl, const char* name, int groupId)
	:QWidget(p),m_groupId(groupId) 
{
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
	QString imgurl = QString("http://49.232.169.205:80/UploadDemo/img/g%1.png").arg(groupId);
	m_networkMgr = new QNetworkAccessManager();
	connect(m_networkMgr, SIGNAL(finished(QNetworkReply*)),this, SLOT(slot_replyFinished(QNetworkReply*)));
	m_networkMgr->get(QNetworkRequest(QUrl(imgurl)));

	setFixedHeight(65);
	setObjectName("QCommGroupItemWnd");
}

void QCommGroupItemWnd::slot_replyFinished(QNetworkReply* reply)
{
	if (reply->error() == QNetworkReply::NoError)
	{
		QPixmap pixmap;
		pixmap.loadFromData(reply->readAll());
		pixmap = pixmap.scaled(40,40);
		m_groupImgUrl->setPixmap(pixmap);
		QDataManager::getInstance()->m_GroupId2ImgMap[m_groupId] = pixmap;
	}
}

