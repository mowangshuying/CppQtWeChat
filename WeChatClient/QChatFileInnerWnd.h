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

    // ��������������ʾ�ļ�������
    QProgressBar* m_progressBar;

    QLabel* m_fileIcon;  // �ļ�ͼ��
    QLabel* m_fileName;  // �ļ���
    QLabel* m_fileSize;  // �ļ���С

    QLabel* m_sendState;         // ����״̬
    QPushButton* m_openFile;     // ���ļ�
    QPushButton* m_copyFile;     // ����
    QPushButton* m_openFileDir;  // ������Ŀ¼
    QPushButton* m_downLoad;     // �����ļ�

    QString m_fileFullpath = "";   // �ļ�ȫ·��
    QString m_fileFullDir = "";    // �ļ�����Ŀ¼
    QString m_serveFilePath = "";  //Զ�̵��ļ���ַ

    // ����Զ���ļ�
    QNetworkAccessManager* m_pNetManager;
};