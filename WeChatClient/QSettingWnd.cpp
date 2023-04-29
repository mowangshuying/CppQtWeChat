#include "QSettingWnd.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QPushButton>
#include "QStyleSheetMgr.h"
#include <QMouseEvent>
#include "QSimpleSplit.h"

QSettingWnd::QSettingWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    QWidget* m_centerWnd = new QWidget(this);
    m_centerWnd->setFixedSize(560, 480);
    m_centerWnd->setObjectName("QSettingWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + m_centerWnd->objectName() + ".qss";
    object.m_widget = m_centerWnd;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    setFixedSize(560, 480);
    // 设置为无边框及背景透明
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // 水平布局为主要布局
    QHBoxLayout* m_hLayout = new QHBoxLayout(m_centerWnd);
    m_centerWnd->setLayout(m_hLayout);

    QVBoxLayout* m_leftLayout = new QVBoxLayout();
    m_hLayout->addLayout(m_leftLayout);
    QPushButton* m_themeBtn = new QPushButton();
    m_themeBtn->setText("主题");

    QPushButton* m_aboutBtn = new QPushButton();
    m_aboutBtn->setText("关于");
    m_leftLayout->addWidget(m_themeBtn);
    m_leftLayout->addWidget(m_aboutBtn);
    m_leftLayout->addStretch();

    QSimpleSplit* m_split = new QSimpleSplit(m_centerWnd, QSimpleSplit::Direction_V);
    m_hLayout->addWidget(m_split);

    QStackedLayout* m_rightLayout = new QStackedLayout();
    m_hLayout->addLayout(m_rightLayout);

    QWidget* m_aboutWnd = new QWidget();
    m_rightLayout->addWidget(m_aboutWnd);
}

void QSettingWnd::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_leftBtnPressed)
    {
        return;
    }

    move(event->pos() - m_LeftBtnPressPoint + pos());
}

void QSettingWnd::mousePressEvent(QMouseEvent* event)
{
    m_leftBtnPressed = true;
    m_LeftBtnPressPoint = event->pos();
}

void QSettingWnd::mouseReleaseEvent(QMouseEvent* event)
{
    m_leftBtnPressed = false;
}
