#pragma once

//���ñ����ʽΪutf-8
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QStackedLayout>
#include <QTabWidget>


class QDealNewFriendsApplyWnd : public QWidget
{
	Q_OBJECT
public:
	QDealNewFriendsApplyWnd(QWidget* p = nullptr);
public:
	void addListItem(const char* headurl, const char* name, const char* msg, int state, int id,bool isApplyer,int userid);

	//���������б���Ϣ����Զ�˷��������������б����
	void setFriendApplyList();

	//����item,�ж��Ƿ��Ѿ����б���Ƕ�����id
	bool hasThisFriendApplyById(int id);
public slots:

public:
	QVBoxLayout* m_vLayout;
	QHBoxLayout* m_hLayout;
	QLabel* m_titileLable;
	QPushButton* m_minBtn;
	QPushButton* m_maxBtn;
	QPushButton* m_closeBtn;

	QTabWidget* m_tabWnd;
	QListWidget* m_listWnd1;
	QListWidget* m_listWnd2;

	bool m_bPress = false;
	QPoint m_poPress;

};