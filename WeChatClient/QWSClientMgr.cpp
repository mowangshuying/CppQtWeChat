#include "QWSClientMgr.h"
#include <QDebug>
#include <QThread>

QWSClientMgr* QWSClientMgr::m_WsClientMgr = nullptr;

QWSClientMgr::QWSClientMgr()
{
    m_bConn = false;
    m_webSock = new QWebSocket();

    //创建一个定时器
    m_timer = new QTimer();
    m_timer->start(1000);

    connect(m_timer, &QTimer::timeout, this, &QWSClientMgr::slot_timer);
    connect(m_webSock, &QWebSocket::connected, this, &QWSClientMgr::slot_connected);
    connect(m_webSock, &QWebSocket::disconnected, this, &QWSClientMgr::slot_disconnected);
    connect(m_webSock, &QWebSocket::textMessageReceived, this, &QWSClientMgr::slot_recvMsg);

    //连接远端服务器
    m_webSock->open(QUrl("ws://49.232.169.205:5000"));
    // m_webSock->open(QUrl("ws://127.0.0.1:5000"));
    LogDebug << "threadId:" << QThread::currentThread()->currentThreadId();
}

QWSClientMgr* QWSClientMgr::getInstance()
{
    if (m_WsClientMgr == nullptr)
    {
        m_WsClientMgr = new QWSClientMgr();
    }
    return m_WsClientMgr;
}

void QWSClientMgr::sendMsg(const QString& message)
{
    m_webSock->sendTextMessage(message);
}

void QWSClientMgr::regMsgCall(QString cmd, NetEventCall netEventCall)
{
    m_Msg2CallbackMap[cmd] = netEventCall;
    LogDebug << "threadId = " << QThread::currentThread()->currentThreadId();
}

void QWSClientMgr::transfer(neb::CJsonObject& msg)
{
    QString str = msg.ToString().c_str();
    sendMsg(str);
}

void QWSClientMgr::request(const std::string& cmd, neb::CJsonObject& data)
{
    neb::CJsonObject msg;
    msg.Add("cmd", cmd);
    msg.Add("type", msg_type_req);
    msg.Add("msgId", ++m_MsgId);

    //这里时间是不准确的，需要和服务同步
    int time = QTime::currentTime().msec();

    msg.Add("time", time);
    msg.Add("data", data);

    transfer(msg);
}

void QWSClientMgr::request(const std::string& cmd, neb::CJsonObject& json, NetEventCall call)
{
    request(cmd, json);
    if (call != nullptr)
    {
        NetEventCallData calldata;
        calldata.callFun = call;
        calldata.dt = QTime::currentTime().msec();
        m_RequestMsg2CallbackMap[m_MsgId] = calldata;
    }
}

void QWSClientMgr::onNetMsgDo(int64 msgId, neb::CJsonObject& msgJson)
{
    auto itf = m_RequestMsg2CallbackMap.find(msgId);
    if (itf != m_RequestMsg2CallbackMap.end())
    {
        itf->second.callFun(msgJson);
        m_RequestMsg2CallbackMap.erase(itf);
        return;
    }
    LogDebug << "can not find msgid = " << msgId << " in onNetMsgDo ";
}

void QWSClientMgr::onNetMsgDo(std::string cmd, neb::CJsonObject& msgJson)
{
    QString qcmdStr = cmd.c_str();
    LogDebug << "cmd:" << qcmdStr;
    auto itf = m_Msg2CallbackMap.find(qcmdStr);
    if (itf != m_Msg2CallbackMap.end())
    {
        itf->second(msgJson);
        return;
    }
    LogDebug << "can not find cmd = " << qcmdStr << " in onNetMsgDo ";
}

void QWSClientMgr::slot_connected()
{
    LogDebug << "slot_connected()...";
    LogDebug << "threadId = " << QThread::currentThread()->currentThreadId();
    //向远端服务器发送一个注册消息
    neb::CJsonObject json;
    json.Add("type", "Client");
    json.Add("cckey", "ccmm00@123456");
    request("cs_reg_client", json, [this](neb::CJsonObject& msg) {
        LogDebug << "reg client suc";
        m_bConn = true;
    });
}

void QWSClientMgr::slot_disconnected()
{
    LogDebug << "slot_disconnected...";
    m_bConn = false;
}

void QWSClientMgr::slot_recvMsg(const QString& message)
{
    LogDebug << "threadId = " << QThread::currentThread()->currentThreadId();
    neb::CJsonObject json;
    if (!json.Parse(message.toStdString()))
    {
        LogDebug << "json parse failed in slot_recvMsg";
        return;
    }

    //
    int msgType = 0;
    if (!json.Get("type", msgType))
    {
        LogDebug << "can not find msgType in slot_recvMsg";
        return;
    }

    if (msgType == msg_type_resp)
    {
        int msgId = 0;
        if (!json.Get("msgId", msgId))
        {
            LogDebug << "can not find msgid in slot_recvMsg";
            return;
        }
        onNetMsgDo(msgId, json);
        return;
    }

    if (msg_type_push == msgType || msg_type_broadcast == msgType)
    {
        std::string cmd;
        if (!json.Get("cmd", cmd))
        {
            LogDebug << "can not find cmd in slot_recvMsg";
            return;
        }

        onNetMsgDo(cmd, json);
        return;
    }
}

void QWSClientMgr::slot_timer()
{
    m_time++;
    if (m_time % 5 == 0)
    {
        // 5秒向网关服务器发送心跳
        neb::CJsonObject json;
        request("cs_msg_heart", json);
    }

    if (!m_bConn)
    {
        m_webSock->open(QUrl("ws://49.232.169.205:5000"));
        LogDebug << "Retry to connect server";
    }
}
