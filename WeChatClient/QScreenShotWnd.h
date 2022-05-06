#pragma once

//���ñ����ʽΪutf-8
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>
#include <QMenu>

class QScreenShotWnd : public QWidget {
	Q_OBJECT
public:
	QScreenShotWnd(QWidget* p = nullptr);
public slots:
	//���ƽ�ͼ
	void slot_copyScreenShot();
	//�����ͼ
	void slot_saveScreenShot();
	//�˳���ͼ
	void slot_exitSccreenShot();
protected:
	void showEvent(QShowEvent*);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void paintEvent(QPaintEvent* event);

	void contextMenuEvent(QContextMenuEvent*);  //--�Ҽ��˵��¼�
	void keyPressEvent(QKeyEvent* e);      //--�����¼�
public:
	QPoint getBeginPos();
	QPoint getEndPos();
	void setBeginPos(QPoint p);
	void setEndPos(QPoint p);
public:
	QPixmap m_fullScreenPixmap;
private:
	//��꿪ʼλ��
	QPoint m_beginPos;
	//������λ��
	QPoint m_endPos;
	//���ν�ͼ����
	QRect  m_screenShotRect;
	//����Ƿ���
	bool m_leftBtnPress;

	//�Ҽ��˵�
	QMenu* m_RightBtnMenu;
};

