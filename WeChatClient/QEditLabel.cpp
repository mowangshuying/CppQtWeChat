#include "QEditLabel.h"
#include <QDebug>
#include <QApplication>
#include <QVector>
#include <vector>
#include <QMap>
#include <map>

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
		//qDebug() << "QEvent::Enter..." << endl;
	}

	if (QEvent::Leave == e->type()) {
		m_StackLayout->setCurrentWidget(m_label);
		m_label->setText(m_lineEdit->text());
		//qDebug() << "QEvent::Leave..." << endl;
	}

	return QWidget::eventFilter(obj, e);
}

void QEditLabel::initCtrls() {
	m_StackLayout = new QStackedLayout(this);
	setLayout(m_StackLayout);

	m_label = new QLabel();
	m_lineEdit = new QLineEdit();
	//m_label->setFocusPolicy(Qt::FocusPolicy::ClickFocus); // 暂时先注释，并未发现异常

	m_label->installEventFilter(this);
	m_lineEdit->installEventFilter(this);

	m_label->setStyleSheet("border:0px;");

	m_StackLayout->addWidget(m_label);
	m_StackLayout->addWidget(m_lineEdit);
	m_StackLayout->setCurrentWidget(m_label);

	sizeof(QVector<int>);
	sizeof(std::vector<int>);
	sizeof(QMap<int, int>);
	sizeof(std::map<int,int>);
}