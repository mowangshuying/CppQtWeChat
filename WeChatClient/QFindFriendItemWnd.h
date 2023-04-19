#pragma once

#include "QPch.h"

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

#include "QApplyFriendNextWnd.h"

class QFindFriendItemWnd : public QWidget
{
    Q_OBJECT
public:
    QFindFriendItemWnd(QWidget* p);
    QFindFriendItemWnd(QWidget* p, const char* headUrl, const char* name);
public slots:
    void slotAddFriendBtnClick();

public:
    QHBoxLayout* m_hLayout;
    //��ϵ��ͷ��
    QLabel* m_headUrl;
    //��ϵ������
    QLabel* m_name;
    //������Ѱ�ť
    QPushButton* m_addFriendBtn;
    //��Ӻ��ѵ��û�id
    int64_t m_friendid;
    //��Ӻ����û���
    QString m_username;

    QApplyFriendNextWnd* m_nextWnd;
};