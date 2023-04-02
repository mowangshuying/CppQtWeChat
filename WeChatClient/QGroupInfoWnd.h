#pragma once

#include "QPch.h"

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QEvent>
#include <QPushButton>
#include <QHBoxLayout>
#include <QScrollArea>

#include "QEditLabel.h"
#include "QSwitchButton.h"
#include "QGroupFriendsWnd.h"

class QGroupInfoWnd : public QWidget
{
    Q_OBJECT
public:
    QGroupInfoWnd(QWidget* p = nullptr);

protected:
    bool event(QEvent* event);

public:
    //滚动条
    QScrollArea* m_scrollArea;

    // 中间窗口
    QWidget* m_centerWnd;
    //暂时用于测试
    // QLabel* m_label;
    QVBoxLayout* m_vLayout;

    //
    QGroupFriendsWnd* m_groupfriendsWnd;
    //此窗口中信息待定,暂时不予处理

    //简单的群信息窗口
    QWidget* m_simpleGroupInfoWnd;
    QVBoxLayout* m_vLayout1;
    //群名
    QLabel* m_groupName1;
    QEditLabel* m_groupName2;

    //群公告
    QLabel* m_groupNotice1;
    // QLabel* m_groupNotice2;
    QEditLabel* m_groupNotice2;
    //备注
    QLabel* m_groupRemarks1;
    QEditLabel* m_groupRemarks2;
    //我在本群的昵称
    QLabel* m_groupRoleName1;
    QEditLabel* m_groupRoleName2;

    //简单群的设置窗口
    QWidget* m_simpleGroupSettingWnd;
    QVBoxLayout* m_vLayout2;

    QHBoxLayout* m_hLayout21;
    QLabel* m_showGroupFriendsRoleNameLabel;
    QSwitchButton* m_showGroupFriendsRoleNameBtn;

    //消息免打扰
    QHBoxLayout* m_hLayout22;
    QLabel* m_MsgFreeLabel;
    QSwitchButton* m_MsgFreeBtn;

    //置顶聊天
    QHBoxLayout* m_hLayout23;
    QLabel* m_topWndLabel;
    QSwitchButton* m_topWndBtn;

    //保存到通讯录
    QHBoxLayout* m_hLayout24;
    QLabel* m_save2AddrBookLabel;
    QSwitchButton* m_save2AddrBookBtn;

    //删除并退出窗口
    QWidget* m_delexitWnd;
    QVBoxLayout* m_vLayout3;
    QPushButton* m_delexitBtn;

    //当前是否是打开状态
    // bool m_isOpen = false;
};
