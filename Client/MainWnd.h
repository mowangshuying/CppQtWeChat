#pragma once

#include "def.h"

#include <FramelessHelper/Widgets/FramelessWidget>

#include <map>
#include <QWidget>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QNetworkReply>

#include "ToolWnd.h"
#include "CommListWnd.h"
#include "SessionWnd.h"
#include "CommContactInfoWnd.h"
#include "GroupInfoWnd.h"
#include "DealNewFriendsApplyWnd.h"
#include "./json/CJsonObject.hpp"
#include <QSystemTrayIcon>
#include <QMenu>
// #include "VoiceTelphoneWnd.h"
#include "SettingWnd.h"

/*
 * 主窗口窗口只能有一个，将主窗口设置为单例对象
 */
class MainWnd : public FRAMELESSHELPER_PREPEND_NAMESPACE(FramelessWidget)
{
    // 添加消息映射支持
    Q_OBJECT
  private:
    // 会话窗口
    MainWnd(QWidget* p = nullptr);

    void makeSystemTray();

  public:
    ~MainWnd();

  public:
    static MainWnd* getMainWnd();

    // 接收到别人发送的消息
    void cs_msg_sendmsg(neb::CJsonObject& json);
    // 接收到群组中发送的消息
    void cs_msg_sendgroupmsg(neb::CJsonObject& msg);
    // 接收到更新会话的消息
    void cs_msg_update_sessionlist(neb::CJsonObject& msg);
    // 接收到更新群聊列表消息
    void cs_msg_update_grouplist(neb::CJsonObject& msg);
    // 接收到更新联系人的消息
    void cs_msg_update_friendlist(neb::CJsonObject& msg);

    void requestHeadImg();
    void requestFriendList();
    void requestSessionList();

    bool hasSessionWndBySessionId(int sesid);

    // 请求群组消息
    void requestGroupList();

    void setUserIdAndName(int64_t userId, QString username)
    {
        m_username = username;
        m_userid = userId;
    }

    void request()
    {
        requestHeadImg();
        requestFriendList();
        requestSessionList();
        requestGroupList();
    }

  public slots:
    void closeWnd();
    void minWnd();
    void maxWnd();
    void showNormalWnd();

    void mouseMoveEvent(QMouseEvent* event);

    //  void adjustWndSizeByMouseMove(QMouseEvent* event);

    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void slotSesIdToIndex(int sesid);

    void slotSendMsgBtnClick(QMap<QString, QString> infoMap);
    void slotToolWndPageChanged(int page);
    void slotReplyFinished(QNetworkReply* reply);
    void slotOnSystemTrayIconClick(QSystemTrayIcon::ActivationReason reason);
    void slotOnSettingBtnClick();
    void slotSearchText(QString searchText);

  public:
    QWidget* m_centerWnd;

    QHBoxLayout* m_hLayout;
    ToolWnd* m_toolWnd;

    CommListWnd* m_commMsgListWnd;
    CommListWnd* m_commContactsListWnd;
    CommListWnd* m_commGroupsListWnd;
    CommListWnd* m_commSearchListWnd;

    CommContactInfoWnd* m_commContactInfo;

    DealNewFriendsApplyWnd* m_dealNewFriendsApplyWnd;

    SessionWnd* m_sessionWnd;
    QStackedLayout* m_sMiddleLayout;
    QStackedLayout* m_sRightLayout;

    int m_lastSesId = -1;
    int m_lastContactId = -1;

    int m_currentSesId = 0;

    bool m_bLeftBtnPress = false;
    QPoint m_leftBtnPressPoint;

    static MainWnd* m_mainWnd;

    // 用户名
    QString m_username;
    // 用户id
    int64_t m_userid;
    QNetworkAccessManager* m_networkMgr;

    //  BorderArea m_borderArea;

    // 系统托盘功能
    QSystemTrayIcon* m_systemTrayIcon;
    QMenu* m_systemTrayIconMenu;
    QAction* m_systemTrayIconExitAction;
    QAction* m_systemTrayIconShowMainWndAction;

    // VoiceTelphoneWnd* m_voiceTelphoneWnd;
    SettingWnd* m_settingWnd;
};
