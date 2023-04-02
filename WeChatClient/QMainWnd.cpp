#include "QMainWnd.h"
#include "QSimpleSplit.h"
#include "json/CJsonObject.hpp"
#include "QWSClientMgr.h"
#include <QApplication>
#include "QCommMsgItemWnd.h"
#include "QCommContactItemWnd.h"
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include "QChatMsgWnd.h"
#include "QDataManager.h"
#include "QChatFileOuterWnd.h"
#include "QCommGroupItemWnd.h"
#include <QPainter>
#include <cstdlib>
#include <qmath.h>

QMainWnd* QMainWnd::m_mainWnd = nullptr;

QMainWnd::QMainWnd(QWidget* p /*= nullptr*/)
	: QWidget(p)
{
	m_hLayout = new QHBoxLayout(this);
	m_toolWnd = new QToolWnd(this);
	
	m_commMsgListWnd = new QCommListWnd(this,QCommListWnd::QCommListWndEnum::QCommMsgItemWnd_Tpye);
	m_commContactsListWnd = new QCommListWnd(this, QCommListWnd::QCommListWndEnum::QCommContactItemWnd_Type);
	m_commGroupsListWnd = new QCommListWnd(this, QCommListWnd::QCommListWndEnum::QCommGroupItemWnd_Type);

	//m_groupInfoWnd = new QGroupInfoWnd();
	//m_groupInfoWnd->show();

	m_hLayout->setContentsMargins(0, 0, 0, 0);
	m_hLayout->setSpacing(0);

	m_sLayout1 = new QStackedLayout();
	//消息列表
	m_sLayout1->addWidget(m_commMsgListWnd);
	//联系人列表
	m_sLayout1->addWidget(m_commContactsListWnd);
	//群组列表
	m_sLayout1->addWidget(m_commGroupsListWnd);
	m_sLayout1->setContentsMargins(0, 0, 0, 0);

	m_hLayout->setSpacing(0);

	m_sLayout2 = new QStackedLayout();
	m_sLayout2->setContentsMargins(0, 0, 0, 0);

	{
		m_commContactInfo = new QCommContactInfo(this);
		m_sLayout2->addWidget(m_commContactInfo);
		connect(m_commContactInfo->m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
		connect(m_commContactInfo->m_maxBtn, SIGNAL(clicked()), this, SLOT(maxWnd()));
		connect(m_commContactInfo->m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));
	}

	{
		m_dealNewFriendsApplyWnd = new QDealNewFriendsApplyWnd(this);
		connect(m_dealNewFriendsApplyWnd->m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
		connect(m_dealNewFriendsApplyWnd->m_maxBtn, SIGNAL(clicked()), this, SLOT(maxWnd()));
		connect(m_dealNewFriendsApplyWnd->m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));
		m_sLayout2->addWidget(m_dealNewFriendsApplyWnd);
	}

	m_commContactsListWnd->addContactsItem("./img/head2.png", "新的朋友",true);

	m_hLayout->addWidget(m_toolWnd);

	{
		/*添加分割线的示例代码*/
		QSimpleSplit* sp = new QSimpleSplit(this,QSimpleSplit::Direction_H);
		m_hLayout->addWidget(sp);
	}

	m_hLayout->addLayout(m_sLayout1,1);


	{
		/*添加分割线的示例代码*/
		QSimpleSplit* sp = new QSimpleSplit(this, QSimpleSplit::Direction_V);
		m_hLayout->addWidget(sp);
	}
	m_hLayout->setSpacing(0);
	m_hLayout->addLayout(m_sLayout2,2);
	//m_hLayout->addWidget(m_groupInfoWnd);
	m_hLayout->addStretch();

	setLayout(m_hLayout);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_StyledBackground);
	//setStyleSheet("background-color:#ffffff;");
	//setContentsMargins(2, 2, 2, 2);

	//
	connect(m_toolWnd, SIGNAL(signal_toolWndPageChanged(int)), 
		this, SLOT(slot_toolWndPageChanged(int)));

	connect(m_commMsgListWnd, SIGNAL(commListChanged(int)), 
		this, SLOT(slot_sesIdToIndex(int)));
	
	connect(m_commContactsListWnd, SIGNAL(signal_contactInfoChange(QMap<QString, QString>)),
		m_commContactInfo, SLOT(slot_contactInfoChange(QMap< QString, QString>)));

	connect(m_commContactInfo, SIGNAL(signal_sendMsgBtnClick(QMap<QString, QString>)),
		this, SLOT(slot_sendMsgBtnClick(QMap<QString, QString>)));

	QWSClientMgr::getInstance()->regMsgCall("cs_msg_sendmsg",
		std::bind(&QMainWnd::cs_msg_sendmsg,this, std::placeholders::_1) );

	QWSClientMgr::getInstance()->regMsgCall("cs_msg_sendgroupmsg",
		std::bind(&QMainWnd::cs_msg_sendgroupmsg, this, std::placeholders::_1));

	QWSClientMgr::getInstance()->regMsgCall("cs_msg_update_sessionlist",
		std::bind(&QMainWnd::cs_msg_update_sessionlist, this, std::placeholders::_1));

	QWSClientMgr::getInstance()->regMsgCall("cs_msg_update_grouplist",
		std::bind(&QMainWnd::cs_msg_update_grouplist, this, std::placeholders::_1));

	QWSClientMgr::getInstance()->regMsgCall("cs_msg_update_friendlist",
		std::bind(&QMainWnd::cs_msg_update_friendlist, this, std::placeholders::_1));

	m_networkMgr = new QNetworkAccessManager();
	connect(m_networkMgr, SIGNAL(finished(QNetworkReply*)),this, SLOT(slot_replyFinished(QNetworkReply*)));
}

