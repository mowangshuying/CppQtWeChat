#include "QSessionToolBar.h"
#include <QAppliCation>
#include <QPixMap>
#include <QDesktopWidget>
#include "QScreenShotWnd.h"

QSessionToolBar::QSessionToolBar(QWidget* p /*= nullptr*/)
	: QWidget(p)
{
	setFixedHeight(40);
	setStyleSheet("boder:2px gray");

	m_hLayout = new QHBoxLayout();
	setLayout(m_hLayout);

	m_emojiBtn = new QPushButton();
	m_emojiBtn->setIcon(QPixmap("./img/emojiBtn.png"));
	m_emojiBtn->setIconSize(QSize(30, 30));
	
	m_sendFileBtn = new QPushButton();
	m_sendFileBtn->setIcon(QPixmap("./img/sendFileBtn.png"));
	m_sendFileBtn->setIconSize(QSize(30, 30));

	m_screenshotBtn = new QPushButton();
	m_screenshotBtn->setIcon(QPixmap("./img/screenshotBtn.png"));
	m_screenshotBtn->setIconSize(QSize(30, 30));
	
	m_emoijWnd = new QEmoijWnd();
	m_emoijWnd->hide();
	
	m_hLayout->addWidget(m_emojiBtn);
	m_hLayout->addWidget(m_sendFileBtn);
	m_hLayout->addWidget(m_screenshotBtn);

	m_hLayout->addStretch();
	connect(m_emojiBtn, SIGNAL(clicked()), this, SLOT(slot_emojiBtnClick()));
	connect(m_screenshotBtn, SIGNAL(clicked()), this, SLOT(slot_screenshotBtnClick()));
}

void QSessionToolBar::slot_emojiBtnClick()
{
	QPoint gPoint = m_emojiBtn->mapToGlobal(QPoint(0, 0));

	/*选择的窗口只能允许出现一个*/
	//QWidget *qw = new QSelectWnd1(nullptr);
	QRect swRect = m_emoijWnd->geometry();
	swRect.setX(gPoint.x() - m_emoijWnd->width()/2 + m_emojiBtn->width()/2);
	swRect.setY(gPoint.y() - m_emoijWnd->height());
	m_emoijWnd->setGeometry(swRect);
	m_emoijWnd->show();
}

void QSessionToolBar::slot_screenshotBtnClick()
{
	QScreenShotWnd* s = new QScreenShotWnd();
	s->m_fullScreenPixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
	s->showFullScreen();
}
