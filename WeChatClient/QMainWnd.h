#pragma once

//设置编码格式为utf-8
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
* 主窗口窗口只能有一个，将主窗口设置为单例对象
*/
class QMainWnd : public QWidget
{
	//添加消息映射支持
	Q_OBJECT
private:
	//会话窗口
	QMainWnd(QWidget* p = nullptr);

public:
	static QMainWnd* getSinletonInstance() {
		if (m_mainWnd == nullptr)
		{
			m_mainWnd = new QMainWnd();
		}
		return m_mainWnd;
	}
	
	//接收到别人发送的消息
	void cs_msg_sendmsg(neb::CJsonObject& json);
	//接收到群组中发送的消息
	void cs_msg_sendgroupmsg(neb::CJsonObject& msg);
	//接收到更新会话的消息
	void cs_msg_update_sessionlist(neb::CJsonObject& msg);
	//接收到更新群聊列表消息
	void cs_msg_update_grouplist(neb::CJsonObject& msg);
	//接收到更新联系人的消息
	void cs_msg_update_friendlist(neb::CJsonObject& msg);

	void requestHeadImg();
	void requestFriendList();
	void requestSessionList();
	//请求群组消息
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

	//用户名
	QString m_username;
	//用户id
	int64_t m_userid;
	QNetworkAccessManager* m_networkMgr;
};
