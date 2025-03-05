#include "VoiceTelphoneWnd.h"
#include "WSClientMgr.h"
#include "json/CJsonObject.hpp"
#include "MainWnd.h"
#include <QThread>
#include <functional>

VoiceTelphoneWnd::VoiceTelphoneWnd(QWidget* p) : QWidget(p)
{
    LogFunc;
    setObjectName("QVoiceTelphoneWnd");
    setFixedSize(350, 600);
    setStyleSheet("background-color:rgb(25,25,25)");

    m_headImgLabel = new QLabel(this);
    m_headImgLabel->setFixedSize(90, 90);
    m_headImgLabel->setPixmap(QPixmap("./img/voiceTelphoneHeadImg.png"));
    m_headImgLabel->setGeometry(width() / 2 - m_headImgLabel->width() / 2, 50, m_headImgLabel->width(), m_headImgLabel->height());

    m_userNameLabel = new QLabel(this);
    m_userNameLabel->setText("美丽的天空 ");
    m_userNameLabel->setStyleSheet("color:white");
    m_userNameLabel->setAlignment(Qt::AlignCenter);
    m_userNameLabel->setGeometry(width() / 2 - m_userNameLabel->width() / 2, m_headImgLabel->y() + m_headImgLabel->height() + 20, m_userNameLabel->width(), m_userNameLabel->height());

    m_statusLabel = new QLabel(this);
    m_statusLabel->setText("当前你的网络通话不佳 ");
    m_statusLabel->setStyleSheet("color:white");
    m_statusLabel->setGeometry(width() / 2 - m_statusLabel->width() / 2, m_userNameLabel->y() + m_userNameLabel->height() + 20, m_statusLabel->width(), m_statusLabel->height());
    m_statusLabel->hide();

    // 通话时长
    m_timeLabel = new QLabel(this);
    m_timeLabel->setText("00:00");
    m_timeLabel->setAlignment(Qt::AlignCenter);
    m_timeLabel->setStyleSheet("color:white");
    m_timeLabel->setGeometry(width() / 2 - m_timeLabel->width() / 2, 450, m_timeLabel->width(), m_timeLabel->height());

    // 接听电话
    m_acceptBtn = new QPushButton(this);
    m_acceptBtn->setIconSize(QSize(42, 42));
    m_acceptBtn->setIcon(QIcon("./img/voiceTelphoneAccept.png"));
    m_acceptBtn->setStyleSheet("border:0px");
    m_acceptBtn->setGeometry(width() / 2 - 21, 500, 42, 42);

    // 拒绝接听电话
    m_refuseBtn = new QPushButton(this);
    m_refuseBtn->setIconSize(QSize(42, 42));
    m_refuseBtn->setIcon(QIcon("./img/voiceTelphoneRefuse.png"));
    m_refuseBtn->setStyleSheet("border:0px");
    m_refuseBtn->setGeometry(width() / 2 - 21, 500, 42, 42);

    // 媒体相关
    m_format.setSampleRate(16000);                      // 设置采样率
    m_format.setChannelCount(1);                        // 设定声道数目，mono(平声道)的声道数目是1；stero(立体声)的声道数目是2
    m_format.setSampleSize(16);                         // 设置采样大小
    m_format.setCodec("audio/pcm");                     // 设置编码器，"audio/pcm"在所有的平台都支持
    m_format.setSampleType(QAudioFormat::SignedInt);    // 设置采样类型
    m_format.setByteOrder(QAudioFormat::LittleEndian);  // 设定高低位的，LittleEndian（低位优先）/LargeEndian(高位优先)
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(m_format))  // 格式是否支持;
    {
        m_format = info.nearestFormat(m_format);
    }

    m_output = new QAudioOutput(m_format, this);  // 输出音频
    m_input = new QAudioInput(m_format, this);    // 输入音频

    m_outputDevice = m_output->start();
    m_inputDevice = m_input->start();

    m_bells = new QSoundEffect(this);
    m_bells->setSource(QUrl::fromLocalFile("./music/callPhone.wav"));
    m_bells->setLoopCount(10);

    // 注册信号和槽
    regSignalSlot();
    // 注册网络消息
    regNetMsg();

    // 定时器一直打开
    m_timerId = startTimer(30);
    m_calltimeCount = 0;
    m_phoningTimeCount = 0;
}

void VoiceTelphoneWnd::regSignalSlot()
{
    connect(m_acceptBtn, &QPushButton::clicked, this, &VoiceTelphoneWnd::slotOnAcceptBtnClick);
    connect(m_refuseBtn, &QPushButton::clicked, this, &VoiceTelphoneWnd::slotOnRefuseBtnClick);
}

