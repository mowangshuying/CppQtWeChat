#pragma once


#include "QPch.h"

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

