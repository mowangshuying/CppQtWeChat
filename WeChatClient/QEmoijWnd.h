#pragma once

#include <QWidget>
#include <QEvent>
#include <QVBoxLayout>
#include <QTableWidget>

class QEmoijWnd : public QWidget
{
    Q_OBJECT
public:
    QEmoijWnd(QWidget* p = nullptr);

protected:
    bool event(QEvent* event);

    void paintEvent(QPaintEvent* paintEvent);
signals:
    void signal_emoijClicked(QString m_emoijString);
public slots:
    //判断哪个单元格子被点击
    void slot_cellClicked(int x, int y);

public:
    QStringList m_emoijStrList;
    QTableWidget* m_centerWnd;
    QVBoxLayout* m_vLayout;
};
