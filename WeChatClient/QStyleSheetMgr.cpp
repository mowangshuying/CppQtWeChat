#include "QStyleSheetMgr.h"
#include "QSelfLog.h"

QStyleSheetMgr* QStyleSheetMgr::m_mgr = nullptr;

QStyleSheetMgr::QStyleSheetMgr(QObject* parent /* = nullptr*/) : QObject(parent)
{
}

void QStyleSheetMgr::init()
{
    m_timer.setInterval(5000);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer.start();
}

void QStyleSheetMgr::run()
{
    for (auto itMapBeg = m_map.begin(); itMapBeg != m_map.end(); itMapBeg++)
    {
        for (auto itBegVct = itMapBeg->second.begin(); itBegVct != itMapBeg->second.end(); itBegVct++)
        {
            QWidget* wnd = itBegVct->m_widget;
            QString qssFileName = itBegVct->m_qssFileName;
            if (qssFileName.isEmpty() || wnd == nullptr)
            {
                continue;
            }

            QFile qss(qssFileName);
            if (qss.open(QFile::ReadOnly))
            {
                wnd->setStyleSheet(qss.readAll());
                LogDebug << "update qss fileName:" << qssFileName << "every 5 s";
                qss.close();
            }
        }
    }
}

void QStyleSheetMgr::reg(QString wndObjectName, QStyleSheetObject object)
{
    // m_map[wndObjectName] = object;
    auto itf = m_map.find(wndObjectName);
    if (itf != m_map.end())
    {
        itf->second.push_back(object);
    }
    else
    {
        QStyleSheetObjectVct vct;
        vct.push_back(object);
        m_map[wndObjectName] = vct;
    }
}

void QStyleSheetMgr::unReg(QString wndObjectName)
{
    auto itf = m_map.find(wndObjectName);
    if (itf != m_map.end())
    {
        m_map.erase(itf);
    }
}

QStyleSheetMgr* QStyleSheetMgr::getMgr()
{
    if (m_mgr == nullptr)
    {
        m_mgr = new QStyleSheetMgr();
    }

    return m_mgr;
}
