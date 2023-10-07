#pragma once

#include "QPch.h"

/*
 * �ṩ�򵥵Ļ���websocket��ͨ������
 */

#include <QWebSocket>

#include <functional>
#include <QTimer>
#include "json/CJsonObject.hpp"

#define msg_type_req 1
#define msg_type_resp 2
#define msg_type_push 3
#define msg_type_broadcast 4
#define msg_type_push_s 5

class QWSClientMgr : public QObject
{
    Q_OBJECT
  private:
    QWSClientMgr();
    ~QWSClientMgr();

  public:
    static void initMgr();
    static void exitMgr();
    static QWSClientMgr* getMgr();

    void sendMsg(const QString& message);

    typedef std::function<void(neb::CJsonObject&)> NetEventCall;
    struct NetEventCallData
    {
        NetEventCall callFun = nullptr;
        time_t dt = 0;
    };

    typedef std::map<QString, NetEventCall> Msg2CallbackMap;
    typedef std::map<int, NetEventCallData> RequestMsg2CallbackMap;

    void regMsgCall(QString cmd, NetEventCall netEventCall);

    void transfer(neb::CJsonObject& msg);

    void request(const std::string& cmd, neb::CJsonObject& json);
    void request(const std::string& cmd,
                 neb::CJsonObject& json,
                 NetEventCall call);

    void onNetMsgDo(int64 msgId, neb::CJsonObject& msgJson);
    void onNetMsgDo(std::string cmd, neb::CJsonObject& msgJson);

  public slots:
    void slotConnected();
    void slotDisconnected();
    void slotRecvMsg(const QString& message);
    void slotTimer();

  public:
    static QWSClientMgr* m_WsClientMgr;

    QWebSocket* m_webSock;
    //����Զ����Ϣ
    Msg2CallbackMap m_Msg2CallbackMap;
    //�����ص��˺���
    RequestMsg2CallbackMap m_RequestMsg2CallbackMap;
    int64 m_MsgId = 0;
    QTimer* m_timer;
    // �Ƿ�������Զ�˵ķ�����
    bool m_bConn;
    int m_time = 0;
};
