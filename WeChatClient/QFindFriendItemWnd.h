#pragma once


//设置编码格式为utf-8
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
	//联系人头像
	QLabel* m_headUrl;
	//联系人名字
	QLabel* m_name;
	//添加朋友按钮
	QPushButton* m_addFriendBtn;
	//添加好友的用户id
	int64_t m_friendid;
	//添加好友用户名
	QString m_username;

	QNextWnd* m_nextWnd;
};