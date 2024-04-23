#pragma once

#include "QPch.h"

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QVector>
#include <QNetworkAccessManager>

#include "SelfLabel.h"
#include "UserInfoWnd.h"
#include "PictureToolWnd.h"
#include <QNetworkReply>
#include "SelectMoreWnd.h"

/*
 * 最右边工具栏窗口
 */
class ToolWnd : public QWidget
{
    // 添加消息映射支持
    Q_OBJECT
  public:
    // 工具栏窗口
    ToolWnd(QWidget* p = nullptr);
  signals:
    void signalToolWndPageChanged(int num);
  public slots:
    void slotOnClickMsgBtn();
    void slotOnClickContactsBtn();
    void slotOnClickHeadUrlLabel();
    void slotOnClickChangeHeadImgBtn();
    void slotOnClickGroupsBtn();
    void slotOnClickMoreBtn();

  public:
    int m_selectIndex;
    QVBoxLayout* m_vBoxLayout;
    QPushButton* m_msgBtn;
    QPushButton* m_contactsBtn;
    QPushButton* m_groupsBtn;
    SelfLabel* m_headUrlLabel;
    QVector<QPushButton*> m_btnVct;
    UserInfoWnd* m_userInfoWnd;
    PictureToolWnd* m_pictureToolWnd;
    QPixmap m_headImg;
    QPushButton* m_moreBtn;
    SelectMoreWnd* m_selectMoreWnd;
};