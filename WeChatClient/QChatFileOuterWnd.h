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
        ChatFile_Owner = 0,  // ���յ��Լ����ļ�
        ChatFile_Other,      // ���յ����˵��ļ�
    };

    QChatFileOuterWnd(QWidget* p = nullptr,
                      int64_t sendid = -1,
                      int64_t recvid = -1);

    void setFileName(QString fileName);

    void setFileSize(QString fileSize);

    void setFileDir(QString fileDir);

    void setFilePath(QString filePath);

    void setFileState(QString fileState);

    void setSendFileShow();
    void setRecvFileShow();

    void setServerFileName(QString serverFileName);

    void setFileUploadData(QString fileName,
                           QString fileSize,
                           QString fileDir,
                           QString filePath);
    void setFileDownLoadData(QString fileName,
                             QString fileSize,
                             QString fileState,
                             QString serverFileName);

    void setProgressBarMax(int max);

    void setProgressBarMin(int min);

    void setProgressBarValue(int value);

  public:
    QLabel* m_headUrl;              // ͷ����Ϣ
    QChatFileInnerWnd* m_innerWnd;  // �ļ���
    QHBoxLayout* m_hLayout;
    QVBoxLayout* m_vLayout;
};