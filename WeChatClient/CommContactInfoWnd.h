#pragma once

#include "QPch.h"

#include <Qwidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMap>

class CommContactInfoWnd : public QWidget
{
    Q_OBJECT
  public:
    CommContactInfoWnd(QWidget* p = nullptr);
    ~CommContactInfoWnd();
  signals:
    void signalSendMsgBtnClick(QMap<QString, QString> map);
  public slots:
    void slotContactInfoChange(QMap<QString, QString> map);
    void slotSendMsgBtnClick();

  public:
    void showBgPng();
    void hideBgPng();

  public:
    QVBoxLayout* m_vLayout;

    QWidget* m_contentWnd;
    QVBoxLayout* m_vLayout1;

    QHBoxLayout* m_hLayout0;  // 顶部的最大化，最小化按钮等
    QHBoxLayout* m_hLayout1;
    QHBoxLayout* m_hLayout2;
    QHBoxLayout* m_hLayout3;
    QHBoxLayout* m_hLayout4;
    QHBoxLayout* m_hLayout5;

    QPushButton* m_minBtn;
    QPushButton* m_maxBtn;
    QPushButton* m_closeBtn;

    QLabel* m_nickNameLabel;
    QLabel* m_headimgLabel;

    QLabel* m_markNameLabel;
    QLabel* m_rolenameLabel;  // 角色名
    QLabel* m_channelLabel;

    QLabel* m_markNameFillLabel;
    QLabel* m_rolenameFillLabel;
    QLabel* m_channelFillLabel;

    //发送消息窗口
    QPushButton* m_sendMsgBtn;
};
