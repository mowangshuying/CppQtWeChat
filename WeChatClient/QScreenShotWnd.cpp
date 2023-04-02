#include "QScreenShotWnd.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QPen>
#include <QFileDialog>
#include <QGuiApplication>
#include <QClipboard>

QScreenShotWnd::QScreenShotWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    m_beginPos = QPoint(-1, -1);
    m_endPos = QPoint(-1, -1);
    m_leftBtnPress = false;
    // m_bFirst = true;
    setMouseTracking(true);  //开启鼠标实时追踪，实时的显示鼠标的位置
    m_screenShotRect = QRect(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height());

    m_RightBtnMenu = new QMenu(this);
    m_RightBtnMenu->addAction("复制(CTRL+C)", this, SLOT(slot_copyScreenShot()));
    m_RightBtnMenu->addAction("保存(Ctrl+S)", this, SLOT(slot_saveScreenShot()));
    m_RightBtnMenu->addAction("退出(ESC)", this, SLOT(slot_exitSccreenShot()));
}

void QScreenShotWnd::slot_copyScreenShot()
{
    QGuiApplication::clipboard()->setPixmap(m_fullScreenPixmap.copy(m_screenShotRect));
    this->close();
}

void QScreenShotWnd::slot_saveScreenShot()
{
    QString fileName = QFileDialog::getSaveFileName(this, "截图另存为", "test.png", "Image (*.jpg *.png *.bmp)");

    if (fileName.length() > 0)
    {
        m_fullScreenPixmap.copy(m_screenShotRect).save(fileName, "png");
        this->close();
    }
}

void QScreenShotWnd::slot_exitSccreenShot()
{
    this->close();
}

void QScreenShotWnd::showEvent(QShowEvent* showEvent)
{
    setWindowOpacity(0.2);
}

void QScreenShotWnd::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_leftBtnPress = true;
        setBeginPos(event->pos());

        //按下鼠标时候设置窗口的背景颜色
        // setStyleSheet("background-color:black;");
    }
}

void QScreenShotWnd::mouseMoveEvent(QMouseEvent* event)
{
    if (m_leftBtnPress)
    {
        setEndPos(event->pos());
        update();
    }
}

void QScreenShotWnd::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_leftBtnPress = false;
        setEndPos(event->pos());

        if (m_beginPos.x() > m_endPos.x())
        {
            m_beginPos.setX(m_beginPos.x() + m_endPos.x());
            m_endPos.setX(m_beginPos.x() - m_endPos.x());
            m_beginPos.setX(m_beginPos.x() - m_endPos.x());
        }
        if (m_beginPos.y() > m_endPos.y())
        {
            m_beginPos.setY(m_beginPos.y() + m_endPos.y());
            m_endPos.setY(m_beginPos.y() - m_endPos.y());
            m_beginPos.setY(m_beginPos.y() - m_endPos.y());
        }

        // 设置屏幕截图的范围
        m_screenShotRect.setRect(m_beginPos.x(), m_beginPos.y(), m_endPos.x() - m_beginPos.x(), m_endPos.y() - m_beginPos.y());
    }
}

void QScreenShotWnd::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);  //将当前窗体对象设置为画布
    QPen pen;
    pen.setColor(Qt::blue);  //设置笔色
    pen.setWidth(2);         //画笔线条宽度
    painter.setPen(pen);     //设置画笔

    int lx = m_beginPos.x() < m_endPos.x() ? m_beginPos.x() : m_endPos.x();                                 //矩形截图区域左上角x坐标
    int ly = m_beginPos.y() < m_endPos.y() ? m_beginPos.y() : m_endPos.y();                                 //矩形截图区域右上角x坐标
    int w = m_beginPos.x() < m_endPos.x() ? m_endPos.x() - m_beginPos.x() : m_beginPos.x() - m_endPos.x();  //矩形截图区域宽度
    int h = m_beginPos.y() < m_endPos.y() ? m_endPos.y() - m_beginPos.y() : m_beginPos.y() - m_endPos.y();  //矩形截图区域高度

    //矩形截图区域
    QRect rect = QRect(lx, ly, w, h);  //矩形截图区域
    //防止第一次就重绘 并且宽高大于0时才进行截图操作
    if (lx != -1 && w > 0 && h > 0)
    {
        //重绘截图矩形部分，即恢复原图，达到去除幕布效果
        painter.drawPixmap(rect, m_fullScreenPixmap, rect);
        //画截图矩形
        painter.drawRect(lx, ly, w, h);

        //截图区域大小位置提示
        if (ly > 10)
        {
            //避免看不到提示,在截图矩形上边不接近屏幕上边时，提示在截图矩形的上边的上面
            painter.drawText(lx + 2, ly - 8, tr("截图范围(%1,%2) - (%3,%4)  截图大小：(%5 x %6)").arg(lx).arg(ly).arg(lx + w).arg(ly + h).arg(w).arg(h));
        }
        else
        {
            //在截图矩形上边接近屏幕上边时，提示在截图矩形的上边的下面
            painter.drawText(lx + 2, ly + 12, tr("截图范围(%1,%2) - (%3,%4)  截图大小：(%5 x %6)").arg(lx).arg(ly).arg(lx + w).arg(ly + h).arg(w).arg(h));
        }
    }

    //实时显示鼠标的位置
    // painter.drawText(cursor().pos().x(), cursor().pos().y(), tr("(%1,%2)").arg(cursor().pos().x()).arg(cursor().pos().y()));
}

void QScreenShotWnd::contextMenuEvent(QContextMenuEvent*)
{
    this->setCursor(Qt::ArrowCursor);      //设置鼠标的样式为箭头
    m_RightBtnMenu->exec(cursor().pos());  //菜单显示的位置跟随鼠标
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
        slot_copyScreenShot();

    }  ///截图另存为(Ctrl+S)
    else if (e->key() == Qt::Key_S && e->modifiers() == Qt::ControlModifier)
    {
        slot_saveScreenShot();
    }
    else
    {
        e->ignore();  //忽略
    }
}

QPoint QScreenShotWnd::getBeginPos()
{
    return m_beginPos;
}

QPoint QScreenShotWnd::getEndPos()
{
    return m_endPos;
}

void QScreenShotWnd::setBeginPos(QPoint p)
{
    m_beginPos = p;
}

void QScreenShotWnd::setEndPos(QPoint p)
{
    m_endPos = p;
}
