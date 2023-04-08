#include "QScreenShotWnd.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QPen>
#include <QFileDialog>
#include <QGuiApplication>
#include <QClipboard>
#include <QDebug>
#include <qalgorithms.h>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>
#include <QCursor>

QScreenShotWnd::QScreenShotWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QDesktopWidget* destTopWnd = QApplication::desktop();
    int curMonitor = destTopWnd->screenNumber(QCursor().pos());
    LogDebug << "curMonitor:" << curMonitor;

    QList<QScreen*> screens = QApplication::screens();
    LogDebug << "screens size:" << screens.size();
    m_screen = screens[curMonitor];

    QRect screenRect = m_screen->geometry();
    LogDebug << "screenRect:" << screenRect;

    m_fullScreenPixmap = m_screen->grabWindow(0);
    QSize size = m_fullScreenPixmap.size();
    LogDebug << "screen pixmap size:" << m_fullScreenPixmap.size();

    m_begPos = QPoint(-1, -1);
    m_endPos = QPoint(-1, -1);
    m_leftBtnPress = false;
    //开启鼠标实时追踪，实时的显示鼠标的位置
    setMouseTracking(true);

    m_screenShotRect = QRect(0, 0, size.width(), size.height());
    m_RightBtnMenu = new QMenu(this);
    m_RightBtnMenu->addAction("复制(CTRL+C)", this, SLOT(slotCopyScreenShot()));
    m_RightBtnMenu->addAction("保存(Ctrl+S)", this, SLOT(slotSaveScreenShot()));
    m_RightBtnMenu->addAction("退出(ESC)", this, SLOT(slotExitSccreenShot()));
    m_isPainting = false;
    m_patingSuc = false;

    m_toolBarWnd = new QScreenShotToolBarWnd(this);
    m_toolBarWnd->hide();
    connect(m_toolBarWnd->m_copyBtn, SIGNAL(clicked()), this, SLOT(slotCopyScreenShot()));
    connect(m_toolBarWnd->m_saveBtn, SIGNAL(clicked()), this, SLOT(slotSaveScreenShot()));
    connect(m_toolBarWnd->m_closeBtn, SIGNAL(clicked()), this, SLOT(slotExitSccreenShot()));
}

void QScreenShotWnd::slotCopyScreenShot()
{
    QGuiApplication::clipboard()->setPixmap(m_fullScreenPixmap.copy(m_screenShotRect));
    this->close();
    m_toolBarWnd->close();
}

void QScreenShotWnd::slotSaveScreenShot()
{
    QString defaultFileName = QString("截图%1.png").arg(QDateTime::currentDateTime().toString("yyyyhhmmsszzz"));
    QString fileName = QFileDialog::getSaveFileName(this, "截图另存为", defaultFileName, "Image (*.jpg *.png *.bmp)");
    if (fileName.length() > 0)
    {
        m_fullScreenPixmap.copy(m_screenShotRect).save(fileName, "png");
        this->close();
        m_toolBarWnd->close();
    }
}

void QScreenShotWnd::slotExitSccreenShot()
{
    m_toolBarWnd->close();
    this->close();
}

void QScreenShotWnd::showEvent(QShowEvent* showEvent)
{
    // setWindowOpacity(0.2);
    // setStyleSheet("background-color:black;");
}

void QScreenShotWnd::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    if (m_patingSuc)
    {
        return;
    }

    m_leftBtnPress = true;
    m_begPos = event->pos();
    m_globalBegPos = event->globalPos();
    // setStyleSheet("background-color:black;");
}

void QScreenShotWnd::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_leftBtnPress)
    {
        return;
    }

    m_endPos = event->pos();
    m_globalEndPos = event->globalPos();

    QPoint xPoint;
    xPoint.setX(qMin(m_begPos.x(), m_endPos.x()));
    xPoint.setY(qMin(m_begPos.y(), m_endPos.y()));
    int nW = qAbs(m_endPos.x() - m_begPos.x());
    int nH = qAbs(m_endPos.y() - m_begPos.y());
    m_screenShotRect.setRect(xPoint.x(), xPoint.y(), nW, nH);
    update();
}

