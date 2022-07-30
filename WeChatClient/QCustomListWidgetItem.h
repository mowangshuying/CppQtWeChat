#pragma once

//设置编码格式为utf-8
#pragma execution_character_set("utf-8")

#include <QListWidget>

class QCustomListWidgetItem : public QListWidgetItem
{
public:
	explicit QCustomListWidgetItem(QListWidget* view = nullptr, int type = Type);
	int sesId() const;
	void setSesId(int sesid);
private:
	//添加一些额外的数据，如会话id；
	//设置默认值为-1
	int m_sesId = -1;
};