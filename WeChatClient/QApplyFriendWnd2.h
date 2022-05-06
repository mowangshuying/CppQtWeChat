#pragma once

//设置编码格式为utf-8
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QLabel>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>

class QApplyFriendWnd2 : public QWidget {
	Q_OBJECT
public:
	QApplyFriendWnd2(QWidget* p = nullptr);

public:
	//主要布局
	QHBoxLayout* m_hLayout;

	//左边布局
	//QVBoxLayout* m_vLayout1;
	//右边布局
	QVBoxLayout* m_vLayout;

	//联系人头像
	QLabel* m_headUrl;
	//角色名
	QLabel* m_roleName;
	//其他信息
	QLabel* m_moreInfo;
	//用户信息窗口
	QWidget* m_userInfoWnd;
	QVBoxLayout* m_vUserInfoLayout;
	//
	QLabel* m_inMsgLabel;
	//QTextEdit* m_inMsgEdit;
};