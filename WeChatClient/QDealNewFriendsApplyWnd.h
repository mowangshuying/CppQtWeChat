#pragma once

#include "QPch.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QStackedLayout>
#include <QTabWidget>

class QDealNewFriendsApplyWnd : public QWidget
{
    Q_OBJECT
  public:
    QDealNewFriendsApplyWnd(QWidget* p = nullptr);

  protected:
    void resizeEvent(QResizeEvent* event) override;

  public:
    void addListItem(const char* headurl,
                     const char* name,
                     const char* msg,
                     int state,
                     int id,
                     bool isApplyer,
                     int userid);

    //设置申请列表信息，向远端服务器请求申请列表并添加
    void setFriendApplyList();

    //遍历item,判断是否已经向列表中嵌入这个id
    bool hasThisFriendApplyById(int id);
  public slots:

  public:
    QVBoxLayout* m_vLayout;
    QHBoxLayout* m_hLayout;
    QLabel* m_titileLable;
    QPushButton* m_minBtn;
    QPushButton* m_maxBtn;
    QPushButton* m_closeBtn;

    QTabWidget* m_tabWnd;
    QListWidget* m_listWnd1;
    QListWidget* m_listWnd2;

    bool m_bPress = false;
    QPoint m_poPress;
};