#pragma once

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include "QEmoijWnd.h"
class QSessionToolBar : public QWidget
{
    Q_OBJECT
public:
    QSessionToolBar(QWidget* p = nullptr);
public slots:
    void slot_emojiBtnClick();
    void slot_screenshotBtnClick();

public:
    QHBoxLayout* m_hLayout;
    QPushButton* m_emojiBtn;
    QPushButton* m_sendFileBtn;
    QPushButton* m_screenshotBtn;

    QEmoijWnd* m_emoijWnd;
};
