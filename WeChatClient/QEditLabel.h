#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QEvent>
#include <QObject>
#include <QStackedLayout>

class QEditLabel : public QWidget {
	
	Q_OBJECT
public:
	QEditLabel(QWidget* parent = nullptr);

	void setText(const char* text);

protected:
	bool eventFilter(QObject* obj, QEvent* e);
private:
	QStackedLayout* m_StackLayout;
	QLabel* m_label;
	QLineEdit* m_lineEdit;
private:
	void initCtrls();
};