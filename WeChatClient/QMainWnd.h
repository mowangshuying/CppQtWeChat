#pragma once

#include "QPch.h"

#include <map>
#include <QWidget>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QNetworkReply>

#include "QToolWnd.h"
#include "QCommListWnd.h"
#include "QSessionWnd.h"
#include "QCommContactInfoWnd.h"
#include "QGroupInfoWnd.h"
#include "QDealNewFriendsApplyWnd.h"
#include "./json/CJsonObject.hpp"
#include <QSystemTrayIcon>
#include <QMenu>
#include "QVoiceTelphoneWnd.h"
#include "QSettingWnd.h"

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

    //~QMainWnd();

    enum BorderArea
    {
        BorderAreaNone = 0,
        BorderAreaTop,
        BorderAreaBottom,
        BorderAreaLeft,
        BorderAreaRight,
        BorderAreaTopLeft,
        BorderAreaTopRight,
        BorderAreaBottomLeft,
        BorderAreaBottomRight,
    };

public:
    static QMainWnd* getMainWnd();

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

    bool hasSessionWndBySessionId(int sesid);

    //����Ⱥ����Ϣ
    void requestGroupList();

    //
    void UpdateBorderArea(QPoint pos);
    void UpdateCursor();
    void UpdateWindowByBorderArea();

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

    void adjustWndSizeByMouseMove(QMouseEvent* event);

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
    QToolWnd* m_toolWnd;

    QCommListWnd* m_commMsgListWnd;
    QCommListWnd* m_commContactsListWnd;
    QCommListWnd* m_commGroupsListWnd;
    QCommListWnd* m_commSearchListWnd;

    QCommContactInfoWnd* m_commContactInfo;

    QDealNewFriendsApplyWnd* m_dealNewFriendsApplyWnd;

    QSessionWnd* m_sessionWnd;
    QStackedLayout* m_sMiddleLayout;
    QStackedLayout* m_sRightLayout;

    int m_lastSesId = -1;
    int m_lastContactId = -1;

    int m_currentSesId = 0;

    bool m_bLeftBtnPress = false;
    QPoint m_leftBtnPressPoint;

    static QMainWnd* m_mainWnd;

    //�û���
    QString m_username;
    //�û�id
    int64_t m_userid;
    QNetworkAccessManager* m_networkMgr;

    BorderArea m_borderArea;

    // ϵͳ���̹���
    QSystemTrayIcon* m_systemTrayIcon;
    QMenu* m_systemTrayIconMenu;
    QAction* m_systemTrayIconExitAction;
    QAction* m_systemTrayIconShowMainWndAction;

    QVoiceTelphoneWnd* m_voiceTelphoneWnd;
    QSettingWnd* m_settingWnd;
};
