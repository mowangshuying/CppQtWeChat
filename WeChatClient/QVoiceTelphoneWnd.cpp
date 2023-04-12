#include "QVoiceTelphoneWnd.h"
#include "QWSClientMgr.h"
#include "json/CJsonObject.hpp"
#include "QMainWnd.h"
#include <QThread>

QVoiceTelphoneWnd::QVoiceTelphoneWnd(QWidget* p) : QWidget(p)
{
    setFixedSize(350, 600);
    setStyleSheet("background-color:rgb(25,25,25)");

    m_headImgLabel = new QLabel(this);
    m_headImgLabel->setFixedSize(90, 90);
    m_headImgLabel->setPixmap(QPixmap("./img/voiceTelphoneHeadImg.png"));
    m_headImgLabel->setGeometry(width() / 2 - m_headImgLabel->width() / 2, 50, m_headImgLabel->width(), m_headImgLabel->height());

    m_userNameLabel = new QLabel(this);
    m_userNameLabel->setText("���������");
    m_userNameLabel->setStyleSheet("color:white");
    m_userNameLabel->setAlignment(Qt::AlignCenter);
    m_userNameLabel->setGeometry(width() / 2 - m_userNameLabel->width() / 2,
                                 m_headImgLabel->y() + m_headImgLabel->height() + 20,
                                 m_userNameLabel->width(),
                                 m_userNameLabel->height());

    m_statusLabel = new QLabel(this);
    m_statusLabel->setText("��ǰ�������ͨ������");
    m_statusLabel->setStyleSheet("color:white");
    m_statusLabel->setGeometry(width() / 2 - m_statusLabel->width() / 2,
                               m_userNameLabel->y() + m_userNameLabel->height() + 20,
                               m_statusLabel->width(),
                               m_statusLabel->height());
    m_statusLabel->hide();

    // ͨ��ʱ��
    m_timeLabel = new QLabel(this);
    m_timeLabel->setText("05:35");
    m_timeLabel->setAlignment(Qt::AlignCenter);
    m_timeLabel->setStyleSheet("color:white");
    m_timeLabel->setGeometry(width() / 2 - m_timeLabel->width() / 2, 450, m_timeLabel->width(), m_timeLabel->height());

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
    m_format.setSampleRate(8000);                       //���ò�����
    m_format.setChannelCount(1);                        //�趨������Ŀ��mono(ƽ����)��������Ŀ��1��stero(������)��������Ŀ��2
    m_format.setSampleSize(8);                          //���ò�����С
    m_format.setCodec("audio/pcm");                     //���ñ�������"audio/pcm"�����е�ƽ̨��֧��
    m_format.setSampleType(QAudioFormat::SignedInt);    //���ò�������
    m_format.setByteOrder(QAudioFormat::LittleEndian);  //�趨�ߵ�λ�ģ�LittleEndian����λ���ȣ�/LargeEndian(��λ����)
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(m_format))  //��ʽ�Ƿ�֧��;
    {
        m_format = info.nearestFormat(m_format);
    }

    m_output = new QAudioOutput(m_format, this);  //�����Ƶ
    m_input = new QAudioInput(m_format, this);    //������Ƶ

    m_outputDevice = m_output->start();
    m_inputDevice = m_input->start();

    connect(m_acceptBtn, &QPushButton::clicked, this, &QVoiceTelphoneWnd::slotOnAcceptBtnClick);
    connect(m_refuseBtn, &QPushButton::clicked, this, &QVoiceTelphoneWnd::slotOnRefuseBtnClick);
    m_bells = new QSound("./music/callPhone.wav");
    m_bells->setLoops(10);
}

void QVoiceTelphoneWnd::timerEvent(QTimerEvent* event)
{
    LogDebug << "called";

    // ��ȡ�����������ݲ����͵�������
    QByteArray inputByteArray = m_inputDevice->readAll();
    requestSendVoiceDataToServer(inputByteArray);

    // ������Ƶ
    playAudioFormByteArrayVct();
}

void QVoiceTelphoneWnd::playAudioFormByteArrayVct()
{
    // ����m_ByteArrayVct������
    if (m_ByteArrayVct.isEmpty())
    {
        LogDebug << "m_ByteArrayVct is empty!";
        return;
    }

    QByteArray tmpByteArray = m_ByteArrayVct[0];
    m_outputDevice->write(tmpByteArray);
    m_ByteArrayVct.pop_front();
}

void QVoiceTelphoneWnd::requestSendVoiceDataToServer(QByteArray& inputByteArray)
{
    neb::CJsonObject json;
    json.Add("sendid", QMainWnd::getInstance()->m_userid);
    json.Add("recvid", m_recvId);
    json.Add("sesid", m_sesId);
    std::string msgText = inputByteArray.toBase64().toStdString();
    json.Add("msgtext", msgText);
    json.Add("msgtype", 2);

    LogDebug << "intputByteArray:" << inputByteArray.length() << "msgtext:" << msgText.length() << "json:" << json.ToString().length();
    QWSClientMgr::getInstance()->request("cs_msg_sendmsg", json, [](neb::CJsonObject& msg) {
        //��Զ�˷�����Ϣ
        LogDebug << "send msg suc!";
    });
}

void QVoiceTelphoneWnd::cs_msg_sendvoicemsg(neb::CJsonObject& json)
{
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
    m_state = VoiceTelphoneState::VTS_waitAccept;
}

void QVoiceTelphoneWnd::closePhone()
{
    // cs_close_phone
    // ��Զ�˷���������رյ绰
    // Զ�˷��غ����ش���
    LogDebug << "called";
    killTimer(m_timerId);
    m_bells->stop();
    hide();
    m_state = VoiceTelphoneState::VTS_close;
}

void QVoiceTelphoneWnd::acceptPhone()
{
    // cs_accept_phone
    // ��Զ�˷���������������绰
    // Զ�˷��غ󣬹ر�����
}

void QVoiceTelphoneWnd::slotOnAcceptBtnClick()
{
    LogDebug << "called";
    m_timerId = startTimer(10);
    // m_bells->play();
}

void QVoiceTelphoneWnd::slotOnRefuseBtnClick()
{
    closePhone();
}
