#pragma once

#include "QPch.h"

#include <QWidget>
#include <QLabel>
#include <QMovie>

class QChatMsgWnd : public QWidget
{
    Q_OBJECT
public:
    //��Ϣ����
    enum ChatMsgTypeEnum
    {
        ChatMsg_System = 10,
        ChatMsg_Time,           // ʱ����Ϣ - (һ�����Լ�������)
        ChatMsg_OwnerMsgText,   // �Լ�����Ϣ
        ChatMsg_OtherMsgText,   // �����˷��͵���Ϣ
    };

public:
    //���캯��
    QChatMsgWnd(QWidget* p = nullptr, int64_t sendid = 0, int64_t recvid = 0);

    //����str����������Ĵ�С
    QSize fontRect(QString str);

    QSize getRealStringSize(QString str);

    void setText(QString text, QString time, QSize allSize, QChatMsgWnd::ChatMsgTypeEnum chatMsgType);

protected:
    //����
    void paintEvent(QPaintEvent* event);

public:
    int64_t m_sendid;
    int64_t m_recvid;

    QString m_msg;
    QString m_time;
    QString m_curTime;
    QSize m_allSize;

    ChatMsgTypeEnum m_chatMsgType = ChatMsg_System;

    int m_outerFrameWidth;
    int m_textWidth;
    int m_spaceWidth;
    int m_lineHeight;

    QRect m_iconLeftRect;
    QRect m_iconRightRect;
    
    QRect m_triangleLeftRect;
    QRect m_triangleRightRect;

    QRect m_outerFrameLeftRect;
    QRect m_outerFrameRightRect;

    QRect m_leftUserNameRect;
    QRect m_rightUserNameRect;

    // �����ı���
    QRect m_textLeftRect;
    QRect m_textRigtRect;

    QPixmap m_leftPixmap;
    QPixmap m_rightPixmap;

    QLabel* m_loadingLable = nullptr;
    QMovie* m_loadingMovie = nullptr;

    //�Ƿ��ڷ�����
    bool m_bSending = false;
};