#pragma once

//设置编码格式为utf-8
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>

#include "QGroupFriendsItemWnd.h"

class QGroupFriendsWnd : public QWidget 
{
	Q_OBJECT
public:
	QGroupFriendsWnd(QWidget* p = nullptr);
	void addItem(QGroupFriendsItemWnd* w);
public:
	QVBoxLayout* m_vLayout;
	QListWidget* m_listWnd;
};