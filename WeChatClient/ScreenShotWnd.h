#pragma once

#include "QPch.h"

#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>
#include <QMenu>
#include "ScreenShotToolBarWnd.h"

class ScreenShotWnd : public QWidget
{
    Q_OBJECT
  public:
    ScreenShotWnd(QWidget* p = nullptr);
    ~ScreenShotWnd();
  public slots:
    // 复制截图
    void slotCopyScreenShot();
    // 保存截图
    void slotSaveScreenShot();
    // 退出截图
    void slotExitSccreenShot();

  protected:
    void showEvent(QShowEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

    void contextMenuEvent(QContextMenuEvent*) override;  //--右键菜单事件
    void keyPressEvent(QKeyEvent* e) override;           //--按键事件
  public:
    QPixmap m_fullScreenPixmap;

  private:
    // 鼠标开始位置
    QPoint m_begPos;
    // 鼠标结束位置
    QPoint m_endPos;

    // 鼠标开始全局位置
    QPoint m_globalBegPos;
    // 鼠标结束全局位置
    QPoint m_globalEndPos;

    // 矩形截图区域
    QRect m_screenShotRect;
    // 左键是否按下
    bool m_leftBtnPress;

    // 右键菜单
    QMenu* m_RightBtnMenu;

    QScreen* m_screen;

    ScreenShotToolBarWnd* m_toolBarWnd;

    bool m_isPainting;
    bool m_patingSuc;
};
