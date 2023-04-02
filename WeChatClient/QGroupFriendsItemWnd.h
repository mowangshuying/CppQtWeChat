#pragma once


#include "QPch.h"

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class QGroupFriendsItemWnd : public QWidget
{
	Q_OBJECT
public:
	QGroupFriendsItemWnd(QWidget* p = nullptr);
	//向远端请求图片信息
	void requestHeadImg();
public slots:
	void slot_replyFinished(QNetworkReply* reply);
public:
	QHBoxLayout* m_hLayout;//
	QLabel* m_headImg;//头像
	QLabel* m_name;   //名称
	int64_t m_friendId;//好友id
	QNetworkAccessManager* m_networkMgr;
};