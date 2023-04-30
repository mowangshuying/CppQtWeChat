#pragma once
#include "QPch.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedLayout>
#include "QSimpleSplit.h"
#include "QSettingAboutWnd.h"
#include <QComboBox>

class QSettingWnd : public QWidget
{
public:
    QSettingWnd(QWidget* p = nullptr);

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

public:
    bool m_leftBtnPressed = false;
    QPoint m_LeftBtnPressPoint;

    QWidget* m_centerWnd;
    QVBoxLayout* m_vLayout;

    QHBoxLayout* m_hTitleLayout;
    QPushButton* m_minBtn;
    QPushButton* m_closeBtn;

    //  QHBoxLayout* m_hContentLayout;
    // QStackedLayout* m_rightLayout;
    // QWidget* m_aboutWnd;

    QHBoxLayout* m_hVerLayout;
    QLabel* m_keyVerLabel;
    QLabel* m_valueVerLabel;

    // ÷˜Ã‚…Ë÷√
    QHBoxLayout* m_themeLayout;
    QLabel* m_keyThemeLabel;
    QComboBox* m_valueThemeComboBox;
};
