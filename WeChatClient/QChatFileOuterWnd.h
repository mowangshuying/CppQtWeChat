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

    void setFileName(QString fileName)
    {
        if (m_innerWnd == nullptr)
        {
            return;
        }

        m_innerWnd->m_fileName->setText(fileName);
    }

    void setFileSize(QString fileSize)
    {
        if (m_innerWnd == nullptr)
        {
            return;
        }
        m_innerWnd->m_fileSize->setText(fileSize);
    }

    void setFileDir(QString fileDir)
    {
        if (m_innerWnd == nullptr)
        {
            return;
        }

        m_innerWnd->m_fileFullDir = fileDir;
    }

    void setFilePath(QString filePath)
    {
        if (m_innerWnd == nullptr)
        {
            return;
        }
        m_innerWnd->m_fileFullpath = filePath;
    }

    void setSendFileShow()
    {
        if (m_innerWnd == nullptr)
        {
            return;
        }

        m_innerWnd->sendFileShow();
    }

    void setFileUploadData(QString fileName, QString fileSize, QString fileDir, QString filePath)
    {
        setFileName(fileName);
        setFileSize(fileSize);
        setFileDir(fileDir);
        setFilePath(filePath);
        setSendFileShow();
    }

public:
    QLabel* m_headUrl; // 头像信息
    QChatFileInnerWnd* m_innerWnd; // 文件框
    QHBoxLayout* m_hLayout;
    QVBoxLayout* m_vLayout;
};