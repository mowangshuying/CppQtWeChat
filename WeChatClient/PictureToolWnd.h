#pragma once

#include "QPch.h"

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>

class PictureToolWnd : public QWidget
{
    Q_OBJECT
  public:
    PictureToolWnd(QWidget* p = nullptr);

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

  public slots:
    void slotUploadBtnClicked();
    void slotCancelBtnClicked();
    void slotDetermineBtnClicked();
    void slotCloseWnd();
    void SlotMinWnd();

  public:
    QWidget* m_centerWnd;

    QLabel* m_picLable;
    QPixmap m_headImg;
    QString m_filePath;

    QVBoxLayout* m_vLayout;

    QHBoxLayout* m_hLayout1;
    QPushButton* m_minBtn;
    QPushButton* m_closeBtn;

    QHBoxLayout* m_hLayout2;

    QPushButton* m_determineBtn;
    QPushButton* m_cancelBtn;
    QPushButton* m_uploadBtn;

    bool m_bPress = false;
    QPoint m_poPress;
};
