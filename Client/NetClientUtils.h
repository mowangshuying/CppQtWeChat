#pragma once

#include "def.h"
#include "json/CJsonObject.hpp"
#include <QWebSocket>
#include <functional>
#include <QTimer>
#include <list>
#include <string>

#define msg_type_req 1
#define msg_type_resp 2
#define msg_type_push 3
#define msg_type_broadcast 4
#define msg_type_push_s 5


class NetClientUtils : public QObject
{
    Q_OBJECT
  private:
    using NetEventCall = std::function<void(neb::CJsonObject&)>;
    class NetEventCaller
    {
      public:
        NetEventCaller(NetEventCall call, std::string rand, std::string cmd) : call(call), m_rand(rand), m_cmd(cmd)
        {
            m_sendTime = QTime::currentTime().msec();
        }

      public:
        NetEventCall call;
        time_t m_sendTime;
        std::string m_rand;
        std::string m_cmd;
    };

  private:
    NetClientUtils();
    ~NetClientUtils();

  public:
    static void init();
    static void deinit();

    static NetClientUtils* getUtils();
    static std::string getRandString();

    static QWebSocket* getWebSock()
    {
        return getUtils()->m_webSock;
    }

    static bool getConn()
    {
        return getUtils()->m_bConn;
    }

    static bool setConn(bool bConn)
    {
        getUtils()->m_bConn = bConn;
    }

    static bool send(const QString& message);
    static void regMsgCall(QString cmd, NetEventCall call);
    static bool transfer(neb::CJsonObject& msg);
    static bool request(const std::string& cmd, neb::CJsonObject& json);
    static bool request(const std::string& cmd, neb::CJsonObject& json, NetEventCall call);

  public slots:
    void onConnected();
    void onDisconnected();
    void onRecvMsg(const QString& message);
    void onTimer();

  protected:
    static NetClientUtils* m_netClientUtils;
    QWebSocket* m_webSock;

    // callers;
    std::list<NetEventCaller> m_netEventCallers;
    
    // timer;
    QTimer* m_timer;
    bool m_bConn;
};
