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
    //��ϵ��ͷ��
    QLabel* m_headUrl;
    //��ϵ������
    QLabel* m_name;
    //�Ƿ����µ�������һ����Ϣ
    bool m_bNewFriend = false;
    //��ʶ��ϵ�˵�id
    int m_friendId = -1;
    QNetworkAccessManager* m_networkMgr;
};