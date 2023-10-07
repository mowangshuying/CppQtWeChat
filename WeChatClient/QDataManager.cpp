#include "QDataManager.h"
#include "QPch.h"

QDataManager* QDataManager::m_dataManager = nullptr;

QDataManager::QDataManager()
{
    LogFunc;
    QFile emoijFile("./emoij/emoij.txt");
    if (emoijFile.exists() &&
        emoijFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString str = emoijFile.readAll();
        m_emoijStrList = str.split('|', QString::SkipEmptyParts);
        emoijFile.close();
    }
}

QDataManager* QDataManager::getMgr()
{
    if (m_dataManager == nullptr)
    {
        m_dataManager = new QDataManager();
    }
    return m_dataManager;
}

void QDataManager::exitMgr()
{
    if (m_dataManager != nullptr)
    {
        delete m_dataManager;
        m_dataManager = nullptr;
    }
}

void QDataManager::setUserIdAndName(int64_t userId, QString userName)
{
    m_userid = userId;
    m_username = userName;
}

void QDataManager::setUserHeadImg(int64_t userId, QPixmap pixmap)
{
    m_UserId2HeadImgMap[userId] = pixmap;
}