void QScreenShotWnd::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_leftBtnPress)
    {
        m_leftBtnPress = false;

        m_endPos = event->pos();
        m_globalEndPos = event->globalPos();

        LogDebug << "end pos x = " << m_endPos.x() << ",y = " << m_endPos.y();
        LogDebug << "global end pos x = " << m_globalEndPos.x() << ",y = " << m_globalEndPos.y();

        QPoint xPoint;
        xPoint.setX(qMin(m_begPos.x(), m_endPos.x()));
        xPoint.setY(qMin(m_begPos.y(), m_endPos.y()));
        int nW = qAbs(m_endPos.x() - m_begPos.x());
        int nH = qAbs(m_endPos.y() - m_begPos.y());

        m_screenShotRect.setRect(xPoint.x(), xPoint.y(), nW, nH);
        LogDebug << "mouse release event:"
                 << "x = " << xPoint.x() << ",y = " << xPoint.y() << ",w = " << nW << ",h = " << nH;
        update();

        if (m_screenShotRect.height() > 20 && m_screenShotRect.width() > 20)
        {
            m_patingSuc = true;
            m_toolBarWnd->show();

            QRect tmpRect;
            tmpRect.setX(m_globalEndPos.x() - m_toolBarWnd->width());
            tmpRect.setY(m_globalEndPos.y());
            tmpRect.setHeight(m_toolBarWnd->height());
            tmpRect.setWidth(m_toolBarWnd->width());
            m_toolBarWnd->setGeometry(tmpRect);
            m_toolBarWnd->show();
            LogDebug << "pating suc!";
        }
    }
}

void QScreenShotWnd::paintEvent(QPaintEvent* event)
{
    if (m_isPainting)
    {
        return;
    }

    m_isPainting = true;
    QPainter painter(this);  //将当前窗体对象设置为画布
    QPen pen;
    pen.setColor(qRgba(26, 255, 25, 1.0));  //设置笔色
    pen.setWidth(4);                        //画笔线条宽度
    painter.setPen(pen);                    //设置画笔

    LogDebug << "screen rect: x = " << m_screenShotRect.x() << ", y = " << m_screenShotRect.y() << ", w = " << m_screenShotRect.width()
             << ",h = " << m_screenShotRect.height();
    //防止第一次就重绘 并且宽高大于0时才进行截图操作
    if (m_screenShotRect.width() > 0 && m_screenShotRect.height() > 0)
    {
        LogDebug << "paint it!";
        painter.drawPixmap(m_screenShotRect, m_fullScreenPixmap, m_screenShotRect);
        painter.drawRect(m_screenShotRect);
        // painter.drawText(cursor().pos().x(), cursor().pos().y(), tr("(%1,%2)").arg(cursor().pos().x()).arg(cursor().pos().y()));
    }
    m_isPainting = false;
}

void QScreenShotWnd::contextMenuEvent(QContextMenuEvent*)
{
    // this->setCursor(Qt::ArrowCursor);      //设置鼠标的样式为箭头
    // m_RightBtnMenu->exec(cursor().pos());  //菜单显示的位置跟随鼠标
}

void QScreenShotWnd::keyPressEvent(QKeyEvent* e)
{
    /// Esc 键退出截图;
    if (e->key() == Qt::Key_Escape)
    {
        hide();
    }  /// CTRL+C 复制
    else if (e->key() == Qt::Key_C && e->modifiers() == Qt::ControlModifier)
    {
        slotCopyScreenShot();

    }  ///截图另存为(Ctrl+S)
    else if (e->key() == Qt::Key_S && e->modifiers() == Qt::ControlModifier)
    {
        slotSaveScreenShot();
    }
    else
    {
        e->ignore();  //忽略
    }
}