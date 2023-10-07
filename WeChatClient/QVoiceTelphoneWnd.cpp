#include "QVoiceTelphoneWnd.h"
#include "QWSClientMgr.h"
#include "json/CJsonObject.hpp"
#include "QMainWnd.h"
#include <QThread>
#include <functional>

QVoiceTelphoneWnd::QVoiceTelphoneWnd(QWidget* p) : QWidget(p)
{
    LogFunc;
    setObjectName("QVoiceTelphoneWnd");
    setFixedSize(350, 600);
    setStyleSheet("background-color:rgb(25,25,25)");

    m_headImgLabel = new QLabel(this);
    m_headImgLabel->setFixedSize(90, 90);
    m_headImgLabel->setPixmap(QPixmap("./img/voiceTelphoneHeadImg.png"));
    m_headImgLabel->setGeometry(width() / 2 - m_headImgLabel->width() / 2,
                                50,
                                m_headImgLabel->width(),
                                m_headImgLabel->height());

    m_userNameLabel = new QLabel(this);
    m_userNameLabel->setText("���������");
    m_userNameLabel->setStyleSheet("color:white");
    m_userNameLabel->setAlignment(Qt::AlignCenter);
    m_userNameLabel->setGeometry(width() / 2 - m_userNameLabel->width() / 2,
                                 m_headImgLabel->y() +
                                     m_headImgLabel->height() + 20,
                                 m_userNameLabel->width(),
                                 m_userNameLabel->height());

    m_statusLabel = new QLabel(this);
    m_statusLabel->setText("��ǰ�������ͨ������");
    m_statusLabel->setStyleSheet("color:white");
    m_statusLabel->setGeometry(width() / 2 - m_statusLabel->width() / 2,
                               m_userNameLabel->y() +
                                   m_userNameLabel->height() + 20,
                               m_statusLabel->width(),
                               m_statusLabel->height());
    m_statusLabel->hide();

    // ͨ��ʱ��
    m_timeLabel = new QLabel(this);
    m_timeLabel->setText("00:00");
    m_timeLabel->setAlignment(Qt::AlignCenter);
    m_timeLabel->setStyleSheet("color:white");
    m_timeLabel->setGeometry(width() / 2 - m_timeLabel->width() / 2,
                             450,
                             m_timeLabel->width(),
                             m_timeLabel->height());

    // �����绰
    m_acceptBtn = new QPushButton(this);
    m_acceptBtn->setIconSize(QSize(42, 42));
    m_acceptBtn->setIcon(QIcon("./img/voiceTelphoneAccept.png"));
    m_acceptBtn->setStyleSheet("border:0px");
    m_acceptBtn->setGeometry(width() / 2 - 21, 500, 42, 42);

    // �ܾ������绰
    m_refuseBtn = new QPushButton(this);
    m_refuseBtn->setIconSize(QSize(42, 42));
    m_refuseBtn->setIcon(QIcon("./img/voiceTelphoneRefuse.png"));
    m_refuseBtn->setStyleSheet("border:0px");
    m_refuseBtn->setGeometry(width() / 2 - 21, 500, 42, 42);

    // ý�����
    m_format.setSampleRate(16000);  //���ò�����
    m_format.setChannelCount(
        1);  //�趨������Ŀ��mono(ƽ����)��������Ŀ��1��stero(������)��������Ŀ��2
    m_format.setSampleSize(16);      //���ò�����С
    m_format.setCodec("audio/pcm");  //���ñ�������"audio/pcm"�����е�ƽ̨��֧��
    m_format.setSampleType(QAudioFormat::SignedInt);  //���ò�������
    m_format.setByteOrder(
        QAudioFormat::
            LittleEndian);  //�趨�ߵ�λ�ģ�LittleEndian����λ���ȣ�/LargeEndian(��λ����)
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(m_format))  //��ʽ�Ƿ�֧��;
    {
        m_format = info.nearestFormat(m_format);
    }

    m_output = new QAudioOutput(m_format, this);  //�����Ƶ
    m_input = new QAudioInput(m_format, this);    //������Ƶ

    m_outputDevice = m_output->start();
    m_inputDevice = m_input->start();

    m_bells = new QSound("./music/callPhone.wav");
    m_bells->setLoops(10);

    // ע���źźͲ�
    regSignalSlot();
    // ע��������Ϣ
    regNetMsg();

    // ��ʱ��һֱ��
    m_timerId = startTimer(30);
    m_calltimeCount = 0;
    m_phoningTimeCount = 0;
}

