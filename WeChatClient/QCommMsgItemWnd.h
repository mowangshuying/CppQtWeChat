#pragma once

//���ñ����ʽΪutf-8
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QNetworkReply>
#include <QNetworkAccessManager>

/*
* ��Ϣ���������ࣺ��Ϣ��ͷ����ϵ�����ֵ�
*/
class QCommMsgItemWnd : public QWidget
{
	Q_OBJECT
public:
	QCommMsgItemWnd(QWidget* p);
	QCommMsgItemWnd(QWidget* p, const char* headUrl, const char* name, const char* msg,int64_t sesid,int64_t userid,bool isGroppMsg);
public slots:
	void slot_replyFinished(QNetworkReply* reply);
public:
	QVBoxLayout* m_vLayout;
	QHBoxLayout* m_hLayout;
	QLabel* m_headurl;
	QLabel* m_name;
	QLabel* m_msg;
	int64_t m_userid;
	int64_t m_sesId;
	bool m_isGroupMsg;
	QNetworkAccessManager* m_networkMgr;
};