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
	//��Զ������ͼƬ��Ϣ
	void requestHeadImg();
public slots:
	void slot_replyFinished(QNetworkReply* reply);
public:
	QHBoxLayout* m_hLayout;//
	QLabel* m_headImg;//ͷ��
	QLabel* m_name;   //����
	int64_t m_friendId;//����id
	QNetworkAccessManager* m_networkMgr;
};