#pragma once

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include "QEmoijWnd.h"
#include "QVoiceTelphoneWnd.h"

class QSessionToolBar : public QWidget
{
    Q_OBJECT
public:
    QSessionToolBar(QWidget* p = nullptr);
public slots:
    void slotEmojiBtnClick();
    void slotScreenshotBtnClick();

public:
    QHBoxLayout* m_hLayout;
    QPushButton* m_emojiBtn;
    QPushButton* m_sendFileBtn;
    QPushButton* m_screenshotBtn;
    QPushButton* m_voiceTelphoneBtn;

    QEmoijWnd* m_emoijWnd;
    QVoiceTelphoneWnd* m_phoneWnd;
};
