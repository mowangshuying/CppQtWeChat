#pragma once

#include "QPch.h"

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QEvent>
#include <QPushButton>
#include <QHBoxLayout>
#include <QScrollArea>

#include "QEditLabel.h"
#include "QSwitchButton.h"
#include "QGroupFriendsWnd.h"

class QGroupInfoWnd : public QWidget
{
    Q_OBJECT
public:
    QGroupInfoWnd(QWidget* p = nullptr);

protected:
    bool event(QEvent* event);

public:
    //������
    QScrollArea* m_scrollArea;

    // �м䴰��
    QWidget* m_centerWnd;
    //��ʱ���ڲ���
    // QLabel* m_label;
    QVBoxLayout* m_vLayout;

    //
    QGroupFriendsWnd* m_groupfriendsWnd;
    //�˴�������Ϣ����,��ʱ���账��

    //�򵥵�Ⱥ��Ϣ����
    QWidget* m_simpleGroupInfoWnd;
    QVBoxLayout* m_vLayout1;
    //Ⱥ��
    QLabel* m_groupName1;
    QEditLabel* m_groupName2;

    //Ⱥ����
    QLabel* m_groupNotice1;
    // QLabel* m_groupNotice2;
    QEditLabel* m_groupNotice2;
    //��ע
    QLabel* m_groupRemarks1;
    QEditLabel* m_groupRemarks2;
    //���ڱ�Ⱥ���ǳ�
    QLabel* m_groupRoleName1;
    QEditLabel* m_groupRoleName2;

    //��Ⱥ�����ô���
    QWidget* m_simpleGroupSettingWnd;
    QVBoxLayout* m_vLayout2;

    QHBoxLayout* m_hLayout21;
    QLabel* m_showGroupFriendsRoleNameLabel;
    QSwitchButton* m_showGroupFriendsRoleNameBtn;

    //��Ϣ�����
    QHBoxLayout* m_hLayout22;
    QLabel* m_MsgFreeLabel;
    QSwitchButton* m_MsgFreeBtn;

    //�ö�����
    QHBoxLayout* m_hLayout23;
    QLabel* m_topWndLabel;
    QSwitchButton* m_topWndBtn;

    //���浽ͨѶ¼
    QHBoxLayout* m_hLayout24;
    QLabel* m_save2AddrBookLabel;
    QSwitchButton* m_save2AddrBookBtn;

    //ɾ�����˳�����
    QWidget* m_delexitWnd;
    QVBoxLayout* m_vLayout3;
    QPushButton* m_delexitBtn;

    //��ǰ�Ƿ��Ǵ�״̬
    // bool m_isOpen = false;
};
