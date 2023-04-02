#pragma once


#include "QPch.h"

#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>
#include <QMenu>

class QScreenShotWnd : public QWidget {
	Q_OBJECT
public:
	QScreenShotWnd(QWidget* p = nullptr);
public slots:
	//复制截图
	void slot_copyScreenShot();
	//保存截图
	void slot_saveScreenShot();
	//退出截图
	void slot_exitSccreenShot();
protected:
	void showEvent(QShowEvent*);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void paintEvent(QPaintEvent* event);

	void contextMenuEvent(QContextMenuEvent*);  //--右键菜单事件
	void keyPressEvent(QKeyEvent* e);      //--按键事件
public:
	QPoint getBeginPos();
	QPoint getEndPos();
	void setBeginPos(QPoint p);
	void setEndPos(QPoint p);
public:
	QPixmap m_fullScreenPixmap;
private:
	// 是否是第一次截屏
	//bool m_bFirst;

	//鼠标开始位置
	QPoint m_beginPos;
	//鼠标结束位置
	QPoint m_endPos;
	//矩形截图区域
	QRect  m_screenShotRect;
	//左键是否按下
	bool m_leftBtnPress;

	//右键菜单
	QMenu* m_RightBtnMenu;

	// 构建截屏工具条

};

