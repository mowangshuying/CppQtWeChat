#pragma once

#include "def.h"

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QEvent>

#include "FindFriendOrGroupWnd.h"
#include "CreateGroupWnd.h"

class SelectGroupFriendWnd : public QWidget
{
    Q_OBJECT

  public:
    SelectGroupFriendWnd(QWidget* p);

    bool event(QEvent* event);
  public slots:
    void slotAddContactsOrGroupBtnClick();
    void slotCreateGroup();

  protected:
    QVBoxLayout* m_vMainLayout;
    QWidget* m_centerWnd;
    QVBoxLayout* m_vCenterLayout;
    QPushButton* m_addContactsOrGroupBtn;
    QPushButton* m_createGroupBtn;
    FindFriendOrGroupWnd* m_findFriendOrGroupWnd;
    CreateGroupWnd* m_crateGroupWnd;
};