void QMainWnd::cs_msg_sendmsg(neb::CJsonObject& msg)
{
	//QMessageBox::information(nullptr, "info", msg.ToString().c_str());
	//首先获取对应的会话id； 
	int sesid = -1;
	if (!msg["data"].Get("sesid", sesid))
	{
		return;
	}

	int msgtype = -1;
	if (!msg["data"].Get("msgtype", msgtype))
	{
		return;
	}

	std::string msgtext = "";
	if (!msg["data"].Get("msgtext", msgtext))
	{
		return;
	}

	int64_t sendid = -1;
	if (!msg["data"].Get("sendid", sendid))
	{
		return;
	}

	int64_t recvid = -1;
	if (!msg["data"].Get("sendid", recvid))
	{
		return;
	}

	//查找对应的会话
	QSessionWnd* ses = nullptr;
	int count = m_sLayout2->count();
	for (int i = 0; i < count; i++) {

		if (i <= 1) {
			continue;
		}

		QLayoutItem* item = m_sLayout2->layout()->itemAt(i);
		QSessionWnd* sesWnd = dynamic_cast<QSessionWnd*>(item->widget());
		if (sesWnd->m_sesId == sesid) {
			ses = sesWnd;
			break;
		}
	}

	if (ses != nullptr)
	{
		if (msgtype == 0)
		{
			//向会话中嵌入一条数据；
			QString time = QString::number(QDateTime::currentDateTime().toTime_t());
			QChatMsgWnd* msgWnd = new QChatMsgWnd(ses->m_MsgWndList, sendid, recvid);
			QListWidgetItem* msgItem = new QListWidgetItem(ses->m_MsgWndList);
			msgWnd->setFixedWidth(640);
			QSize msgSize = msgWnd->fontRect(msgtext.c_str());

			//

			msgItem->setSizeHint(msgSize);
			//会设置消息并调用相应的
			msgWnd->setText(msgtext.c_str(), time, msgSize, QChatMsgWnd::ChatMsg_Other);
			//关联项与窗口
			ses->m_MsgWndList->setItemWidget(msgItem, msgWnd);
		}

		if (msgtype == 1)
		{
			neb::CJsonObject json;
			if (!json.Parse(msgtext))
			{
				return;
			}

			std::string filename_server = "";
			std::string filename_client = "";
			std::string filesize = "";

			if (!json.Get("filename_server", filename_server))
			{
				return;
			}

			if (!json.Get("filename_client", filename_client))
			{
				return;
			}

			if (!json.Get("filesize", filesize))
			{
				return;
			}


			////接收端是一个文件
			QChatFileOuterWnd* fileWnd = new QChatFileOuterWnd(nullptr, sendid, recvid);
			fileWnd->m_innerWnd->m_fileName->setText(filename_client.c_str());
			fileWnd->m_innerWnd->m_fileSize->setText(filesize.c_str());
			fileWnd->m_innerWnd->m_sendState->setText("等待下载...");
			fileWnd->m_innerWnd->m_serveFilePath = filename_server.c_str();
			fileWnd->m_innerWnd->recvFileShow();
			//

			fileWnd->setFixedWidth(ses->m_MsgWndList->width());
			QListWidgetItem* fileItem = new QListWidgetItem(ses->m_MsgWndList);
			QSize fileWndSize(fileWnd->width(), 100 + 20);
			fileItem->setSizeHint(fileWndSize);
			ses->m_MsgWndList->setItemWidget(fileItem, fileWnd);
		}

		ses->m_MsgWndList->scrollToBottom();
	}
}

