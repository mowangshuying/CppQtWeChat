#pragma once

#include "def.h"

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QEvent>

#include "FindFriendOrGroupWnd.h"
#include "CreateGroupWnd.h"

class SelectAddGroupOrAddFriendWnd : public QWidget
{
    Q_OBJECT
  protected:
    bool event(QEvent* event);

  public:
    SelectAddGroupOrAddFriendWnd(QWidget* p);

  public slots:
    void slotAddContactsOrGroupBtnClick();
    void slotCreateGroup();

  public:
    // 将玩家的数据设置到QListWidget中
    //  void updateData();
  public:
    QWidget* m_centerWnd;
    QVBoxLayout* m_vLayout;
    QPushButton* m_addContactsOrGroupBtn;
    QPushButton* m_createGroupBtn;
    FindFriendOrGroupWnd* m_findFriendOrGroupWnd;
    CreateGroupWnd* m_crateGroupWnd;
};