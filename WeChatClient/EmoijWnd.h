#pragma once

#include "QPch.h"
#include <QWidget>
#include <QEvent>
#include <QVBoxLayout>
#include <QTableWidget>

class EmoijWnd : public QWidget
{
    Q_OBJECT
  public:
    EmoijWnd(QWidget* p = nullptr);

  protected:
    bool event(QEvent* event);

    void paintEvent(QPaintEvent* paintEvent);
  signals:
    void signalEmoijClicked(QString m_emoijString);
  public slots:
    // 判断哪个单元格子被点击
    void slotCellClicked(int x, int y);

  public:
    QStringList m_emoijStrList;
    QTableWidget* m_centerWnd;
    QVBoxLayout* m_vLayout;
};
