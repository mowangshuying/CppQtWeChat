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
        VTS_none = 0, // 初始状态
        VTS_call, // 打电话
        VTS_waitAccept,  // 等待接听
        VTS_accept, // 接听电话
        VTS_phoning, //通话中
        VTS_close, // 挂断电话
    };

public:
    QVoiceTelphoneWnd(QWidget* p = nullptr);

    void timerEvent(QTimerEvent* event);

    void playAudioFormByteArrayVct();

    void requestSendVoiceDataToServer(QByteArray& inputByteArray);

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

    // 是否在通话中
    VoiceTelphoneState m_state = VoiceTelphoneState::VTS_none;
};
