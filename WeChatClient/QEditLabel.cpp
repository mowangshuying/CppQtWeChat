#include "QEditLabel.h"
#include <QDebug>
#include <QApplication>

QEditLabel::QEditLabel(QWidget* parent)
	: QWidget(parent) {

	initCtrls();
}

void QEditLabel::setText(const char* text)
{
	if (m_label) {
		m_label->setText(text);
	}
}

bool QEditLabel::eventFilter(QObject* obj, QEvent* e) {

	if (QEvent::Enter == e->type()) {
		m_StackLayout->setCurrentWidget(m_lineEdit);
		m_lineEdit->setText(m_label->text());
		//qDebug() << "1111 e->type() = " << e->type() << "obj = " << obj << endl;
	}

	if (QEvent::Leave == e->type()) {
		m_StackLayout->setCurrentWidget(m_label);
		m_label->setText(m_lineEdit->text());
		//m_label->setStyleSheet("border:0px;");
		//qDebug() << "2222 e->type() = " << e->type() << "obj = " << obj << endl;
	}

	return QWidget::eventFilter(obj, e);
}

void QEditLabel::initCtrls() {
	m_StackLayout = new QStackedLayout(this);
	setLayout(m_StackLayout);

	m_label = new QLabel("test");
	//m_label->setStyleSheet("background-color:white");

	m_lineEdit = new QLineEdit();
	m_label->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

	//m_stackWidget->setGeometry(0, 0, 50, 50);

	m_label->installEventFilter(this);
	m_lineEdit->installEventFilter(this);

	m_label->setStyleSheet("border:0px;");

	m_StackLayout->addWidget(m_label);
	m_StackLayout->addWidget(m_lineEdit);
	m_StackLayout->setCurrentWidget(m_label);
}