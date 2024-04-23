#pragma once

#include "QPch.h"

#include <QWidget>
#include <QLabel>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>

#include "SelfSplit.h"

// 添加好友时候，最先展示的窗口，填写一些验证消息
class ApplyFriendInputInfoWnd : public QWidget
{
    Q_OBJECT
  public:
    ApplyFriendInputInfoWnd(QWidget* p = nullptr);

  public:
    QWidget* m_centerWnd;
    // 主要布局
    QHBoxLayout* m_hLayout;
    QVBoxLayout* m_vLayout;

    // 联系人头像
    QLabel* m_headUrl;

    // 角色名
    QLabel* m_userName;
    // 其他信息
    QLabel* m_moreInfo;
    // 用户信息窗口
    QWidget* m_userInfoWnd;
    QVBoxLayout* m_vUserInfoLayout;

    // 左部窗口与右部串口的分割条
    SelfSplit* m_split;

    // 输入验证消息的标签
    QLabel* m_inputMsgLabel;
    // 输入验证消息的文本框
    QTextEdit* m_inputMsgEdit;
};