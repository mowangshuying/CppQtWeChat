#pragma once

#include "QPch.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget>  //�����б�

#include "QSessionTopWnd.h"
#include "QSessionToolBar.h"
#include "QGroupInfoWnd.h"
#include <QListWidget>
#include <QFileInfo>

// ֧�����ͼƬ���ı���
#include "QSelfTextEdit.h"

/*
 * �Ự�Ĵ��� sesId����,�ұ���Ϣ����С����ѡ��Ҳ��һ��id��ͨ��id�����˴���Ϣ
 * ���޸Ķ�Ӧ�Ĵ�����Ϣ
 */

class QSessionWnd : public QWidget
{
    //�����Ϣӳ��֧��
    Q_OBJECT
public:
    //�Ự����
    QSessionWnd(QWidget* p = nullptr);
public slots:
    void slotSendTextBtnClick();
    void slotEmoijClicked(QString str);
    void slotMoreBtnClick();
    void slotVoiceTelPhoneBtnClick();
    void slotUpdateGroupName(QString groupName);

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

    QString getFileSize(QFileInfo& fileInfo);

    void resizeEvent(QResizeEvent* event) override;

public:
    void sendMsgToUser(neb::CJsonObject json, QString msgText);

    void sendMsgToGroup(neb::CJsonObject json, QString msgText);

    void setTitle(QString title)
    {
        m_sesTopWnd->m_titleLabel->setText(title);
    }

public:
    // QHBoxLayout* m_hLayout;
    // more��ť����ʱ�����ʾ�������
    QGroupInfoWnd* m_groupInfoWnd;
    //���ڲ��ֹ���
    QVBoxLayout* m_vLayout;
    //�Ự���ϷŴ���
    QSessionTopWnd* m_sesTopWnd;
    //�����б���Ϣ

    QListWidget* m_MsgWndList;

    //��Ϣ�Ự��ToolBar
    QSessionToolBar* m_sesToolBar;
    //��Ҫ���͵�����
    QSelfTextEdit* m_sendTextEdit;
    //���ڷ������ֵİ�ť
    QPushButton* m_sendTextBtn;
    //���ڿ���
    QHBoxLayout* m_hLayout1;

    //�Ựid
    qint64 m_sesId;
    //�Ự����
    QString m_name;
    //����˷���
    int64_t m_recvId;

    //�Ƿ���Ⱥ�Ự
    bool m_isGroupSes = false;
    bool m_isClicked = false;
};