void VoiceTelphoneWnd::regNetMsg()
{
    WSClientMgr::getMgr()->regMsgCall("cs_msg_call_phone", std::bind(&VoiceTelphoneWnd::cs_msg_call_phone, this, std::placeholders::_1));
    // cs_msg_accept_phone
    WSClientMgr::getMgr()->regMsgCall("cs_msg_accept_phone", std::bind(&VoiceTelphoneWnd::cs_msg_accept_phone, this, std::placeholders::_1));
    // cs_msg_phonemsg
    WSClientMgr::getMgr()->regMsgCall("cs_msg_phonemsg", std::bind(&VoiceTelphoneWnd::cs_msg_phonemsg, this, std::placeholders::_1));
    // cs_msg_close_phone
    WSClientMgr::getMgr()->regMsgCall("cs_msg_close_phone", std::bind(&VoiceTelphoneWnd::cs_msg_close_phone, this, std::placeholders::_1));
}

void VoiceTelphoneWnd::timerEvent(QTimerEvent* event)
{
    if (VoiceTelphoneState::VTS_phoning == m_state)
    {
        // 读取所有语音数据并发送到服务器
        QByteArray inputByteArray = m_inputDevice->readAll();
        requestSendVoiceDataToServer(inputByteArray);

        // 播放音频
        playAudioFormByteArrayVct();

        m_phoningTimeCount += 30;
        // 通话显示几分几秒中
        int nMin = m_phoningTimeCount / (60 * 1000);
        int nSec = (m_phoningTimeCount % (60 * 1000)) / 1000;
        char timeStr[24] = {0};
        sprintf(timeStr, "通话中:%02d:%02d", nMin, nSec);
        m_timeLabel->setText(timeStr);
    }

    // 处于15秒未接电话状态的话，直接挂掉电话
    if (VoiceTelphoneState::VTS_waitAccept == m_state)
    {
        m_calltimeCount += 30;
        if (m_calltimeCount >= 30 * 500)
        {
            slotOnRefuseBtnClick();
            m_calltimeCount = 0;
            m_state = VoiceTelphoneState::VTS_close;
        }
    }
}

void VoiceTelphoneWnd::playAudioFormByteArrayVct()
{
    if (VoiceTelphoneState::VTS_phoning != m_state)
    {
        return;
    }

    // 播放m_ByteArrayVct的数据
    if (m_ByteArrayVct.isEmpty())
    {
        // LogDebug << "m_ByteArrayVct is empty!";
        return;
    }

    QByteArray tmpByteArray = m_ByteArrayVct[0];
    m_outputDevice->write(tmpByteArray);
    m_ByteArrayVct.pop_front();
}

void VoiceTelphoneWnd::requestSendVoiceDataToServer(QByteArray& inputByteArray)
{
    // 只有在打电话时候才发送采集到的消息给服务器
    if (VoiceTelphoneState::VTS_phoning != m_state)
    {
        return;
    }

    neb::CJsonObject json;
    json.Add("sendid", MainWnd::getMainWnd()->m_userid);
    json.Add("recvid", m_recvId);
    json.Add("sesid", m_sesId);
    std::string msgText = inputByteArray.toBase64().toStdString();
    json.Add("msgtext", msgText);

    // LogDebug << "intputByteArray:" << inputByteArray.length() << "msgtext:"
    // << msgText.length() << "json:" << json.ToString().length();

    // 不请求消息
    WSClientMgr::getMgr()->request("cs_msg_phonemsg", json, [](neb::CJsonObject& msg) {
        // 向远端发送消息
        //  LogDebug << "send msg suc!";
    });
}

void VoiceTelphoneWnd::requestSendCallPhoneToServer()
{
    neb::CJsonObject json;
    json.Add("sendid", MainWnd::getMainWnd()->m_userid);
    json.Add("recvid", m_recvId);
    json.Add("sesid", m_sesId);
    // 将拨打电话的请求推送到服务器，如果推送成功，设置当前状态为等待接听电话
    WSClientMgr::getMgr()->request("cs_msg_call_phone", json, [=](neb::CJsonObject& msg) {
        LogDebug << "recv cs_msg_call_phone";
        m_state = VoiceTelphoneState::VTS_waitAccept;
        m_phoningTimeCount = 0;
        m_timeLabel->hide();
    });
}

void VoiceTelphoneWnd::requestSendAcceptPhoneToServer()
{
    neb::CJsonObject json;
    json.Add("sendid", MainWnd::getMainWnd()->m_userid);
    json.Add("recvid", m_recvId);
    json.Add("sesid", m_sesId);

    WSClientMgr::getMgr()->request("cs_msg_accept_phone", json, [=](neb::CJsonObject& msg) {
        LogDebug << "accept phone";
        m_bells->stop();  // 停止振铃
        m_state = VoiceTelphoneState::VTS_phoning;
        m_timeLabel->show();
        // m_timerId = startTimer(30);
        m_acceptBtn->hide();
        m_refuseBtn->show();
    });
}

