#pragma once

#include "QPch.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget>  //窗口列表

#include "QSessionTopWnd.h"
#include "QSessionToolBar.h"
#include "QGroupInfoWnd.h"
#include <QListWidget>
#include <QFileInfo>

// 支持黏贴图片的文本框
#include "QSelfTextEdit.h"

/*
 * 会话的窗口 sesId作用,右边信息栏的小窗口选项也有一个id，通过id关联彼此信息
 * 来修改对应的窗口信息
 */

class QSessionWnd : public QWidget
{
    //添加消息映射支持
    Q_OBJECT
  public:
    //会话窗口
    QSessionWnd(QWidget* p = nullptr);
    ~QSessionWnd();
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

    void dealMsgTime();

  public:
    // QHBoxLayout* m_hLayout;
    // more按钮按下时候会显示这个窗口
    QGroupInfoWnd* m_groupInfoWnd;
    //窗口布局管理
    QVBoxLayout* m_vLayout;
    //会话的上放窗口
    QSessionTopWnd* m_sesTopWnd;
    //窗口列表信息

    QListWidget* m_MsgWndList;

    //消息会话中ToolBar
    QSessionToolBar* m_sesToolBar;
    //需要发送的文字
    QSelfTextEdit* m_sendTextEdit;
    //用于发送文字的按钮
    QPushButton* m_sendTextBtn;
    //用于控制
    QHBoxLayout* m_hLayout1;

    //会话id
    qint64 m_sesId;
    //会话名字
    QString m_name;
    //向何人发送
    int64_t m_recvId;

    // 上次显示时间日期的时间
    QDateTime m_lastMsgDateTime;

    //是否是群会话
    bool m_isGroupSes = false;
    bool m_isClicked = false;
};