void QVoiceTelphoneWnd::regSignalSlot()
{
    connect(m_acceptBtn,
            &QPushButton::clicked,
            this,
            &QVoiceTelphoneWnd::slotOnAcceptBtnClick);
    connect(m_refuseBtn,
            &QPushButton::clicked,
            this,
            &QVoiceTelphoneWnd::slotOnRefuseBtnClick);
}

void QVoiceTelphoneWnd::regNetMsg()
{
    QWSClientMgr::getMgr()->regMsgCall(
        "cs_msg_call_phone",
        std::bind(&QVoiceTelphoneWnd::cs_msg_call_phone,
                  this,
                  std::placeholders::_1));
    // cs_msg_accept_phone
    QWSClientMgr::getMgr()->regMsgCall(
        "cs_msg_accept_phone",
        std::bind(&QVoiceTelphoneWnd::cs_msg_accept_phone,
                  this,
                  std::placeholders::_1));
    // cs_msg_phonemsg
    QWSClientMgr::getMgr()->regMsgCall(
        "cs_msg_phonemsg",
        std::bind(&QVoiceTelphoneWnd::cs_msg_phonemsg,
                  this,
                  std::placeholders::_1));
    // cs_msg_close_phone
    QWSClientMgr::getMgr()->regMsgCall(
        "cs_msg_close_phone",
        std::bind(&QVoiceTelphoneWnd::cs_msg_close_phone,
                  this,
                  std::placeholders::_1));
}

