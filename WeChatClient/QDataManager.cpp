#include "QDataManager.h"

QDataManager* QDataManager::m_dataManager = nullptr;

QDataManager* QDataManager::getInstance()
{
    if (m_dataManager == nullptr)
    {
        m_dataManager = new QDataManager();
    }
    return m_dataManager;
}

void QDataManager::freeInstance()
{
    if (m_dataManager != nullptr)
    {
        delete m_dataManager;
        m_dataManager = nullptr;
    }
}
