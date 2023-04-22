#pragma once

#include "QPch.h"

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>

class QPictureToolWnd : public QWidget
{
    Q_OBJECT
public:
    QPictureToolWnd(QWidget* p = nullptr);

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

public slots:
    void slotUploadBtnClicked();
    void slotCancelBtnClicked();
    void slotDetermineBtnClicked();
    void closeWnd();
    void minWnd();

public:
    QWidget* m_centerWnd;

    // "./img/default.png"
    QLabel* m_picLable;
    QPixmap m_HeadImg;
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
