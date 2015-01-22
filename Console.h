#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QPlainTextEdit>
#include <QStyleOptionFrameV2>

class Console : public QTextEdit
{
    Q_OBJECT

    public:
        Console(QWidget *parent = NULL);
        void keyPressEvent(QKeyEvent *event);
        QSize sizeHint();
        QTextCursor *bashOutputEnd;
        int bashEndPos;

        void insertBashOutput(QString text);

    signals:
        void returnPressed();
};

#endif // TEXTEDIT_H