void QMainWnd::cs_msg_sendgroupmsg(neb::CJsonObject& msg)
{
	//QMessageBox::information(nullptr, "info", msg.ToString().c_str());
	//首先获取对应的会话id； 
	int sesid = -1;
	if (!msg["data"].Get("sesid", sesid))
	{
		return;
	}

	int msgtype = -1;
	if (!msg["data"].Get("msgtype", msgtype))
	{
		return;
	}

	std::string msgtext = "";
	if (!msg["data"].Get("msgtext", msgtext))
	{
		return;
	}

	int64_t sendid = -1;
	if (!msg["data"].Get("sendid", sendid))
	{
		return;
	}

	int64_t recvid = -1;
	if (!msg["data"].Get("sendid", recvid))
	{
		return;
	}

	//查找对应的会话
	QSessionWnd* ses = nullptr;
	int count = m_sLayout2->count();
	for (int i = 0; i < count; i++) {

		if (i <= 1) {
			continue;
		}

		QLayoutItem* item = m_sLayout2->layout()->itemAt(i);
		QSessionWnd* sesWnd = dynamic_cast<QSessionWnd*>(item->widget());
		if (sesWnd->m_sesId == sesid) {
			ses = sesWnd;
			break;
		}
	}

	if (ses != nullptr)
	{
		if (msgtype == 0)
		{
			//向会话中嵌入一条数据；
			QString time = QString::number(QDateTime::currentDateTime().toTime_t());
			QChatMsgWnd* msgWnd = new QChatMsgWnd(ses->m_MsgWndList, sendid, recvid);
			QListWidgetItem* msgItem = new QListWidgetItem(ses->m_MsgWndList);
			msgWnd->setFixedWidth(640);
			QSize msgSize = msgWnd->fontRect(msgtext.c_str());

			//

			msgItem->setSizeHint(msgSize);
			//会设置消息并调用相应的
			msgWnd->setText(msgtext.c_str(), time, msgSize, QChatMsgWnd::ChatMsg_Other);
			//关联项与窗口
			ses->m_MsgWndList->setItemWidget(msgItem, msgWnd);
		}

		if (msgtype == 1)
		{
			neb::CJsonObject json;
			if (!json.Parse(msgtext))
			{
				return;
			}

			std::string filename_server = "";
			std::string filename_client = "";
			std::string filesize = "";

			if (!json.Get("filename_server", filename_server))
			{
				return;
			}

			if (!json.Get("filename_client", filename_client))
			{
				return;
			}

			if (!json.Get("filesize", filesize))
			{
				return;
			}


			////接收端是一个文件
			QChatFileOuterWnd* fileWnd = new QChatFileOuterWnd(nullptr, sendid, recvid);
			fileWnd->m_innerWnd->m_fileName->setText(filename_client.c_str());
			fileWnd->m_innerWnd->m_fileSize->setText(filesize.c_str());
			fileWnd->m_innerWnd->m_sendState->setText("等待下载...");
			fileWnd->m_innerWnd->m_serveFilePath = filename_server.c_str();
			fileWnd->m_innerWnd->recvFileShow();
			//

			fileWnd->setFixedWidth(ses->m_MsgWndList->width());
			QListWidgetItem* fileItem = new QListWidgetItem(ses->m_MsgWndList);
			QSize fileWndSize(fileWnd->width(), 100 + 20);
			fileItem->setSizeHint(fileWndSize);
			ses->m_MsgWndList->setItemWidget(fileItem, fileWnd);
		}

		ses->m_MsgWndList->scrollToBottom();
	}
}