void QVoiceTelphoneWnd::timerEvent(QTimerEvent* event)
{
    if (VoiceTelphoneState::VTS_phoning == m_state)
    {
        // ��ȡ�����������ݲ����͵�������
        QByteArray inputByteArray = m_inputDevice->readAll();
        requestSendVoiceDataToServer(inputByteArray);

        // ������Ƶ
        playAudioFormByteArrayVct();

        m_phoningTimeCount += 30;
        // ͨ����ʾ���ּ�����
        int nMin = m_phoningTimeCount / (60 * 1000);
        int nSec = (m_phoningTimeCount % (60 * 1000)) / 1000;
        char timeStr[24] = {0};
        sprintf(timeStr, "ͨ����:%02d:%02d", nMin, nSec);
        m_timeLabel->setText(timeStr);
    }

    // ����15��δ�ӵ绰״̬�Ļ���ֱ�ӹҵ��绰
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

void QVoiceTelphoneWnd::playAudioFormByteArrayVct()
{
    if (VoiceTelphoneState::VTS_phoning != m_state)
    {
        return;
    }

    // ����m_ByteArrayVct������
    if (m_ByteArrayVct.isEmpty())
    {
        // LogDebug << "m_ByteArrayVct is empty!";
        return;
    }

    QByteArray tmpByteArray = m_ByteArrayVct[0];
    m_outputDevice->write(tmpByteArray);
    m_ByteArrayVct.pop_front();
}

void QVoiceTelphoneWnd::requestSendVoiceDataToServer(QByteArray& inputByteArray)
{
    // ֻ���ڴ�绰ʱ��ŷ��Ͳɼ�������Ϣ��������
    if (VoiceTelphoneState::VTS_phoning != m_state)
    {
        return;
    }

    neb::CJsonObject json;
    json.Add("sendid", QMainWnd::getMainWnd()->m_userid);
    json.Add("recvid", m_recvId);
    json.Add("sesid", m_sesId);
    std::string msgText = inputByteArray.toBase64().toStdString();
    json.Add("msgtext", msgText);

    // LogDebug << "intputByteArray:" << inputByteArray.length() << "msgtext:"
    // << msgText.length() << "json:" << json.ToString().length();

    // ��������Ϣ
    QWSClientMgr::getMgr()->request("cs_msg_phonemsg",
                                    json,
                                    [](neb::CJsonObject& msg) {
                                        //��Զ�˷�����Ϣ
                                        // LogDebug << "send msg suc!";
                                    });
}

void QVoiceTelphoneWnd::requestSendCallPhoneToServer()
{
    neb::CJsonObject json;
    json.Add("sendid", QMainWnd::getMainWnd()->m_userid);
    json.Add("recvid", m_recvId);
    json.Add("sesid", m_sesId);
    // ������绰���������͵���������������ͳɹ������õ�ǰ״̬Ϊ�ȴ������绰
    QWSClientMgr::getMgr()->request("cs_msg_call_phone",
                                    json,
                                    [=](neb::CJsonObject& msg) {
                                        LogDebug << "recv cs_msg_call_phone";
                                        m_state =
                                            VoiceTelphoneState::VTS_waitAccept;
                                        m_phoningTimeCount = 0;
                                        m_timeLabel->hide();
                                    });
}

void QVoiceTelphoneWnd::requestSendAcceptPhoneToServer()
{
    neb::CJsonObject json;
    json.Add("sendid", QMainWnd::getMainWnd()->m_userid);
    json.Add("recvid", m_recvId);
    json.Add("sesid", m_sesId);

    QWSClientMgr::getMgr()->request("cs_msg_accept_phone",
                                    json,
                                    [=](neb::CJsonObject& msg) {
                                        LogDebug << "accept phone";
                                        m_bells->stop();  // ֹͣ����
                                        m_state =
                                            VoiceTelphoneState::VTS_phoning;
                                        m_timeLabel->show();
                                        // m_timerId = startTimer(30);
                                        m_acceptBtn->hide();
                                        m_refuseBtn->show();
                                    });
}

void QVoiceTelphoneWnd::requestSendClosePhoneToServer()
{
    neb::CJsonObject json;
    json.Add("sendid", QMainWnd::getMainWnd()->m_userid);
    json.Add("recvid", m_recvId);
    json.Add("sesid", m_sesId);
    QWSClientMgr::getMgr()->request("cs_msg_close_phone",
                                    json,
                                    [=](neb::CJsonObject& msg) {
                                        closePhone();  //
                                    });
}

void QVoiceTelphoneWnd::setRecvIdAndSesId(int64_t recvId, int64_t sesId)
{
    m_recvId = recvId;
    m_sesId = sesId;
}

void QVoiceTelphoneWnd::callPhone()
{
    // cs_call_phone
    m_state = VoiceTelphoneState::VTS_call;
    m_acceptBtn->hide();
    m_bells->play();
    requestSendCallPhoneToServer();
    // m_state = VoiceTelphoneState::VTS_waitAccept;
}

void QVoiceTelphoneWnd::closePhone()
{
    // cs_close_phone
    // ��Զ�˷���������رյ绰
    // Զ�˷��غ����ش���
    LogDebug << "called";
    // killTimer(m_timerId);
    m_bells->stop();
    hide();
    m_state = VoiceTelphoneState::VTS_close;
    // ��ղɼ�������Ϣ��������Զ�˷���������
    m_ByteArrayVct.clear();
}

void QVoiceTelphoneWnd::acceptPhone()
{
    // cs_accept_phone
    // ��Զ�˷���������������绰
    // Զ�˷��غ󣬹ر�����
    requestSendAcceptPhoneToServer();
}

void QVoiceTelphoneWnd::cs_msg_call_phone(neb::CJsonObject& msg)
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

void QVoiceTelphoneWnd::cs_msg_accept_phone(neb::CJsonObject& msg)
{
    LogDebug << "called";
    // �ر�����
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

void QVoiceTelphoneWnd::cs_msg_phonemsg(neb::CJsonObject& msg)
{
    // ���յ�Զ�˵���Ϣ�󣬽���Ϣ����m_ByteArrayVct
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

    // �յ�����ͨ����Ϣ������m_phoneWnd
    QString qMsgText = msgtext.c_str();
    QByteArray byteArray = QByteArray::fromBase64(qMsgText.toUtf8());
    m_ByteArrayVct.push_back(byteArray);
}

void QVoiceTelphoneWnd::cs_msg_close_phone(neb::CJsonObject& msg)
{
    // �Ҷϵ绰
    closePhone();
}

void QVoiceTelphoneWnd::slotOnAcceptBtnClick()
{
    LogDebug << "called";
    acceptPhone();
}

void QVoiceTelphoneWnd::slotOnRefuseBtnClick()
{
    // ���߷��������ҵ���˹ҵ绰�İ�ť
    // �������Ὣ�ҵ绰����Ϣ���͵��Է�
    requestSendClosePhoneToServer();
}