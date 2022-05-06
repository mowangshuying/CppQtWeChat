#pragma once

//���ñ����ʽΪutf-8
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class QDealNewFriendsApplyItemWnd : public QWidget
{
	Q_OBJECT
public:
	//QDealNewFriendsApplyItemWnd(QWidget* p = nullptr);
	
	QDealNewFriendsApplyItemWnd(QWidget* p = nullptr, const char* headurl = "", const char* name = "",
		const char* msg = "", int state = 0,int id = 0,bool isApplyer = false, int userid=0);
public slots:
	void slot_onAgreeBtnClick();
	void slot_onRefuseBtnClick();
public:
	QVBoxLayout* m_vLayout;
	QHBoxLayout* m_hLayout;
	QLabel* m_headurl;
	QLabel* m_name;
	QLabel* m_msg;
	//ͬ����Ǹ���ť
	QPushButton* m_AgreeBtn;
	QPushButton* m_refuseBtn;
	//״̬
	int m_state;
	//���ڵ�id
	int m_id;
	//
	int m_userid;
	//�Ƿ���������
	bool m_isApplyer;
};