void QMainWnd::cs_msg_update_sessionlist(neb::CJsonObject& msg)
{
	requestSessionList();
}

void QMainWnd::cs_msg_update_grouplist(neb::CJsonObject& msg)
{
	requestGroupList();
}

void QMainWnd::cs_msg_update_friendlist(neb::CJsonObject& msg)
{
	requestFriendList();
}

void QMainWnd::requestHeadImg()
{
	QString imgurl = QString("http://49.232.169.205:80/UploadDemo/img/u%1.png").arg(m_userid);
	m_networkMgr->get(QNetworkRequest(QUrl(imgurl)));
}

void QMainWnd::requestFriendList()
{
	//
	neb::CJsonObject json;
	json.Add("ownerid", QMainWnd::getInstance()->m_userid);
	QWSClientMgr::getInstance()->request("cs_msg_get_friendslist", json, [this](neb::CJsonObject& msg)
		{
			//QMessageBox::information(nullptr, "info", msg.ToString().c_str());
			if (!msg["data"].IsArray())
			{
				return;
			}

			for (int i = 0; i < msg["data"].GetArraySize(); i++)
			{
				neb::CJsonObject json1;
				if (!msg["data"].Get(i, json1))
				{
					continue;
				}

				int friendid = -1;
				if (!json1.Get("friendid", friendid))
				{
					continue;
				}


				std::string friendusername = "";
				if (!json1.Get("friendusername", friendusername))
				{
					continue;
				}

				bool has = false;
				int count = m_commContactsListWnd->m_listWidget->count();
				for (int i = 0; i < count; i++)
				{
					QListWidgetItem* pitem = m_commContactsListWnd->m_listWidget->item(i);
					QCommContactItemWnd* pWnd = dynamic_cast<QCommContactItemWnd*>(m_commContactsListWnd->m_listWidget->itemWidget(pitem));
					if (pWnd->m_friendId == friendid)
					{
						has = true;
					}
				}

				if (!has)
				{
					m_commContactsListWnd->addContactsItem("./img/head2.png", friendusername.c_str(), false, friendid);
				}

			}

		});
}

