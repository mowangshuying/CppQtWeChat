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
    // �ļ�����
    enum class ChatFileTypeEnum
    {
        ChatFile_Owner = 0,     // ���յ��Լ����ļ�
        ChatFile_Other,         // ���յ����˵��ļ�
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
    QLabel* m_headUrl; // ͷ����Ϣ
    QChatFileInnerWnd* m_innerWnd; // �ļ���
    QHBoxLayout* m_hLayout;
    QVBoxLayout* m_vLayout;
};