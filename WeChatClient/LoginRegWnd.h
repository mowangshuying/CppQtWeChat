#pragma once

#include "def.h"

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include "MainWnd.h"

// 登录和注册窗口
class LoginRegWnd : public QWidget
{
    Q_OBJECT
  public:
    LoginRegWnd(QWidget* p = nullptr);
    ~LoginRegWnd();

    void regSignalSlot();

    // to enable qss
    void paintEvent(QPaintEvent* event);
  public slots:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void slotCloseWnd();
    void slotMinWnd();
    void slotRegLoginSel(bool isSel = false);
    void slotRegLoginBtnClicked();

  public:
    QWidget* m_centerWnd;

    QVBoxLayout* m_vLayout;

    QWidget* m_topWnd;
    QVBoxLayout* m_vTopLayout;
    QHBoxLayout* m_hTopLayout;

    QLabel* m_titleLabel;
    QPushButton* m_minBtn;
    QPushButton* m_closeBtn;
    QPushButton* m_settingBtn;

    QWidget* m_bottomWnd;
    QHBoxLayout* m_hBottomLayout1;
    QVBoxLayout* m_vBottomLayout;
    QLineEdit* m_accuntEdit;
    QLineEdit* m_pwdEdit;
    QPushButton* m_regOrLoginBtn;

    QHBoxLayout* m_hBottomLayout2;
    QCheckBox* m_regLoginCB;
    QCheckBox* m_remmerPwdCB;

    MainWnd* m_mainWnd = nullptr;

    // 用于控制窗口的移动
    bool m_bPress = false;
    QPoint m_poPress;

    bool m_bReg = false;
};