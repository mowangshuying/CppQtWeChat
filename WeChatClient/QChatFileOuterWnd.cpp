#include "QDataManager.h"
#include "QChatFileOuterWnd.h"

QChatFileOuterWnd::QChatFileOuterWnd(QWidget* p /*= nullptr*/, int64_t sendid /* = -1*/, int64_t recvid /* = -1*/) : QWidget(p)
{
    setObjectName("QChatFileOuterWnd");
    m_hLayout = new QHBoxLayout();
    m_vLayout = new QVBoxLayout();
    setLayout(m_hLayout);
    setMinimumHeight(120);

    m_headUrl = new QLabel();
    m_headUrl->setFixedSize(30, 30);
    m_vLayout->addWidget(m_headUrl);
    m_vLayout->addStretch();

    m_innerWnd = new QChatFileInnerWnd();
    if (sendid != QDataManager::getMgr()->m_userid)
    {
        m_hLayout->setSpacing(0);
        m_hLayout->addSpacing(10);
        //设置玩家的头像
        m_hLayout->addLayout(m_vLayout);
        m_hLayout->addSpacing(5);
        m_hLayout->addWidget(m_innerWnd);
        m_hLayout->addStretch();
        QPixmap pixmap = QDataManager::getMgr()->m_UserId2HeadImgMap[sendid];
        pixmap = pixmap.scaled(30, 30);
        m_headUrl->setPixmap(pixmap);
    }
    else
    {
        m_hLayout->addStretch();
        m_hLayout->addWidget(m_innerWnd);
        m_hLayout->addSpacing(5);
        m_hLayout->addLayout(m_vLayout);
        m_hLayout->addSpacing(10);

        QPixmap pixmap = QDataManager::getMgr()->m_UserId2HeadImgMap[sendid];
        pixmap = pixmap.scaled(30, 30);
        m_headUrl->setPixmap(pixmap);
    }
}

void QChatFileOuterWnd::setFileName(QString fileName)
{
    if (m_innerWnd == nullptr)
    {
        return;
    }

    m_innerWnd->m_fileName->setText(fileName);
}

void QChatFileOuterWnd::setFileSize(QString fileSize)
{
    if (m_innerWnd == nullptr)
    {
        return;
    }
    m_innerWnd->m_fileSize->setText(fileSize);
}

void QChatFileOuterWnd::setFileDir(QString fileDir)
{
    if (m_innerWnd == nullptr)
    {
        return;
    }

    m_innerWnd->m_fileFullDir = fileDir;
}

void QChatFileOuterWnd::setFilePath(QString filePath)
{
    if (m_innerWnd == nullptr)
    {
        return;
    }
    m_innerWnd->m_fileFullpath = filePath;
}

void QChatFileOuterWnd::setFileState(QString fileState)
{
    if (!m_innerWnd)
    {
        return;
    }

    m_innerWnd->m_sendState->setText(fileState);
}

void QChatFileOuterWnd::setSendFileShow()
{
    if (m_innerWnd == nullptr)
    {
        return;
    }

    m_innerWnd->sendFileShow();
}

void QChatFileOuterWnd::setRecvFileShow()
{
    if (!m_innerWnd)
    {
        return;
    }

    m_innerWnd->recvFileShow();
}

void QChatFileOuterWnd::setServerFileName(QString serverFileName)
{
    if (!m_innerWnd)
    {
        return;
    }

    m_innerWnd->m_serveFilePath = serverFileName;
}

void QChatFileOuterWnd::setFileUploadData(QString fileName, QString fileSize, QString fileDir, QString filePath)
{
    setFileName(fileName);
    setFileSize(fileSize);
    setFileDir(fileDir);
    setFilePath(filePath);
    setSendFileShow();
}

void QChatFileOuterWnd::setFileDownLoadData(QString fileName, QString fileSize, QString fileState, QString serverFileName)
{
    setFileName(fileName);
    setFileSize(fileSize);
    setFileState(fileState);
    setServerFileName(serverFileName);
    setRecvFileShow();
}

void QChatFileOuterWnd::setProgressBarMax(int max)
{
    if (!m_innerWnd)
        return;
    m_innerWnd->m_progressBar->setMaximum(max);
}

void QChatFileOuterWnd::setProgressBarMin(int min)
{
    if (!m_innerWnd)
        return;
    m_innerWnd->m_progressBar->setMinimum(min);
}

void QChatFileOuterWnd::setProgressBarValue(int value)
{
    if (!m_innerWnd)
        return;
    m_innerWnd->m_progressBar->setValue(value);
}
