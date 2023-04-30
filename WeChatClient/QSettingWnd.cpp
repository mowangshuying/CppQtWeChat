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
    m_centerWnd = new QWidget(this);
    m_centerWnd->setFixedSize(560, 480);
    m_centerWnd->setObjectName("QSettingWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + m_centerWnd->objectName() + ".qss";
    object.m_widget = m_centerWnd;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    setFixedSize(560, 480);
    // ����Ϊ�ޱ߿򼰱���͸��
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    m_vLayout = new QVBoxLayout(m_centerWnd);
    m_centerWnd->setLayout(m_vLayout);

    m_hTitleLayout = new QHBoxLayout(m_centerWnd);
    // ��С����ť
    m_minBtn = new QPushButton();
    m_minBtn->setIcon(QPixmap("./img/minBtn_.png"));
    m_minBtn->setIconSize(QSize(20, 20));
    m_minBtn->setFixedSize(20, 20);

    // �رհ�ť
    m_closeBtn = new QPushButton();
    m_closeBtn->setIcon(QPixmap("./img/closeBtn_.png"));
    m_closeBtn->setIconSize(QSize(20, 20));
    m_closeBtn->setFixedSize(20, 20);

    // ��ӵ���
    m_hTitleLayout->addStretch();
    m_hTitleLayout->addWidget(m_minBtn);
    m_hTitleLayout->addWidget(m_closeBtn);

    m_vLayout->addLayout(m_hTitleLayout);

    m_hVerLayout = new QHBoxLayout(m_centerWnd);
    m_keyVerLabel = new QLabel("�汾��Ϣ:");
    m_valueVerLabel = new QLabel();
    m_valueVerLabel->setText(WECHAT_VERSION);
    m_hVerLayout->addSpacing(25);
    m_hVerLayout->addWidget(m_keyVerLabel);
    m_hVerLayout->addSpacing(60);
    m_hVerLayout->addWidget(m_valueVerLabel);
    m_hVerLayout->addStretch();

    m_themeLayout = new QHBoxLayout(m_centerWnd);
    m_keyThemeLabel = new QLabel();
    m_keyThemeLabel->setText("��������:  ");
    m_valueThemeComboBox = new QComboBox(m_centerWnd);
    m_valueThemeComboBox->addItem("ǳɫ����");
    m_valueThemeComboBox->addItem("��ɫ����");

    m_themeLayout->addSpacing(25);
    m_themeLayout->addWidget(m_keyThemeLabel);
    m_themeLayout->addSpacing(50);
    m_themeLayout->addWidget(m_valueThemeComboBox);
    m_themeLayout->addStretch();

    m_vLayout->addLayout(m_themeLayout);
    m_vLayout->addLayout(m_hVerLayout);
    m_vLayout->addStretch();

    connect(m_minBtn, SIGNAL(clicked()), this, SLOT(slotClickedCloseBtn()));
    connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(slotClickedMinBtn()));
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

void QSettingWnd::slotClickedCloseBtn()
{
    hide();
}

void QSettingWnd::slotClickedMinBtn()
{
    showMinimized();
}
