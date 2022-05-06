#pragma once

//设置编码格式为utf-8
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QPushButton>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class QNextWnd : public QWidget
{
	Q_OBJECT
public:
	QNextWnd(QWidget* p = nullptr,int64_t friendid = -1,QString username = "");
	
	enum PushBtn_State {
		Ps_Next = 0,
		Ps_Close = 1
	};

public:
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

public slots:
	void closeWnd();
	void minWnd();
	void slot_pushBtnClick();
public:
	int m_index = 0;

	QLabel* m_titleLabel;
	QPushButton* m_minBtn;
	QPushButton* m_closeBtn;

	QWidget* m_wnd1;
	QWidget* m_wnd2;

	QPushButton* m_pushBtn;
	QStackedLayout* m_sLayout;
	QVBoxLayout* m_vLayout;

	QHBoxLayout* m_hLayout1;
	QHBoxLayout* m_hLayout2;

	bool m_bPress = false;
	QPoint m_poPress;

	PushBtn_State m_state;

	int64_t m_friendid;
	QString m_username;
};

