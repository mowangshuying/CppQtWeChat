#pragma once

#include "QPch.h"

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class CommGroupItemWnd : public QWidget
{
    Q_OBJECT
  public:
    CommGroupItemWnd(QWidget* p);

    CommGroupItemWnd(QWidget* p, const char* headUrl, const char* name, int groupId = -1);

    ~CommGroupItemWnd();
  public slots:
    void slotReplyFinished(QNetworkReply* reply);

  public:
    QHBoxLayout* m_hLayout;
    // 联系人头像
    QLabel* m_groupImgUrl;
    // 联系人名字
    QLabel* m_groupName;
    // 标识联系人的id
    int m_groupId = -1;
    // 用于请求头像信息
    QNetworkAccessManager* m_networkMgr;
};