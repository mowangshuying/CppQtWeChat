#pragma once


//���ñ����ʽΪutf-8
#pragma execution_character_set("utf-8")

#include<QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

#include "QNextWnd.h"

class QFindFriendItemWnd :public  QWidget {
	Q_OBJECT
public:
	QFindFriendItemWnd(QWidget* p);
	QFindFriendItemWnd(QWidget* p, const char* headUrl, const char* name);
public slots:
	void slot_addFriendBtnClick();
public:
	QHBoxLayout* m_hLayout;
	//��ϵ��ͷ��
	QLabel* m_headUrl;
	//��ϵ������
	QLabel* m_name;
	//������Ѱ�ť
	QPushButton* m_addFriendBtn;
	//��Ӻ��ѵ��û�id
	int64_t m_friendid;
	//��Ӻ����û���
	QString m_username;

	QNextWnd* m_nextWnd;
};