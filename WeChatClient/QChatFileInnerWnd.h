#pragma once

#include "QPch.h"

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QProgressBar>
#include <QNetworkAccessManager>

class QChatFileInnerWnd : public QWidget
{
    Q_OBJECT
  public:
    QChatFileInnerWnd(QWidget* p = nullptr);
  public slots:
    void slotOpenFileBtnClick();
    void slotCopyFileBtnClick();
    void slotOpenFileDirBtnClick();
    void slotDownloadFileBtnClick();

  public:
    void sendFileShow();
    void recvFileShow();

  public:
    QVBoxLayout* m_vLayout;

    QHBoxLayout* m_hLayout1;
    QVBoxLayout* m_vLayout1;
    QHBoxLayout* m_hLayout2;

    // 进度条，用于显示文件传进度
    QProgressBar* m_progressBar;

    QLabel* m_fileIcon;  // 文件图标
    QLabel* m_fileName;  // 文件名
    QLabel* m_fileSize;  // 文件大小

    QLabel* m_sendState;         // 发送状态
    QPushButton* m_openFile;     // 打开文件
    QPushButton* m_copyFile;     // 拷贝
    QPushButton* m_openFileDir;  // 打开所在目录
    QPushButton* m_downLoad;     // 下载文件

    QString m_fileFullpath = "";   // 文件全路径
    QString m_fileFullDir = "";    // 文件所在目录
    QString m_serveFilePath = "";  //远程的文件地址

    // 下载远程文件
    QNetworkAccessManager* m_pNetManager;
};