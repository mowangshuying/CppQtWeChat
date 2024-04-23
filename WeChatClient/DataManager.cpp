#include "DataManager.h"
#include "QPch.h"

DataManager* DataManager::m_dataManager = nullptr;

DataManager::DataManager()
{
    LogFunc;
    QFile emoijFile("./emoij/emoij.txt");
    if (emoijFile.exists() && emoijFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString str = emoijFile.readAll();
        m_emoijStrList = str.split('|', QString::SkipEmptyParts);
        emoijFile.close();
    }
}

DataManager* DataManager::getMgr()
{
    if (m_dataManager == nullptr)
    {
        m_dataManager = new DataManager();
    }
    return m_dataManager;
}

void DataManager::exitMgr()
{
    if (m_dataManager != nullptr)
    {
        delete m_dataManager;
        m_dataManager = nullptr;
    }
}

void DataManager::setUserIdAndName(int64_t userId, QString userName)
{
    m_userid = userId;
    m_username = userName;
}

void DataManager::setUserHeadImg(int64_t userId, QPixmap pixmap)
{
    m_UserId2HeadImgMap[userId] = pixmap;
}
