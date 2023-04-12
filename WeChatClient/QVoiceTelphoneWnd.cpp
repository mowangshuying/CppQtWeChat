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
    m_userNameLabel->setText("美丽的天空");
    m_userNameLabel->setStyleSheet("color:white");
    m_userNameLabel->setAlignment(Qt::AlignCenter);
    m_userNameLabel->setGeometry(width() / 2 - m_userNameLabel->width() / 2,
                                 m_headImgLabel->y() + m_headImgLabel->height() + 20,
                                 m_userNameLabel->width(),
                                 m_userNameLabel->height());

    m_statusLabel = new QLabel(this);
    m_statusLabel->setText("当前你的网络通话不佳");
    m_statusLabel->setStyleSheet("color:white");
    m_statusLabel->setGeometry(width() / 2 - m_statusLabel->width() / 2,
                               m_userNameLabel->y() + m_userNameLabel->height() + 20,
                               m_statusLabel->width(),
                               m_statusLabel->height());
    m_statusLabel->hide();

    // 通话时长
    m_timeLabel = new QLabel(this);
    m_timeLabel->setText("05:35");
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
    m_format.setSampleRate(8000);                       //设置采样率
    m_format.setChannelCount(1);                        //设定声道数目，mono(平声道)的声道数目是1；stero(立体声)的声道数目是2
    m_format.setSampleSize(8);                          //设置采样大小
    m_format.setCodec("audio/pcm");                     //设置编码器，"audio/pcm"在所有的平台都支持
    m_format.setSampleType(QAudioFormat::SignedInt);    //设置采样类型
    m_format.setByteOrder(QAudioFormat::LittleEndian);  //设定高低位的，LittleEndian（低位优先）/LargeEndian(高位优先)
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(m_format))  //格式是否支持;
    {
        m_format = info.nearestFormat(m_format);
    }

    m_output = new QAudioOutput(m_format, this);  //输出音频
    m_input = new QAudioInput(m_format, this);    //输入音频

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

    // 读取所有语音数据并发送到服务器
    QByteArray inputByteArray = m_inputDevice->readAll();
    requestSendVoiceDataToServer(inputByteArray);

    // 播放音频
    playAudioFormByteArrayVct();
}

void QVoiceTelphoneWnd::playAudioFormByteArrayVct()
{
    // 播放m_ByteArrayVct的数据
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
        //向远端发送消息
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
    // 向远端服务器请求关闭电话
    // 远端返回后，隐藏窗口
    LogDebug << "called";
    killTimer(m_timerId);
    m_bells->stop();
    hide();
    m_state = VoiceTelphoneState::VTS_close;
}

void QVoiceTelphoneWnd::acceptPhone()
{
    // cs_accept_phone
    // 向远端发送请求，请求接听电话
    // 远端返回后，关闭振铃
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