void VoiceTelphoneWnd::requestSendClosePhoneToServer()
{
    neb::CJsonObject json;
    json.Add("sendid", MainWnd::getMainWnd()->m_userid);
    json.Add("recvid", m_recvId);
    json.Add("sesid", m_sesId);
    WSClientMgr::getMgr()->request("cs_msg_close_phone", json, [=](neb::CJsonObject& msg) {
        closePhone();  //
    });
}

void VoiceTelphoneWnd::setRecvIdAndSesId(int64_t recvId, int64_t sesId)
{
    m_recvId = recvId;
    m_sesId = sesId;
}

void VoiceTelphoneWnd::callPhone()
{
    // cs_call_phone
    m_state = VoiceTelphoneState::VTS_call;
    m_acceptBtn->hide();
    m_bells->play();
    requestSendCallPhoneToServer();
    // m_state = VoiceTelphoneState::VTS_waitAccept;
}

void VoiceTelphoneWnd::closePhone()
{
    // cs_close_phone
    // 向远端服务器请求关闭电话
    // 远端返回后，隐藏窗口
    LogDebug << "called";
    // killTimer(m_timerId);
    m_bells->stop();
    hide();
    m_state = VoiceTelphoneState::VTS_close;
    // 清空采集到的消息，不在向远端服务器推送
    m_ByteArrayVct.clear();
}

void VoiceTelphoneWnd::acceptPhone()
{
    // cs_accept_phone
    // 向远端发送请求，请求接听电话
    // 远端返回后，关闭振铃
    requestSendAcceptPhoneToServer();
}

void VoiceTelphoneWnd::cs_msg_call_phone(neb::CJsonObject& msg)
{
    LogDebug << msg.ToString().c_str();

    int sesid = -1;
    if (!msg["data"].Get("sesid", sesid))
    {
        return;
    }

    int64_t sendid = -1;
    if (!msg["data"].Get("sendid", sendid))
    {
        return;
    }

    int64_t recvid = -1;
    if (!msg["data"].Get("recvid", recvid))
    {
        return;
    }

    setRecvIdAndSesId(sendid, sesid);
    show();
    m_bells->play();
    m_refuseBtn->hide();
    m_acceptBtn->show();
    m_state = VoiceTelphoneState::VTS_waitAccept;
    m_phoningTimeCount = 0;
    m_calltimeCount = 0;
    m_timeLabel->hide();
}

void VoiceTelphoneWnd::cs_msg_accept_phone(neb::CJsonObject& msg)
{
    LogDebug << "called";
    // 关闭铃声
    int sesid = -1;
    if (!msg["data"].Get("sesid", sesid))
    {
        return;
    }

    int64_t sendid = -1;
    if (!msg["data"].Get("sendid", sendid))
    {
        return;
    }

    int64_t recvid = -1;
    if (!msg["data"].Get("recvid", recvid))
    {
        return;
    }

    setRecvIdAndSesId(sendid, sesid);
    m_bells->stop();
    m_state = VoiceTelphoneState::VTS_phoning;
    // m_timerId = startTimer(30);
    m_refuseBtn->show();
    m_acceptBtn->hide();
    m_timeLabel->show();
}

void VoiceTelphoneWnd::cs_msg_phonemsg(neb::CJsonObject& msg)
{
    // 接收到远端的消息后，将消息存入m_ByteArrayVct
    int sesid = -1;
    if (!msg["data"].Get("sesid", sesid))
    {
        return;
    }

    std::string msgtext = "";
    if (!msg["data"].Get("msgtext", msgtext))
    {
        return;
    }

    int64_t sendid = -1;
    if (!msg["data"].Get("sendid", sendid))
    {
        return;
    }

    int64_t recvid = -1;
    if (!msg["data"].Get("sendid", recvid))
    {
        return;
    }

    // 收到语音通话消息，存入m_phoneWnd
    QString qMsgText = msgtext.c_str();
    QByteArray byteArray = QByteArray::fromBase64(qMsgText.toUtf8());
    m_ByteArrayVct.push_back(byteArray);
}

void VoiceTelphoneWnd::cs_msg_close_phone(neb::CJsonObject& msg)
{
    // 挂断电话
    closePhone();
}

void VoiceTelphoneWnd::slotOnAcceptBtnClick()
{
    LogDebug << "called";
    acceptPhone();
}

void VoiceTelphoneWnd::slotOnRefuseBtnClick()
{
    // 告诉服务器，我点击了挂电话的按钮
    // 服务器会将挂电话的信息推送到对方
    requestSendClosePhoneToServer();
}