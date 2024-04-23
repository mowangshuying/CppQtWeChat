#pragma once

#include "QPch.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class DealNewFriendsApplyItemWnd : public QWidget
{
    Q_OBJECT
  public:
    DealNewFriendsApplyItemWnd(QWidget* p = nullptr, const char* headurl = "", const char* name = "", const char* msg = "", int state = 0, int id = 0, bool isApplyer = false, int userid = 0);
  public slots:
    void slotOnAgreeBtnClick();
    void slotOnRefuseBtnClick();

  public:
    QVBoxLayout* m_vLayout;
    QHBoxLayout* m_hLayout;
    QLabel* m_headurl;
    QLabel* m_name;
    QLabel* m_msg;
    // 同意的那个按钮
    QPushButton* m_AgreeBtn;
    QPushButton* m_refuseBtn;
    // 状态
    int m_state;
    // 所在的id
    int m_id;
    //
    int m_userid;
    // 是否是申请者
    bool m_isApplyer;
};