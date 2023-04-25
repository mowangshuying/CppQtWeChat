#pragma once

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include "QEmoijWnd.h"
#include "QVoiceTelphoneWnd.h"
#include "QScreenShotWnd.h"

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
    QEmoijWnd* m_emoijWnd;
    QPushButton* m_emojiBtn;
    QPushButton* m_screenshotBtn;
    QPushButton* m_sendFileBtn;
    QPushButton* m_voiceTelphoneBtn;
    QScreenShotWnd* m_screenShotWnd = nullptr;
    QVoiceTelphoneWnd* m_phoneWnd;
};
