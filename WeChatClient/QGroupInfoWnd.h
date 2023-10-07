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
#include "QSelfSwitchButton.h"
#include "QGroupFriendsWnd.h"

class QGroupInfoWnd : public QWidget
{
    Q_OBJECT
  public:
    QGroupInfoWnd(QWidget* p = nullptr);

  protected:
    bool event(QEvent* event);

  public:
    void addGroupFriendItem(int64_t ownerId, QString nickName);

    void setGroupId(int64_t groupId)
    {
        m_groupId = groupId;
    }

    int64_t getGroupId()
    {
        return m_groupId;
    }

    void setGroupName(QString groupName)
    {
        m_groupName2->setText(groupName.toStdString().c_str());
    }
  signals:
    void signalUpdateGroupName(QString groupName);
  public slots:
    void slotSetGroupName();

  public:
    QWidget* m_centerWnd;
    //������
    QScrollArea* m_scrollArea;

    // �м䴰��
    QWidget* m_scrollAreaWnd;
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
    QSelfSwitchButton* m_showGroupFriendsRoleNameBtn;

    //��Ϣ�����
    QHBoxLayout* m_hLayout22;
    QLabel* m_MsgFreeLabel;
    QSelfSwitchButton* m_MsgFreeBtn;

    //�ö�����
    QHBoxLayout* m_hLayout23;
    QLabel* m_topWndLabel;
    QSelfSwitchButton* m_topWndBtn;

    //���浽ͨѶ¼
    QHBoxLayout* m_hLayout24;
    QLabel* m_save2AddrBookLabel;
    QSelfSwitchButton* m_save2AddrBookBtn;

    //ɾ�����˳�����
    QWidget* m_delexitWnd;
    QVBoxLayout* m_vLayout3;
    QPushButton* m_delexitBtn;

    int64_t m_groupId;
};