void QMainWnd::requestSessionList()
{
	//向远端请求会话列表
	neb::CJsonObject json;
	json.Add("ownerid", QMainWnd::getInstance()->getInstance()->m_userid);
	QWSClientMgr::getInstance()->request("cs_msg_get_sessionlist", json, [this](neb::CJsonObject& msg)
		{
			//QMessageBox::information(nullptr, "info", msg.ToString().c_str());
			qDebug() << "msg:" << msg.ToString().c_str();
			//向会话列表中添加一些数据
			if (!msg["data"].IsArray())
			{
				return;
			}

			for (int i = 0; i < msg["data"].GetArraySize(); i++)
			{
				neb::CJsonObject json1;
				if (!msg["data"].Get(i, json1))
				{
					continue;
				}

				int sesid = -1;
				if (!json1.Get("id", sesid))
				{
					continue;
				}

				std::string recentlymsg;
				if (!json1.Get("recentlymsg", recentlymsg))
				{
					continue;
				}

				std::string recvusername;
				if (!json1.Get("recvusername", recvusername))
				{
					continue;
				}

				int64_t sendid = -1;
				int64_t recvid = -1;

				//
				if (!json1.Get("sendid", sendid))
				{
					continue;
				}


				if (!json1.Get("recvid", recvid))
				{
					continue;
				}

				int isgroupses = -1;
				if (!json1.Get("isgroupses", isgroupses))
				{
					continue;
				}

				bool isGroup = false;
				if (isgroupses == 1)
				{
					isGroup = true;
				}

				{
					bool has1 = false;
					int count = m_commMsgListWnd->m_listWidget->count();
					for (int i = 0; i < count; i++)
					{
						QListWidgetItem* pitem = m_commMsgListWnd->m_listWidget->item(i);
						QCommMsgItemWnd* pWnd = dynamic_cast<QCommMsgItemWnd*>(m_commMsgListWnd->m_listWidget->itemWidget(pitem));
						if (pWnd->m_sesId == sesid)
						{
							has1 = true;
							break;
						}
					}

					if (!has1)
					{
						//
						if (isGroup)
						{
							m_commMsgListWnd->addMsgItem("./img/groupHead.png", recvusername.c_str(), recentlymsg.c_str(), sesid, recvid, isGroup);
						}
						else
						{
							m_commMsgListWnd->addMsgItem("./img/head1.png", recvusername.c_str(), recentlymsg.c_str(), sesid, recvid, isGroup);

						}
					}
				}

				{
					bool has2 = false;

					int count = m_sLayout2->count();
					for (int i = 0; i < count; i++) {

						if (i <= 1) {
							continue;
						}

						QLayoutItem* item = m_sLayout2->layout()->itemAt(i);
						QSessionWnd* sesWnd = dynamic_cast<QSessionWnd*>(item->widget());
						if (sesWnd->m_sesId == sesid) {
							has2 = true;
							break;
						}
					}

					if (!has2)
					{
						QSessionWnd* sesWnd = new QSessionWnd();
						sesWnd->m_sesTopWnd->m_titleLabel->setText(recvusername.c_str());
						sesWnd->m_sesId = sesid;
						sesWnd->m_recvId = recvid;
						sesWnd->m_isGroupSes = isgroupses;
						
						// 不是群组会话，应该隐藏more按钮
						if (!isgroupses) 
						{
							sesWnd->m_sesTopWnd->m_moreBtn->hide();
						}

						connect(sesWnd->m_sesTopWnd->m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
						connect(sesWnd->m_sesTopWnd->m_maxBtn, SIGNAL(clicked()), this, SLOT(maxWnd()));
						connect(sesWnd->m_sesTopWnd->m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));
						m_sLayout2->addWidget(sesWnd);
					}

				}
			}

		});
}

void QMainWnd::requestGroupList()
{
	neb::CJsonObject json;
	json.Add("ownerid", QMainWnd::getInstance()->getInstance()->m_userid);
	QWSClientMgr::getInstance()->request("cs_msg_get_groupList", json, [this](neb::CJsonObject& msg)
		{
			qDebug() << "requestGroupList:" <<msg.ToString().c_str();
			//先判断传入是否是msg["data"]是否是array
			if (!msg["data"].IsArray())
			{
				return;
			}
			
			for (int i = 0; i < msg["data"].GetArraySize(); i++)
			{
				//
				neb::CJsonObject  tempJson;
				if (!msg["data"].Get(i, tempJson))
				{
					continue;
				}

				//获取groupname，获取群组的groupid
				//groupname
				std::string groupname = "";
				if (!tempJson.Get("groupname", groupname))
				{
					continue;
				}

				//获取到groupid
				int groupid = -1;
				if (!tempJson.Get("groupid", groupid))
				{
					continue;
				}

				//判断群聊列表中是否含有此群聊
				int count = m_commGroupsListWnd->m_listWidget->count();
				bool has = false;
				for (int i = 0; i < count; i++)
				{
					QListWidgetItem* pitem = m_commGroupsListWnd->m_listWidget->item(i);
					QCommGroupItemWnd* pWnd = dynamic_cast<QCommGroupItemWnd*>(m_commGroupsListWnd->m_listWidget->itemWidget(pitem));
					if (pWnd->m_groupId == groupid)
					{
						has = true;
						break;
					}
				}

				if (!has)
				{
					m_commGroupsListWnd->addGroupItem("./img/groupHead.png", groupname.c_str(), groupid);
				}
			}
		});
}

void QMainWnd::closeWnd()
{
	qApp->quit();
}

void QMainWnd::minWnd()
{
	showMinimized();
}

void QMainWnd::maxWnd()
{
	//暂时去掉窗口最大化
	//showMaximized();
}

