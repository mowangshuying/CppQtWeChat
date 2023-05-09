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
    // 文件类型
    enum class ChatFileTypeEnum
    {
        ChatFile_Owner = 0,     // 接收到自己的文件
        ChatFile_Other,         // 接收到他人的文件
    };

    QChatFileOuterWnd(QWidget* p = nullptr, int64_t sendid = -1, int64_t recvid = -1);
    QLabel* m_headUrl; // 头像信息
    QChatFileInnerWnd* m_innerWnd; // 文件框
    
    QHBoxLayout* m_hLayout;
    QVBoxLayout* m_vLayout;
};