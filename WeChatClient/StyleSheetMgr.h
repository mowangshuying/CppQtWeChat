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

class StyleSheetMgr : public QObject
{
    Q_OBJECT
  private:
    StyleSheetMgr(QObject* parent = nullptr);
    ~StyleSheetMgr();
    static StyleSheetMgr* m_mgr;

  public:  // 成员函数
    void reg(QString wndObjectName, QStyleSheetObject object);

    void reg(QString wndObjectName, QString qssFileName, QWidget* wnd);

    void unReg(QString wndObjectName, QWidget* wnd);

    static StyleSheetMgr* getMgr();

    static void initMgr()
    {
        getMgr();
    }

    static void exitMgr()
    {
        if (m_mgr)
            delete m_mgr;
    }

  public:
    std::map<QString, QStyleSheetObectWatcher> m_map;
};
