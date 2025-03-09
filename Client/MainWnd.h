#pragma once

#include "def.h"

#include <FramelessHelper/Widgets/FramelessWidget>
#include <QHBoxLayout>
#include <QMenu>
#include <QNetworkReply>
#include <QStackedLayout>
#include <QSystemTrayIcon>
#include <QWidget>
#include <map>

#include "./json/CJsonObject.hpp"
#include "CommContactInfoWnd.h"
#include "CommListWnd.h"
#include "DealNewFriendsApplyWnd.h"
#include "GroupInfoWnd.h"
#include "SessionWnd.h"
#include "SettingWnd.h"
#include "ToolWnd.h"
// #include "VoiceTelphoneWnd.h"

// 主窗口为单例对象；
class MainWnd : public FRAMELESSHELPER_PREPEND_NAMESPACE(FramelessWidget)
{
    // 添加消息映射支持
    Q_OBJECT
  private:
    // 会话窗口
    MainWnd(QWidget* p = nullptr);

  public:
    ~MainWnd();
    
    // 构建系统图标;
    void makeSystemTray();

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
        // 获取头像；
        requestHeadImg();
        // 获取好友列表；
        requestFriendList();
        // 获取群组列表；
        requestGroupList();
        // 获取会话列表；
        requestSessionList();
    }

  public slots:
    void closeWnd();
    void minWnd();
    void maxWnd();
    void showNormalWnd();

    void mouseMoveEvent(QMouseEvent* event);
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
    
    
    // 左边部分：工具栏---------------------
    ToolWnd* m_toolWnd;

    // 中间部分：----------------------------
    // 布局；
    QStackedLayout* m_sMiddleLayout;
    // 消息窗口
    CommListWnd* m_commMsgListWnd;
    // 联系人窗口
    CommListWnd* m_commContactsListWnd;
    // 群聊窗口
    CommListWnd* m_commGroupsListWnd;
    // 查找窗口
    CommListWnd* m_commSearchListWnd;

    // 右侧部分 ----------------------------
    QStackedLayout* m_sRightLayout;
    // 联系人页面
    CommContactInfoWnd* m_commContactInfo;
    // 处理添加好友窗口；
    DealNewFriendsApplyWnd* m_dealNewFriendsApplyWnd;
    // 会话窗口
    SessionWnd* m_sessionWnd;

    int m_lastSesId = -1;
    int m_lastContactId = -1;

    int m_currentSesId = 0;

    bool m_bLeftBtnPress = false;
    QPoint m_leftBtnPressPoint;

    // 用户名和用户id;
    QString m_username;
    int64_t m_userid;
    
    QNetworkAccessManager* m_networkMgr;

    // 系统托盘功能 ----------------------------
    QSystemTrayIcon* m_systemTrayIcon;
    QMenu* m_systemTrayIconMenu;
    QAction* m_systemTrayIconExitAction;
    QAction* m_systemTrayIconShowMainWndAction;

    // 语音通话功能;
    // VoiceTelphoneWnd* m_voiceTelphoneWnd;

    // 设置窗口 -------------------------------
    SettingWnd* m_settingWnd;


    // 主窗口静态变量 -------------------------
    static MainWnd* m_mainWnd;
};
