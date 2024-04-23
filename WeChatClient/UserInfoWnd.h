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

  protected:
    bool event(QEvent* event);

  public:
    QWidget* m_centerWnd;
    QVBoxLayout* m_vLayout;
    QHBoxLayout* m_hLayout1;
    QVBoxLayout* m_vLayout1;
    QLabel* m_usernameLabel;
    QLabel* m_userIdLabel;

    QVBoxLayout* m_vLayout2;
    QLabel* m_headLabel;
    QPushButton* m_changeHeadImgBtn;
    QLabel* m_otherLabel;
};
