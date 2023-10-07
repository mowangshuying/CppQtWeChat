#pragma once

#include "QPch.h"
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QAudioInput>
#include <QAudioOutput>
#include <QIODevice>
#include "json/CJsonObject.hpp"
#include <QSound>
#include <QAudioFormat>

class QVoiceTelphoneWnd : public QWidget
{
    Q_OBJECT
  public:
    enum class VoiceTelphoneState
    {
        VTS_none = 0,    // 初始状态
        VTS_call,        // 打电话
        VTS_waitAccept,  // 等待接听
        VTS_accept,      // 接听电话
        VTS_phoning,     //通话中
        VTS_close,       // 挂断电话
    };

  public:
    QVoiceTelphoneWnd(QWidget* p = nullptr);

    void regSignalSlot();

    void regNetMsg();

    void timerEvent(QTimerEvent* event);

    void playAudioFormByteArrayVct();

    // 采集语音并发送到服务器，服务器再进行转发
    void requestSendVoiceDataToServer(QByteArray& inputByteArray);

    //
    void requestSendCallPhoneToServer();

    void requestSendAcceptPhoneToServer();

    void requestSendClosePhoneToServer();

    //接收到别人发送的消息
    void cs_msg_sendvoicemsg(neb::CJsonObject& json);

    // 设置 recvId 和 sesId;
    void setRecvIdAndSesId(int64_t recvId, int64_t sesId);

    // 拨打电话
    void callPhone();
    // 挂断电话
    void closePhone();
    // 接听电话
    void acceptPhone();
    // cs_msg_phonemsg

    void cs_msg_call_phone(neb::CJsonObject& msg);
    void cs_msg_accept_phone(neb::CJsonObject& msg);
    void cs_msg_phonemsg(neb::CJsonObject& msg);
    void cs_msg_close_phone(neb::CJsonObject& msg);
  public slots:
    void slotOnAcceptBtnClick();
    void slotOnRefuseBtnClick();

  public:
    QAudioFormat m_format;
    int m_timerId;
    QVBoxLayout* m_vLayout;
    QLabel* m_headImgLabel;
    QLabel* m_userNameLabel;
    QLabel* m_statusLabel;
    QLabel* m_timeLabel;
    QHBoxLayout* m_hLayout;
    QPushButton* m_acceptBtn;
    QPushButton* m_refuseBtn;

    QAudioInput* m_input;
    QIODevice* m_inputDevice;

    QAudioOutput* m_output;
    QIODevice* m_outputDevice;  //开始播放
    QVector<QByteArray> m_ByteArrayVct;

    QSound* m_bells;

    int64_t m_recvId = -1;
    int64_t m_sesId = -1;
    // 打电话时间，等待对方接听电话
    int64_t m_calltimeCount = 0;
    // 在通话的时间，显示当前已经通话的时间
    int64_t m_phoningTimeCount = 0;

    // 是否在通话中
    VoiceTelphoneState m_state = VoiceTelphoneState::VTS_none;
};
