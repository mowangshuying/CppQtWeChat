#include "QSessionToolBar.h"
#include <QAppliCation>
#include <QPixMap>
#include <QDesktopWidget>
#include "QScreenShotWnd.h"
#include <QDebug>
#include <QScreen>
#include "QVoiceTelphoneWnd.h"

QSessionToolBar::QSessionToolBar(QWidget* p /*= nullptr*/) : QWidget(p)
{
    LogFunc;
    setObjectName("QSessionToolBar");

    setFixedHeight(40);
    // setStyleSheet("boder:2px gray");

    m_hLayout = new QHBoxLayout();
    setLayout(m_hLayout);

    m_emojiBtn = new QPushButton();
    m_emojiBtn->setIcon(QPixmap("./img/emojiBtn.png"));
    m_emojiBtn->setIconSize(QSize(30, 30));
    m_emojiBtn->setFixedSize(QSize(30, 30));

    m_sendFileBtn = new QPushButton();
    m_sendFileBtn->setIcon(QPixmap("./img/sendFileBtn.png"));
    m_sendFileBtn->setIconSize(QSize(30, 30));
    m_sendFileBtn->setFixedSize(QSize(30, 30));

    m_screenshotBtn = new QPushButton();
    m_screenshotBtn->setIcon(QPixmap("./img/screenshotBtn.png"));
    m_screenshotBtn->setIconSize(QSize(30, 30));
    m_screenshotBtn->setFixedSize(QSize(30, 30));

    m_voiceTelphoneBtn = new QPushButton();
    m_voiceTelphoneBtn->setIcon(QPixmap("./img/voiceTelphoneBtn.png"));
    m_voiceTelphoneBtn->setIconSize(QSize(30, 30));
    m_voiceTelphoneBtn->setFixedSize(QSize(30, 30));
    // m_voiceTelphoneBtn->setStyleSheet(" border:2px solid rgb(0,0,0);");

    m_emoijWnd = new QEmoijWnd();
    m_emoijWnd->hide();

    m_hLayout->addWidget(m_emojiBtn);
    m_hLayout->addWidget(m_sendFileBtn);
    m_hLayout->addWidget(m_screenshotBtn);
    m_hLayout->addStretch();
    m_hLayout->addWidget(m_voiceTelphoneBtn);
    m_hLayout->addSpacing(15);

    connect(m_emojiBtn, SIGNAL(clicked()), this, SLOT(slotEmojiBtnClick()));
    connect(m_screenshotBtn, SIGNAL(clicked()), this, SLOT(slotScreenshotBtnClick()));
}

void QSessionToolBar::slotEmojiBtnClick()
{
    QPoint gPoint = m_emojiBtn->mapToGlobal(QPoint(0, 0));

    /*选择的窗口只能允许出现一个*/
    // QWidget *qw = new QSelectWnd1(nullptr);
    QRect swRect = m_emoijWnd->geometry();
    swRect.setX(gPoint.x() - m_emoijWnd->width() / 2 + m_emojiBtn->width() / 2);
    swRect.setY(gPoint.y() - m_emoijWnd->height());
    m_emoijWnd->setGeometry(swRect);
    m_emoijWnd->show();
}

// 参考资料：https://www.cnblogs.com/syh6324/p/9502307.html
void QSessionToolBar::slotScreenshotBtnClick()
{
    LogDebug << "desktop winId:" << QApplication::desktop()->winId();
    LogDebug << "desktop sceen count:" << QApplication::desktop()->screenCount();
    LogDebug << "desktop width:" << QApplication::desktop()->width() << " height:" << QApplication::desktop()->height();

    // 获取当前鼠标位置
    QPoint mousePoint = QCursor().pos();
    LogDebug << __FUNCTION__ << "mouse point x:" << mousePoint.x() << " y:" << mousePoint.y();
    int nSceenCount = QApplication::desktop()->screenCount();
    int nSceenWidth = QApplication::desktop()->width();
    int nEverySceenWidth = nSceenWidth / nSceenCount;
    int nCurScreenIndex = -1;
    for (int i = 0; i < nSceenCount; i++)
    {
        if (mousePoint.x() >= nEverySceenWidth * i && mousePoint.x() <= nEverySceenWidth * (i + 1))
        {
            nCurScreenIndex = i;
            break;
        }
    }

    LogDebug << __FUNCTION__ << " nCurScreenIndex:" << nCurScreenIndex;

    // 如果原来含有截图窗口，释放原来的截图窗口内存，重新分配内存
    if (m_screenShotWnd != nullptr)
    {
        delete m_screenShotWnd;
        m_screenShotWnd = nullptr;
    }

    m_screenShotWnd = new QScreenShotWnd();
    m_screenShotWnd->showFullScreen();
}
