#pragma once

#include "QPch.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMap>
#include <QString>

#include "QSelectAddGroupOrAddFriendWnd.h"

class QCommListWnd : public QWidget
{
    //�����Ϣӳ��֧��
    Q_OBJECT

  public:
    enum QCommListWndEnum
    {
        MsgItemWndTpye = 0,
        ContactItemWndType = 1,
        GroupItemWndType = 2,
        SearchItemWndType = 3
    };

    QCommListWnd(QWidget* p = nullptr,
                 QCommListWndEnum wndType = MsgItemWndTpye);

  signals:
    void signalCommListChanged(int num);
    void signalContactInfoChange(QMap<QString, QString> infoMap);
    void signalSearchText(QString searchText);
  public slots:
    void slotOnCurrentItemClicked(QListWidgetItem* item);
    //�������Ǹ���ť�Ļ�
    void slotOnStartGroupBtnClicked();

  protected:
    bool eventFilter(QObject* target, QEvent* event);

  public:
    void addMsgItem(const char* name,
                    const char* msg,
                    qint64 sesid,
                    int64_t userid,
                    bool isGroupMsg);
    void addContactsItem(const char* headUrl,
                         const char* name,
                         bool isNewFriend = false,
                         int friendid = -1);
    void addGroupItem(const char* headUrl, const char* name, int groupid);

    bool hasMsgItemBySesId(int64_t sesid);
    bool hasGroupItemByGroupId(int64_t groupid);
    bool hasContactsItemByFriendId(int64_t friendId);

    void setGroupItemNameByGroupId(int64_t grouId, QString groupName);
    void setMsgItemNameBySesId(int64_t sesId, QString msgItemName);

    QVBoxLayout* m_vLayout;
    QHBoxLayout* m_hLayout;

    //������
    QLineEdit* m_searchEdit;

    //��ʼ���Ǹ���ť
    QPushButton* m_startGroupBtn;

    QSelectAddGroupOrAddFriendWnd* m_selectWnd;

    QListWidget* m_listWidget;

    //��������
    QCommListWndEnum m_WndType;
};