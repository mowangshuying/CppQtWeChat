#pragma once

#include "QPch.h"

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QEvent>

#include "QFindFriendOrGroupWnd.h"
#include "QCreateGroupWnd.h"

class QSelectAddGroupOrAddFriendWnd : public QWidget
{
    Q_OBJECT
protected:
    bool event(QEvent* event);

public:
    QSelectAddGroupOrAddFriendWnd(QWidget* p);

public slots:
    void slot_addContactsOrGroupBtnClick();
    void slot_createGroup();

public:
    //将玩家的数据设置到QListWidget中
    // void updateData();
public:
    QVBoxLayout* m_vLayout;
    QPushButton* m_addContactsOrGroupBtn;
    QPushButton* m_createGroupBtn;
    QFindFriendOrGroupWnd* m_findFriendOrGroupWnd;
    QCreateGroupWnd* m_crateGroupWnd;
};