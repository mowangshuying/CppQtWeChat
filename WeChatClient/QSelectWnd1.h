#pragma once

//设置编码格式为utf-8
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QEvent>

#include "QFindFriendOrGroupWnd.h"
#include "QCreateGroupWnd.h"

class QSelectWnd1 : public QWidget {
	Q_OBJECT
protected:
	bool event(QEvent* event);
public:
	QSelectWnd1(QWidget* p);

public slots:
	void slot_addContactsOrGroupBtnClick();
	void slot_createGroup();
public:
	//将玩家的数据设置到QListWidget中
	//void updateData();
public:
	QVBoxLayout* m_vLayout;
	QPushButton* m_addContactsOrGroupBtn;
	QPushButton* m_createGroupBtn;
	QFindFriendOrGroupWnd* m_findFriendOrGroupWnd;
	QCreateGroupWnd* m_crateGroupWnd;
};