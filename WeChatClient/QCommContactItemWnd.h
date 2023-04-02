#pragma once

#include "QPch.h"

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class QCommContactItemWnd : public QWidget
{
    Q_OBJECT
public:
    QCommContactItemWnd(QWidget* p);

    QCommContactItemWnd(QWidget* p, const char* headUrl, const char* name, bool bNewFriend = false, int friendid = -1);
public slots:
    void slot_replyFinished(QNetworkReply* reply);

public:
    QHBoxLayout* m_hLayout;
    //联系人头像
    QLabel* m_headUrl;
    //联系人名字
    QLabel* m_name;
    //是否是新的朋友那一栏消息
    bool m_bNewFriend = false;
    //标识联系人的id
    int m_friendId = -1;
    QNetworkAccessManager* m_networkMgr;
};