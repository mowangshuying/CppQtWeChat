#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "QChatFileInnerWnd.h"

class QChatFileOuterWnd : public QWidget
{
    Q_OBJECT
public:
    //�ļ�����
    enum ChatFileTypeEnum
    {
        ChatFile_Owner,  //���յ��Լ����ļ�
        ChatFile_Other,  //���յ����˵��ļ�
    };

    QChatFileOuterWnd(QWidget* p = nullptr, int64_t sendid = -1, int64_t recvid = -1);
    QLabel* m_headUrl;
    QChatFileInnerWnd* m_innerWnd;
    QHBoxLayout* m_hLayout;
    QVBoxLayout* m_vLayout;
};