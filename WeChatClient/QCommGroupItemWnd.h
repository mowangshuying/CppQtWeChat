#pragma once

#include "QPch.h"

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class QCommGroupItemWnd : public QWidget
{
    Q_OBJECT
public:
    QCommGroupItemWnd(QWidget* p);

    QCommGroupItemWnd(QWidget* p, const char* headUrl, const char* name, int groupId = -1);
public slots:
    void slotReplyFinished(QNetworkReply* reply);

public:
    QHBoxLayout* m_hLayout;
    //��ϵ��ͷ��
    QLabel* m_groupImgUrl;
    //��ϵ������
    QLabel* m_groupName;
    //��ʶ��ϵ�˵�id
    int m_groupId = -1;
    //��������ͷ����Ϣ
    QNetworkAccessManager* m_networkMgr;
};