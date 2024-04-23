#include "StyleSheetMgr.h"
#include "Log.h"

StyleSheetMgr* StyleSheetMgr::m_mgr = nullptr;

StyleSheetMgr::StyleSheetMgr(QObject* parent /* = nullptr*/) : QObject(parent)
{
}

StyleSheetMgr::~StyleSheetMgr()
{
    for (auto& watch : m_map)
    {
        delete watch.second.m_watcher;
        watch.second.m_watcher = nullptr;
    }

    m_map.clear();
}

void StyleSheetMgr::reg(QString wndObjectName, QStyleSheetObject object)
{
    LogDebug << "reg:" << wndObjectName;
    auto itf = m_map.find(wndObjectName);
    if (itf != m_map.end())
    {
        itf->second.m_objectVct.push_back(object);
    }
    else
    {
        QStyleSheetObectWatcher watcher;
        watcher.m_objectVct.push_back(object);

        QFileSystemWatcher* fileWatcher = new QFileSystemWatcher();
        fileWatcher->addPath(object.m_qssFileName);

        watcher.m_watcher = fileWatcher;
        m_map[wndObjectName] = watcher;

        connect(fileWatcher,
                &QFileSystemWatcher::fileChanged,
                [wndObjectName, this](const QString& path) {
                    LogDebug << "file changed path:" << path;
                    QFile qss(path);
                    if (qss.open(QIODevice::ReadOnly))
                    {
                        QString qssStr = qss.readAll();
                        qss.close();
                        for (auto& object : m_map[wndObjectName].m_objectVct)
                        {
                            object.m_widget->setStyleSheet(qssStr);
                        }
                    }
                });
    }

    QFile qss(object.m_qssFileName);
    if (qss.open(QIODevice::ReadOnly))
    {
        QString qssStr = qss.readAll();
        qss.close();
        object.m_widget->setStyleSheet(qssStr);
    }
}

void StyleSheetMgr::reg(QString wndObjectName,
                         QString qssFileName,
                         QWidget* wnd)
{
    QStyleSheetObject styleSheetObject;
    styleSheetObject.m_widget = wnd;
    styleSheetObject.m_qssFileName = qssFileName;
    reg(wndObjectName, styleSheetObject);
}

void StyleSheetMgr::unReg(QString wndObjectName, QWidget* wnd)
{
    auto itf = m_map.find(wndObjectName);
    if (itf != m_map.end())
    {
        auto itVctBeg = itf->second.m_objectVct.begin();
        auto itVctEnd = itf->second.m_objectVct.end();
        for (; itVctBeg != itVctEnd; itVctBeg++)
        {
            if (itVctBeg->m_widget == wnd)
            {
                itf->second.m_objectVct.erase(itVctBeg);
                return;
            }
        }
    }
}

StyleSheetMgr* StyleSheetMgr::getMgr()
{
    if (m_mgr == nullptr)
    {
        m_mgr = new StyleSheetMgr();
    }

    return m_mgr;
}
