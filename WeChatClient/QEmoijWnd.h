#pragma once

#include "QPch.h"
#include <QWidget>
#include <QEvent>
#include <QVBoxLayout>
#include <QTableWidget>

class QEmoijWnd : public QWidget
{
    Q_OBJECT
  public:
    QEmoijWnd(QWidget* p = nullptr);

  protected:
    bool event(QEvent* event);

    void paintEvent(QPaintEvent* paintEvent);
  signals:
    void signalEmoijClicked(QString m_emoijString);
  public slots:
    //�ж��ĸ���Ԫ���ӱ����
    void slotCellClicked(int x, int y);

  public:
    QStringList m_emoijStrList;
    QTableWidget* m_centerWnd;
    QVBoxLayout* m_vLayout;
};
