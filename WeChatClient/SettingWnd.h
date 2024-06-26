#pragma once
#include "def.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedLayout>
#include "SelfSplit.h"
#include <QComboBox>
#include <QLabel>

class SettingWnd : public QWidget
{
    Q_OBJECT
  public:
    SettingWnd(QWidget* p = nullptr);

  protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void paintEvent(QPaintEvent* event);

  public slots:
    void slotClickedCloseBtn();
    void slotClickedMinBtn();

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

    // Ö÷ÌâÉèÖÃ
    QHBoxLayout* m_themeLayout;
    QLabel* m_keyThemeLabel;
    QComboBox* m_valueThemeComboBox;
};
