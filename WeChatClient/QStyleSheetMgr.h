#pragma once

#include <QWidget>
#include <map>
#include <QFile>
#include <QTimer>

class QStyleSheetObject
{
public:
    QWidget* m_widget;  // ����
    QString m_qssFileName;
};

typedef std::vector<QStyleSheetObject> QStyleSheetObjectVct;  

class QStyleSheetMgr : public QObject
{
    Q_OBJECT
private:
    QStyleSheetMgr(QObject* parent = nullptr);
    static QStyleSheetMgr* m_mgr;

public slots:
    void update()
    {
        run();
    }

public:  // ��Ա����
    void init();

    void run();

    void reg(QString wndObjectName, QStyleSheetObject object);

    void unReg(QString wndObjectName);

    static QStyleSheetMgr* getMgr();

    static void freeMgr();

public:
    std::map<QString, QStyleSheetObjectVct> m_map;
    QTimer m_timer;
};
