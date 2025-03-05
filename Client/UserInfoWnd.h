#pragma once

#include "def.h"

#include <QEvent>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QpushButton>

class UserInfoWnd : public QWidget
{
    Q_OBJECT
  public:
    UserInfoWnd(QWidget* p = nullptr);

    bool event(QEvent* event);
  public:
    QVBoxLayout* m_vMainLayout;
    QWidget* m_centerWnd;
    QVBoxLayout* m_vCenterLayout;
    
    QHBoxLayout* m_hTopLayout;
    QVBoxLayout* m_vInfoLayout;
    QHBoxLayout* m_hBtnLayout;

    QLabel* m_usernameLabel;
    QLabel* m_userIdLabel;

  //  QVBoxLayout* m_vLayout2;
    QLabel* m_headLabel;
    QPushButton* m_changeHeadImgBtn;
    QLabel* m_otherLabel;
};
