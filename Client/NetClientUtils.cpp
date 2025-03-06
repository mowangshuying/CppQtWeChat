#include "NetClientUtils.h"
#include <QDebug>
#include <QThread>
#include <QtGlobal>
#include <stdlib.h>
#include <time.h>

NetClientUtils* NetClientUtils::m_netClientUtils = nullptr;

NetClientUtils::NetClientUtils()
{
    m_bConn = false;
    m_webSock = new QWebSocket();

    // 创建一个定时器
    m_timer = new QTimer();
    m_timer->start(1000);

    connect(m_timer, &QTimer::timeout, this, &NetClientUtils::onTimer);
    connect(m_webSock, &QWebSocket::connected, this, &NetClientUtils::onConnected);
    connect(m_webSock, &QWebSocket::disconnected, this, &NetClientUtils::onDisconnected);
    connect(m_webSock, &QWebSocket::textMessageReceived, this, &NetClientUtils::onRecvMsg);

    // 连接远端服务器
    // m_webSock->open(QUrl(CHAT_SERVER_ADDR));
    m_webSock->open(QUrl("ws://127.0.0.1:5000"));
    //LogD << "threadId:" << QThread::currentThread()->currentThreadId();
}

NetClientUtils::~NetClientUtils()
{
    if (m_webSock)
        delete m_webSock;

    if (m_timer)
    {
        m_timer->stop();
        delete m_timer;
    }
}

void NetClientUtils::init()
{
    getUtils();
}

void NetClientUtils::deinit()
{
    if (m_netClientUtils)
    {
        delete m_netClientUtils;
    }
}

NetClientUtils* NetClientUtils::getUtils()
{
    if (m_netClientUtils == nullptr)
    {
        m_netClientUtils = new NetClientUtils();
    }
    return m_netClientUtils;
}

std::string NetClientUtils::getRandString()
{
    char buf[10];
    memset(buf, 0, sizeof(buf));
    static char charS[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e',
        'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    };

    time_t t;
    srand((unsigned)time(&t));

    int i = 0;
    for (; i < 8; i++)
    {
        buf[i] = charS[rand() % sizeof(charS)];
    }

    buf[i] = '\0';
    return std::string(buf);
}

bool NetClientUtils::send(const QString& message)
{
    auto nLen =  m_webSock->sendTextMessage(message);
    return nLen > 0;
}

void NetClientUtils::regMsgCall(QString cmd, NetEventCall call)
{
    NetEventCaller caller(call, "", cmd.toStdString());
    m_netEventCallers.push_back(caller);
}

bool NetClientUtils::transfer(neb::CJsonObject& msg)
{
    QString str = msg.ToString().c_str();
    return send(str);
}

bool NetClientUtils::request(const std::string& cmd, neb::CJsonObject& data)
{
    neb::CJsonObject msg;
    msg.Add("cmd", cmd);
    msg.Add("msgType", "request");
    msg.Add("rand", "");
    msg.Add("timestamp", QTime::currentTime().msec());
    msg.Add("data", data);
    return transfer(msg);
}

bool NetClientUtils::request(const std::string& cmd, neb::CJsonObject& data, NetEventCall call)
{
   neb::CJsonObject msg;
   msg.Add("cmd", cmd);
   msg.Add("msgType", "request");
   std::string randString = getRandString();
   msg.Add("rand", randString);
   msg.Add("timestamp", QTime::currentTime().msec());
   msg.Add("data", data);
   auto ret = transfer(msg);
   if (ret)
   {
       NetEventCaller caller(call, randString, cmd);
       m_netEventCallers.push_back(caller);
   }
   return ret;
}

void NetClientUtils::onConnected()
{
    LogD << "called";
    neb::CJsonObject json;
    request("regClient", json, [=](neb::CJsonObject& msg) { m_bConn = true; });
}

void NetClientUtils::onDisconnected()
{
    LogD << "called";
    m_bConn = false;
}

void NetClientUtils::onRecvMsg(const QString& message)
{
    neb::CJsonObject json;
    if (!json.Parse(message.toStdString()))
    {
        LogE << "json.Parse Message failed.";
        return;
    }

    std::string rand;
    json.Get("rand", rand);

    std::string msgType;
    if (!json.Get("msgType", msgType))
    {
        LogE << "can't get msgType."; 
        return;
    }

    if (msgType != "response")
    {
        LogE << "msgType is not response";
        return;
    }

    std::string cmd;
    if (!json.Get("cmd", cmd))
    {
        LogE << "can't get cmd.";
        return;
    }

    for (auto iter = m_netEventCallers.begin(); iter != m_netEventCallers.end(); iter++) 
    {
        if (iter->m_cmd == cmd && iter->m_rand == rand)
        {
            iter->call(json);
            if (!iter->m_rand.empty())
                m_netEventCallers.erase(iter);
            return;
        }
    }
}

void NetClientUtils::onTimer()
{
    //m_time++;
    if (!m_bConn)
    {
        m_webSock->open(QUrl(CHAT_SERVER_ADDR));
        LogD << "Retry to connect server";
    }
}
