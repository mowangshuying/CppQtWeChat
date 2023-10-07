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
        VTS_none = 0,    // ��ʼ״̬
        VTS_call,        // ��绰
        VTS_waitAccept,  // �ȴ�����
        VTS_accept,      // �����绰
        VTS_phoning,     //ͨ����
        VTS_close,       // �Ҷϵ绰
    };

  public:
    QVoiceTelphoneWnd(QWidget* p = nullptr);

    void regSignalSlot();

    void regNetMsg();

    void timerEvent(QTimerEvent* event);

    void playAudioFormByteArrayVct();

    // �ɼ����������͵����������������ٽ���ת��
    void requestSendVoiceDataToServer(QByteArray& inputByteArray);

    //
    void requestSendCallPhoneToServer();

    void requestSendAcceptPhoneToServer();

    void requestSendClosePhoneToServer();

    //���յ����˷��͵���Ϣ
    void cs_msg_sendvoicemsg(neb::CJsonObject& json);

    // ���� recvId �� sesId;
    void setRecvIdAndSesId(int64_t recvId, int64_t sesId);

    // ����绰
    void callPhone();
    // �Ҷϵ绰
    void closePhone();
    // �����绰
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
    QIODevice* m_outputDevice;  //��ʼ����
    QVector<QByteArray> m_ByteArrayVct;

    QSound* m_bells;

    int64_t m_recvId = -1;
    int64_t m_sesId = -1;
    // ��绰ʱ�䣬�ȴ��Է������绰
    int64_t m_calltimeCount = 0;
    // ��ͨ����ʱ�䣬��ʾ��ǰ�Ѿ�ͨ����ʱ��
    int64_t m_phoningTimeCount = 0;

    // �Ƿ���ͨ����
    VoiceTelphoneState m_state = VoiceTelphoneState::VTS_none;
};
