#pragma once

#include "QPch.h"

#include <QWidget>
#include <QPushButton>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class ApplyFriendNextWnd : public QWidget
{
    Q_OBJECT
  public:
    ApplyFriendNextWnd(QWidget* p = nullptr,
                        int64_t friendid = -1,
                        QString username = "");

    enum class PushBtnState
    {
        PBS_Next = 0,
        PBS_Close = 1
    };

  public:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

  public slots:
    void closeWnd();
    void minWnd();
    void slotPushBtnClick();

  public:
    QWidget* m_centerWnd;
    int m_index = 0;

    QLabel* m_titleLabel;
    QPushButton* m_minBtn;
    QPushButton* m_closeBtn;

    QWidget* m_wnd1;
    QWidget* m_wnd2;

    QPushButton* m_pushBtn;
    QStackedLayout* m_sLayout;
    QVBoxLayout* m_vLayout;

    QHBoxLayout* m_hLayout1;
    QHBoxLayout* m_hLayout2;

    bool m_bPress = false;
    QPoint m_poPress;

    PushBtnState m_state;

    int64_t m_friendid;
    QString m_username;
};
