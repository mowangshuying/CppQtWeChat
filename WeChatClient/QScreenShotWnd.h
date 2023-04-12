#pragma once

#include "QPch.h"

#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>
#include <QMenu>
#include "QScreenShotToolBarWnd.h"

class QScreenShotWnd : public QWidget
{
    Q_OBJECT
public:
    QScreenShotWnd(QWidget* p = nullptr);
public slots:
    //���ƽ�ͼ
    void slotCopyScreenShot();
    //�����ͼ
    void slotSaveScreenShot();
    //�˳���ͼ
    void slotExitSccreenShot();

protected:
    void showEvent(QShowEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

    void contextMenuEvent(QContextMenuEvent*) override;  //--�Ҽ��˵��¼�
    void keyPressEvent(QKeyEvent* e) override;           //--�����¼�
public:
public:
    QPixmap m_fullScreenPixmap;

private:
    //��꿪ʼλ��
    QPoint m_begPos;
    //������λ��
    QPoint m_endPos;

    // ��꿪ʼȫ��λ��
    QPoint m_globalBegPos;
    // ������ȫ��λ��
    QPoint m_globalEndPos;

    //���ν�ͼ����
    QRect m_screenShotRect;
    //����Ƿ���
    bool m_leftBtnPress;

    //�Ҽ��˵�
    QMenu* m_RightBtnMenu;

    QScreen* m_screen;

    QScreenShotToolBarWnd* m_toolBarWnd;

    bool m_isPainting;
    bool m_patingSuc;
};
