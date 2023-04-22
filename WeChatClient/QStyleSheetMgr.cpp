#include "QStyleSheetMgr.h"
#include "QSelfLog.h"

QStyleSheetMgr* QStyleSheetMgr::m_mgr = nullptr;

QStyleSheetMgr::QStyleSheetMgr(QObject* parent /* = nullptr*/) : QObject(parent)
{
}

QStyleSheetMgr::~QStyleSheetMgr()
{
    for (auto& watch : m_map)
    {
        // watch.second.m_objectVct.clear();
        delete watch.second.m_watcher;
        watch.second.m_watcher = nullptr;
    }

    m_map.clear();
}

void QStyleSheetMgr::init()
{
    // m_timer.setInterval(100);
    // connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
    // m_timer.start();
}

void QStyleSheetMgr::run()
{
    // for (auto itMapBeg = m_map.begin(); itMapBeg != m_map.end(); itMapBeg++)
    // {
    //     for (auto itBegVct = itMapBeg->second.begin(); itBegVct != itMapBeg->second.end(); itBegVct++)
    //     {
    //         QWidget* wnd = itBegVct->m_widget;
    //         QString qssFileName = itBegVct->m_qssFileName;
    //         if (qssFileName.isEmpty() || wnd == nullptr)
    //         {
    //             continue;
    //         }

    //         QFile qss(qssFileName);
    //         if (qss.open(QFile::ReadOnly))
    //         {
    //             wnd->setStyleSheet(qss.readAll());
    //             LogDebug << "update qss fileName:" << qssFileName << "every 5 s";
    //             qss.close();
    //         }
    //     }
    // }
}

void QStyleSheetMgr::reg(QString wndObjectName, QStyleSheetObject object)
{
    // m_map[wndObjectName] = object;
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

        connect(fileWatcher, &QFileSystemWatcher::fileChanged, [wndObjectName, this](const QString& path) {
            LogDebug << "file changed path:" << path;
            QFile qss(path);
            if (qss.open(QIODevice::ReadOnly))
            {
                // object.m_widget->setStyleSheet(qss.readAll());
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
