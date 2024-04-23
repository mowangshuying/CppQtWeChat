#pragma once

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include "EmoijWnd.h"
#include "VoiceTelphoneWnd.h"
#include "ScreenShotWnd.h"

class SessionToolBar : public QWidget
{
    Q_OBJECT
  public:
    SessionToolBar(QWidget* p = nullptr);
  public slots:
    void slotEmojiBtnClick();
    void slotScreenshotBtnClick();

  public:
    QHBoxLayout* m_hLayout;
    EmoijWnd* m_emoijWnd;
    QPushButton* m_emojiBtn;
    QPushButton* m_screenshotBtn;
    QPushButton* m_sendFileBtn;
    QPushButton* m_voiceTelphoneBtn;
    ScreenShotWnd* m_screenShotWnd = nullptr;
    VoiceTelphoneWnd* m_phoneWnd;
};
