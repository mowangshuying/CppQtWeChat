#include "QScreenShotWnd.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QPen>
#include <QFileDialog>
#include <QGuiApplication>
#include <QClipboard>

QScreenShotWnd::QScreenShotWnd(QWidget* p /*= nullptr*/)
	: QWidget(p)
{
	m_beginPos = QPoint(-1, -1);
	m_endPos = QPoint(-1, -1);
	m_leftBtnPress = false;
	setMouseTracking(true);//�������ʵʱ׷�٣�ʵʱ����ʾ����λ��
	m_screenShotRect = QRect(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height());

	m_RightBtnMenu = new QMenu(this);
	m_RightBtnMenu->addAction("����(CTRL+C)", this, SLOT(slot_copyScreenShot()));
	m_RightBtnMenu->addAction("����(Ctrl+S)", this, SLOT(slot_saveScreenShot()));
	m_RightBtnMenu->addAction("�˳�(ESC)", this, SLOT(slot_exitSccreenShot()));

}

void QScreenShotWnd::slot_copyScreenShot()
{
	QGuiApplication::clipboard()->setPixmap(m_fullScreenPixmap.copy(m_screenShotRect));
	this->close();
}

void QScreenShotWnd::slot_saveScreenShot()
{
	QString fileName = QFileDialog::getSaveFileName(this, "��ͼ���Ϊ", "test.png", "Image (*.jpg *.png *.bmp)");

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
	setWindowOpacity(0.7);
}

void QScreenShotWnd::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_leftBtnPress = true;
		setBeginPos(event->pos());
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
		m_screenShotRect.setRect(m_beginPos.x(), m_beginPos.y(), m_endPos.x() - m_beginPos.x(), m_endPos.y() - m_beginPos.y());
	}
}

void QScreenShotWnd::paintEvent(QPaintEvent* event)
{
	QPainter painter(this); //����ǰ�����������Ϊ����
	QPen pen;
	pen.setColor(Qt::red);//���ñ�ɫ
	pen.setWidth(4);     //�����������
	painter.setPen(pen);//���û���

	int lx = m_beginPos.x() < m_endPos.x() ? m_beginPos.x() : m_endPos.x();//���ν�ͼ�������Ͻ�x����
	int ly = m_beginPos.y() < m_endPos.y() ? m_beginPos.y() : m_endPos.y();//���ν�ͼ�������Ͻ�x����
	int w = m_beginPos.x() < m_endPos.x() ? m_endPos.x() - m_beginPos.x() : m_beginPos.x() - m_endPos.x();//���ν�ͼ������
	int h = m_beginPos.y() < m_endPos.y() ? m_endPos.y() - m_beginPos.y() : m_beginPos.y() - m_endPos.y();//���ν�ͼ����߶�

	QRect rect = QRect(lx, ly, w, h);//���ν�ͼ����
	if (lx != -1 && w > 0 && h > 0)//��ֹ��һ�ξ��ػ� ���ҿ�ߴ���0ʱ�Ž��н�ͼ����
	{

		painter.drawPixmap(rect, m_fullScreenPixmap, rect);//�ػ��ͼ���β��֣����ָ�ԭͼ���ﵽȥ��Ļ��Ч��
		painter.drawRect(lx, ly, w, h);//����ͼ����
		//��ͼ�����Сλ����ʾ
		if (ly > 10)//���⿴������ʾ,�ڽ�ͼ�����ϱ߲��ӽ���Ļ�ϱ�ʱ����ʾ�ڽ�ͼ���ε��ϱߵ�����
		{
			painter.drawText(lx + 2, ly - 8, tr("��ͼ��Χ(%1,%2) - (%3,%4)  ��ͼ��С��(%5 x %6)").arg(lx).arg(ly).arg(lx + w).arg(ly + h).arg(w).arg(h));
		}
		else//�ڽ�ͼ�����ϱ߽ӽ���Ļ�ϱ�ʱ����ʾ�ڽ�ͼ���ε��ϱߵ�����
		{
			painter.drawText(lx + 2, ly + 12, tr("��ͼ��Χ(%1,%2) - (%3,%4)  ��ͼ��С��(%5 x %6)").arg(lx).arg(ly).arg(lx + w).arg(ly + h).arg(w).arg(h));
		}
	}

	//ʵʱ��ʾ����λ��
	//painter.drawText(cursor().pos().x(), cursor().pos().y(), tr("(%1,%2)").arg(cursor().pos().x()).arg(cursor().pos().y()));

}

void QScreenShotWnd::contextMenuEvent(QContextMenuEvent*)
{
	this->setCursor(Qt::ArrowCursor);//����������ʽΪ��ͷ
	m_RightBtnMenu->exec(cursor().pos());//�˵���ʾ��λ�ø������
}

void QScreenShotWnd::keyPressEvent(QKeyEvent* e)
{
	/// Esc ���˳���ͼ;
	if (e->key() == Qt::Key_Escape)
	{
		hide();
	}///CTRL+C ����
	else if (e->key() == Qt::Key_C && e->modifiers() == Qt::ControlModifier)
	{
		slot_copyScreenShot();

	}///��ͼ���Ϊ(Ctrl+S)
	else if (e->key() == Qt::Key_S && e->modifiers() == Qt::ControlModifier)
	{
		slot_saveScreenShot();
	}
	else
	{
		e->ignore();//����
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

