#pragma once

#include "QPch.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>

#include "GroupFriendsItemWnd.h"

class GroupFriendsWnd : public QWidget
{
    Q_OBJECT
  public:
    GroupFriendsWnd(QWidget* p = nullptr);
    void addItem(GroupFriendsItemWnd* w);

  public:
    QVBoxLayout* m_vLayout;
    QListWidget* m_listWnd;
};