#pragma once

//设置编码格式为utf-8
#pragma execution_character_set("utf-8")

#include<QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class QCommGroupItemWnd : public QWidget
{
	Q_OBJECT
public:
	QCommGroupItemWnd(QWidget* p);

	QCommGroupItemWnd(QWidget* p, const char* headUrl, const char* name,int groupId = -1);
public slots:
	void slot_replyFinished(QNetworkReply* reply);
public:
	QHBoxLayout* m_hLayout;
	//联系人头像
	QLabel* m_groupImgUrl;
	//联系人名字
	QLabel* m_groupName;
	//标识联系人的id
	int m_groupId = -1;
	//用于请求头像信息
	QNetworkAccessManager *m_networkMgr;
};