#pragma once

//���ñ����ʽΪutf-8
#pragma execution_character_set("utf-8")

#include <map>
#include <QWidget>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QNetworkReply>

#include "QToolWnd.h"
#include "QCommListWnd.h"
#include "QSessionWnd.h"
#include "QCommContactInfo.h"
#include "QGroupInfoWnd.h"
#include "QDealNewFriendsApplyWnd.h"
#include "./json/CJsonObject.hpp"


/*
* �����ڴ���ֻ����һ����������������Ϊ��������
*/
class QMainWnd : public QWidget
{
	//�����Ϣӳ��֧��
	Q_OBJECT
private:
	//�Ự����
	QMainWnd(QWidget* p = nullptr);

public:
	static QMainWnd* getSinletonInstance() {
		if (m_mainWnd == nullptr)
		{
			m_mainWnd = new QMainWnd();
		}
		return m_mainWnd;
	}
	
	//���յ����˷��͵���Ϣ
	void cs_msg_sendmsg(neb::CJsonObject& json);
	//���յ�Ⱥ���з��͵���Ϣ
	void cs_msg_sendgroupmsg(neb::CJsonObject& msg);
	//���յ����»Ự����Ϣ
	void cs_msg_update_sessionlist(neb::CJsonObject& msg);
	//���յ�����Ⱥ���б���Ϣ
	void cs_msg_update_grouplist(neb::CJsonObject& msg);
	//���յ�������ϵ�˵���Ϣ
	void cs_msg_update_friendlist(neb::CJsonObject& msg);

	void requestHeadImg();
	void requestFriendList();
	void requestSessionList();
	//����Ⱥ����Ϣ
	void requestGroupList();

public slots:
	void closeWnd();
	void minWnd();
	void maxWnd();

	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

	void slot_sesIdToIndex(int sesid);

	void slot_sendMsgBtnClick(QMap<QString, QString> infoMap);
	void slot_toolWndPageChanged(int page);
	void slot_replyFinished(QNetworkReply* reply);
	//void slot_moreBtnClicked();
public:
	QHBoxLayout* m_hLayout;
	QToolWnd* m_toolWnd;

	QCommListWnd* m_commMsgListWnd;
	QCommListWnd* m_commContactsListWnd;
	QCommListWnd* m_commGroupsListWnd;
	
	QCommContactInfo* m_commContactInfo;

	QDealNewFriendsApplyWnd* m_dealNewFriendsApplyWnd;

	QSessionWnd* m_sessionWnd;
	QStackedLayout *m_sLayout1;
	QStackedLayout* m_sLayout2;

	//QGroupInfoWnd* m_groupInfoWnd;

	int m_lastSesId = -1;
	int m_lastContactId = -1;

	int m_currentSesId = 0;

	bool m_bPress = false;
	QPoint m_poPress;

	static QMainWnd* m_mainWnd;

	//�û���
	QString m_username;
	//�û�id
	int64_t m_userid;
	QNetworkAccessManager* m_networkMgr;
};
