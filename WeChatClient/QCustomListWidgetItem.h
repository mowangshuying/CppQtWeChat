#pragma once

//���ñ����ʽΪutf-8
#pragma execution_character_set("utf-8")

#include <QListWidget>

class QCustomListWidgetItem : public QListWidgetItem
{
public:
	explicit QCustomListWidgetItem(QListWidget* view = nullptr, int type = Type);
	int sesId() const;
	void setSesId(int sesid);
private:
	//���һЩ��������ݣ���Ựid��
	int m_sesId;
};