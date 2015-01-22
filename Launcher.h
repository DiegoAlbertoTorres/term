#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QDialog>
#include <QLayout>
#include <QDebug>
#include <QSocketNotifier>
#include <QTextCursor>
#include <QTextDocumentFragment>

#include <unistd.h>

#include "Console.h"

class Launcher : public QDialog
{
    Q_OBJECT

public:
    Launcher(int bashInputFD, int bashOutputFD, QWidget *parent = 0);
    ~Launcher();

public slots:
    void printBashStdout(int bashStdout);
    void consoleToBash();

private:
    int bashpid;

    int bashIn;
    int bashOut;

    bool skipNextLine;

    QSocketNotifier *bashOutputNotifier;

    QVBoxLayout *layout;
    Console *cons;
};

#endif // LAUNCHER_H
