#pragma once

//���ñ����ʽΪutf-8
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
	//��ϵ��ͷ��
	QLabel* m_groupImgUrl;
	//��ϵ������
	QLabel* m_groupName;
	//��ʶ��ϵ�˵�id
	int m_groupId = -1;
	//��������ͷ����Ϣ
	QNetworkAccessManager *m_networkMgr;
};