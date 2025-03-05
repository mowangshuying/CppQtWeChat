#pragma once
#include <QTextEdit>
#include <QMimeData>

class SelfTextEdit : public QTextEdit
{
  public:
    SelfTextEdit(QWidget *parent = nullptr);

    void insertFromMimeData(const QMimeData *source);
};
