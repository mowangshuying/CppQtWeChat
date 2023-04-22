#pragma once

#include "QPch.h"

#include <QWidget>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QCheckBox>

class QFindFriendOrGroupWnd : public QWidget
{
    Q_OBJECT
public:
    QFindFriendOrGroupWnd(QWidget* p = nullptr);

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void addFriendItem(const char* headUrl, const char* name, int64_t userid);
public slots:
    void slotCloseWnd();
    void slotMinWnd();
    void slotOnSearchBtnClicked();

public:
    QWidget* m_centerWnd;
    QVBoxLayout* m_vLayout;
    QHBoxLayout* m_hLayout1;
    // QHBoxLayout* m_hLayout2;
    QHBoxLayout* m_hLayout2;

    //第1行
    QLabel* m_titleLabel;
    QPushButton* m_minBtn;
    QPushButton* m_closeBtn;

    //第2行
    QCheckBox* m_findPersonChx;
    QCheckBox* m_findGroupChx;

    //第3行
    QLineEdit* m_searchEdit;
    QPushButton* m_searchBtn;

    QListWidget* m_listWidget;

    bool m_bPress = false;
    QPoint m_poPress;
};