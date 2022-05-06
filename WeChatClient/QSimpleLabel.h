#pragma once

//设置编码格式为utf-8
#pragma execution_character_set("utf-8")

#include <QLabel>

class QSimpleLabel : public QLabel
{
	Q_OBJECT
public:
	explicit QSimpleLabel(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	explicit QSimpleLabel(const QString& text, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
signals:
	void clicked();
protected:
	void mousePressEvent(QMouseEvent* ev);
};

