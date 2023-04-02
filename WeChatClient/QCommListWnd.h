#pragma once


#include "QPch.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMap>
#include <QString>

#include "QSelectWnd1.h"

class QCommListWnd : public QWidget
{
	//添加消息映射支持
	Q_OBJECT

public:
	enum QCommListWndEnum
	{
		QCommMsgItemWnd_Tpye		=	0,
		QCommContactItemWnd_Type	=	1,
		QCommGroupItemWnd_Type		=	2,
	};

	QCommListWnd(QWidget* p = nullptr, QCommListWndEnum wndType = QCommMsgItemWnd_Tpye);

signals:
	void commListChanged(int num);
	void signal_contactInfoChange(QMap<QString,QString> infoMap);
public slots:
	void onCurrentItemClicked(QListWidgetItem* item);
	//如果点击那个按钮的话
	void onStartGroupBtnClicked();
public:
	QListWidgetItem* addMsgItem(const char* headUrl, const char* name, const char* msg,qint64 sesid, int64_t userid,bool isGroupMsg);
	void addContactsItem(const char* headUrl, const char* name,bool isNewFriend = false,int friendid = -1);
	void addGroupItem(const char* headUrl, const char* name, int groupid);
	

	QVBoxLayout* m_vLayout;
	QHBoxLayout* m_hLayout;

	//搜索框
	QLineEdit* m_searchEdit;
	//开始的那个按钮
	QPushButton* m_startGroupBtn;

	QSelectWnd1* m_selectWnd1;

	QListWidget* m_listWidget;
	//窗口类型
	QCommListWndEnum m_WndType;
};