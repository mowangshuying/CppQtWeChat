#include "QSelfTextEdit.h"
#include <QApplication>
#include <QClipboard>
#include <QTextCursor>
#include <QImage>

QSelfTextEdit::QSelfTextEdit(QWidget *parent /*= nullptr*/) : QTextEdit(parent)
{
    setAcceptRichText(true);
}

void QSelfTextEdit::insertFromMimeData(const QMimeData *source)
{
    if (source->hasImage())
    {
        const QMimeData *mime = QApplication::clipboard()->mimeData();
        if (mime->hasImage())
        {
            QImage image = qvariant_cast<QImage>(mime->imageData());
            textCursor().insertImage(image);
            return;
        }
    }

    QTextEdit::insertFromMimeData(source);
}
