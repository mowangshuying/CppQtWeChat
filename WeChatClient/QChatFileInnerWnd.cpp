#include <QDir>
#include <QUrl>
#include <QFile>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QCoreApplication>
#include <QNetworkAccessManager>

#include "QDataManager.h"
#include "QSelfSplit.h"
#include "QChatFileInnerWnd.h"
#include "QStyleSheetMgr.h"

QChatFileInnerWnd::QChatFileInnerWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    LogFunc;
    setObjectName("QChatFileInnerWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + objectName() + ".qss";
    object.m_widget = this;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    m_vLayout = new QVBoxLayout();
    setLayout(m_vLayout);

    //���ڴ���ļ���icon
    m_fileIcon = new QLabel();
    m_fileIcon->setFixedSize(40, 40);
    m_fileIcon->setPixmap(QPixmap("./img/fileicon.png"));

    m_fileName = new QLabel("File.rar");
    m_fileSize = new QLabel("24.13k");

    m_vLayout1 = new QVBoxLayout();
    m_vLayout1->addSpacing(5);
    m_vLayout1->addWidget(m_fileName);
    m_vLayout1->addSpacing(5);
    m_vLayout1->addWidget(m_fileSize);
    m_vLayout1->addStretch();

    m_hLayout1 = new QHBoxLayout();
    m_hLayout1->addWidget(m_fileIcon);
    m_hLayout1->addLayout(m_vLayout1);
    m_hLayout1->addStretch();

    m_vLayout->addLayout(m_hLayout1);

    m_progressBar = new QProgressBar();
    m_progressBar->setFixedHeight(10);
    m_vLayout->addWidget(m_progressBar);

    m_hLayout2 = new QHBoxLayout();
    m_sendState = new QLabel("������");
    m_openFile = new QPushButton("���ļ�");
    m_copyFile = new QPushButton("�����ļ�");
    m_openFileDir = new QPushButton("��Ŀ¼");
    m_downLoad = new QPushButton("�����ļ�");

    m_hLayout2->addWidget(m_sendState);
    m_hLayout2->addStretch();
    m_hLayout2->addWidget(m_openFile);
    m_hLayout2->addWidget(m_openFileDir);
    m_hLayout2->addWidget(m_copyFile);
    m_hLayout2->addWidget(m_downLoad);

    m_vLayout->addLayout(m_hLayout2);

    setAttribute(Qt::WA_StyledBackground);
    setFixedSize(280, 100);
    // setObjectName("QChatFileWnd");
    // setStyleSheet("#QChatFileWnd{border:4px solid gray;}");

    connect(m_openFileDir,
            SIGNAL(clicked()),
            this,
            SLOT(slotOpenFileDirBtnClick()));
    connect(m_openFile, SIGNAL(clicked()), this, SLOT(slotOpenFileBtnClick()));
    connect(m_downLoad,
            SIGNAL(clicked()),
            this,
            SLOT(slotDownloadFileBtnClick()));
}

void QChatFileInnerWnd::slotOpenFileBtnClick()
{
    if (m_fileFullpath != "")
    {
        QDesktopServices::openUrl(QUrl(m_fileFullpath));
    }
}

void QChatFileInnerWnd::slotCopyFileBtnClick()
{
}

void QChatFileInnerWnd::slotOpenFileDirBtnClick()
{
    //
    if (m_fileFullDir != "")
    {
        QDesktopServices::openUrl(QUrl(m_fileFullDir));
    }
}

void QChatFileInnerWnd::slotDownloadFileBtnClick()
{
    if (m_serveFilePath != "")
    {
        //����Զ���ļ�����ʾ������
        m_pNetManager = new QNetworkAccessManager(this);
        QString downLoadFileUrl = m_serveFilePath;
        QNetworkReply* reply =
            m_pNetManager->get(QNetworkRequest(QUrl(downLoadFileUrl)));
        connect(reply,
                &QNetworkReply::downloadProgress,
                this,
                [this, reply](qint64 x, qint64 y) {
                    //��ʾ���ؽ���
                    m_progressBar->setMinimum(0);
                    m_progressBar->setMaximum(y);
                    m_progressBar->setValue(x);
                });

        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            m_sendState->setText("�������");
            sendFileShow();
            QString currpath =
                QDataManager::getMgr()->m_localRecvFileDir + m_fileName->text();
            QFile file(currpath);
            QFileInfo fileinfo = QFileInfo(currpath);
            if (file.open(QIODevice::WriteOnly))
            {
                file.write(reply->readAll());
                file.close();
                m_fileFullpath = fileinfo.absoluteFilePath();
                m_fileFullDir = fileinfo.absolutePath();
            }

            // �ͷ�����
            reply->deleteLater();
            delete m_pNetManager;
            m_pNetManager = nullptr;
        });
    }
}

void QChatFileInnerWnd::sendFileShow()
{
    m_openFile->show();
    m_openFileDir->show();
    m_copyFile->show();
    m_downLoad->hide();
}

void QChatFileInnerWnd::recvFileShow()
{
    m_openFile->hide();
    m_openFileDir->hide();
    m_copyFile->hide();
    m_downLoad->show();
}
