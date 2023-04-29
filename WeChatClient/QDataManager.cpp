#include "QDataManager.h"

QDataManager* QDataManager::m_dataManager = nullptr;

 QDataManager::QDataManager()
{
    QFile emoijFile("./emoij/emoij.txt");
    if (emoijFile.exists() && emoijFile.open(QIODevice::ReadOnly | QIODevice::Text))
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

void QDataManager::freeMgr()
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
