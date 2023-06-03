#pragma once

#include "QPch.h"

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include "QMainWnd.h"

//��¼��ע�ᴰ��
class QLoginAndRegWnd : public QWidget
{
    Q_OBJECT
public:
    QLoginAndRegWnd(QWidget* p = nullptr);
    ~QLoginAndRegWnd()
    {
        if (m_mainWnd != nullptr)
            delete m_mainWnd;
    }

    void regSignalSlot();

public slots:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void slotCloseWnd();
    void slotMinWnd();
    void slotRegOrLoginSel(bool isSel = false);
    void slotRegOrLoginBtn();

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
    QCheckBox* m_regOrLoginChx;
    QCheckBox* m_remmerPwdChx;

    QMainWnd* m_mainWnd = nullptr;

    //���ڿ��ƴ��ڵ��ƶ�
    bool m_bPress = false;
    QPoint m_poPress;

    bool m_bReg = false;
};