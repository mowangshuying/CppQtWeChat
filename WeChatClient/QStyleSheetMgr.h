#pragma once

#include <QWidget>
#include <map>
#include <QFile>
#include <QTimer>
#include <QFileSystemWatcher>

class QStyleSheetObject
{
public:
    QWidget* m_widget;  // 窗口
    QString m_qssFileName;
};

typedef std::vector<QStyleSheetObject> QStyleSheetObjectVct;

class QStyleSheetObectWatcher
{
public:
    QFileSystemWatcher* m_watcher;
    QStyleSheetObjectVct m_objectVct;
};

class QStyleSheetMgr : public QObject
{
    Q_OBJECT
private:
    QStyleSheetMgr(QObject* parent = nullptr);
    ~QStyleSheetMgr();
    static QStyleSheetMgr* m_mgr;

public:  // 成员函数
    void reg(QString wndObjectName, QStyleSheetObject object);

    void reg(QString wndObjectName, QString qssFileName, QWidget* wnd)
    {
        QStyleSheetObject styleSheetObject;
        styleSheetObject.m_widget = wnd;
        styleSheetObject.m_qssFileName = qssFileName;
        reg(wndObjectName, styleSheetObject);
    }

    void unReg(QString wndObjectName);

    static QStyleSheetMgr* getMgr();

    static void freeMgr();

public:
    std::map<QString, QStyleSheetObectWatcher> m_map;
};