void QMainWnd::mouseMoveEvent(QMouseEvent* event)
{
	if (m_bPress)
	{
		move(event->pos() - m_poPress + pos());
	}
}

void QMainWnd::mousePressEvent(QMouseEvent* event)
{
	m_bPress = true;
	m_poPress = event->pos();
}

void QMainWnd::mouseReleaseEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
	m_bPress = false;
}

void QMainWnd::slot_sesIdToIndex(int sesid)
{
	int layoutId = 0;
	if (sesid != 0) 
	{
		int count = m_sLayout2->count();
		for (int i = 0; i < count; i++) 
		{
			if (i <= 1) 
			{
				continue;
			}

			QLayoutItem* item = m_sLayout2->layout()->itemAt(i);
			QSessionWnd* sesWnd = dynamic_cast<QSessionWnd*>(item->widget());
			if (sesWnd->m_sesId == sesid)
			{
				layoutId = i;
				break;
			}
		}
	}
	
	if (sesid!=0) 
	{
		m_lastSesId = layoutId;
	}

	m_sLayout2->setCurrentIndex(layoutId);
}

void QMainWnd::slot_sendMsgBtnClick(QMap<QString, QString> infoMap)
{
	bool has = false;
	int sesid = -1;
	int count = m_commMsgListWnd->m_listWidget->count();
	for (int i = 0; i < count; i++)
	{
		QListWidgetItem* pitem = m_commMsgListWnd->m_listWidget->item(i);
		QCommMsgItemWnd* pWnd = dynamic_cast<QCommMsgItemWnd*>(m_commMsgListWnd->m_listWidget->itemWidget(pitem));
		QString namestr = pWnd->m_name->text();
		if (namestr == infoMap["name"])
		{
			m_commMsgListWnd->m_listWidget->setCurrentItem(pitem);
			sesid = pWnd->m_sesId;
			has = true;
			break;
		}
	}
	
	if (has)
	{
		m_lastSesId = sesid;
		m_toolWnd->m_msgBtn->click();
		slot_sesIdToIndex(sesid);
	}
}

void QMainWnd::slot_toolWndPageChanged(int page)
{
	m_sLayout1->setCurrentIndex(page);
	if (page == 0) 
	{
		requestSessionList();
		if (m_lastSesId != -1) 
		{//上次的会话窗口
			slot_sesIdToIndex(m_lastSesId);
		}
		else 
		{
			//设置背景图片打开
			m_commContactInfo->showBgPng();
		}
	}
	
	if(page==1)
	{//请求好友信息
		requestFriendList();
		m_commContactInfo->showBgPng();
		slot_sesIdToIndex(0);
	}

	if (page == 2)
	{
		//预留位置
	}
}

void QMainWnd::slot_replyFinished(QNetworkReply* reply)
{
	if (reply->error() == QNetworkReply::NoError)
	{
		QPixmap pixmap;
		pixmap.loadFromData(reply->readAll());
		pixmap = pixmap.scaled(40, 40);
		m_toolWnd->m_headImg = pixmap;
		QDataManager::getInstance()->m_UserId2HeadImgMap[m_userid] = m_toolWnd->m_headImg;
	    m_toolWnd->m_headUrlLabel->setPixmap(m_toolWnd->m_headImg);
	}
}

//void QMainWnd::paintEvent(QPaintEvent* event)
//{
//	QPainterPath path;
//	path.setFillRule(Qt::WindingFill);
//	QRectF rect(10, 10, this->width() - 20, this->height() - 20);
//	path.addRoundRect(rect, 8, 8);
//
//	QPainter painter(this);
//	painter.setRenderHint(QPainter::Antialiasing, true);
//	painter.fillPath(path, QBrush(Qt::white));
//
//	QColor color(0, 0, 0, 50);
//	for (int i = 0; i < 10; i++) {
//		QPainterPath path;
//		path.setFillRule(Qt::WindingFill);
//		path.addRect(10 - i, 10 - i, this->width() - (10 - i) * 2, this->height() - (10 - i) * 2);
//		color.setAlpha(150 - qSqrt(i) * 50);
//		painter.setPen(color);
//		painter.drawPath(path);
//	}
//}
