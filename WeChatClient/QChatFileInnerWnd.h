#pragma once

#include "QPch.h"

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QProgressBar>

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

    QProgressBar* m_progressBar;
    ;

    QHBoxLayout* m_hLayout2;

    QLabel* m_fileIcon;
    QLabel* m_fileName;
    QLabel* m_fileSize;

    QLabel* m_sendState;         //����״̬
    QPushButton* m_openFile;     //���ļ�
    QPushButton* m_copyFile;     //����
    QPushButton* m_openFileDir;  //������Ŀ¼
    QPushButton* m_downLoad;

    QString m_fileFullpath = "";
    QString m_fileFullDir = "";
    QString m_serveFilePath = "";  //Զ�̵��ļ���ַ
};