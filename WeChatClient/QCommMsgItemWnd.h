#pragma once

#include "QPch.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QNetworkReply>
#include <QNetworkAccessManager>

/*
 * 消息具体内容类：消息，头像，联系人名字等
 */
class QCommMsgItemWnd : public QWidget
{
    Q_OBJECT
  public:
    QCommMsgItemWnd(QWidget* p);
    QCommMsgItemWnd(QWidget* p,
                    const char* name,
                    const char* msg,
                    int64_t sesid,
                    int64_t userid,
                    bool isGroppMsg);
    ~QCommMsgItemWnd();

    void requestHeadImg(int id, bool isGroupMsg);
    void loadUserHeadPixmap(QNetworkReply* reply);
    void loadGroupHeadPixmap(QNetworkReply* reply);
  public slots:
    void slotReplyFinished(QNetworkReply* reply);

  public:
    QVBoxLayout* m_vLayout;
    QHBoxLayout* m_hLayout;
    QLabel* m_headurl;
    QLabel* m_name;
    QLabel* m_msg;
    int64_t m_userid;
    int64_t m_sesId;
    bool m_isGroupMsg;
    QString m_url;
    QNetworkAccessManager* m_networkMgr;
};