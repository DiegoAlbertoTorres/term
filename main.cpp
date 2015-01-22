#include "Launcher.h"
#include <QApplication>

#include <QDebug>

#include <pty.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

# define BUFFER_SIZE    1024
#define MAX(a, b) (a > b ? a : b)

int initBash(int *input, int *output){
    int pid;
    int master;

    if ((pid = forkpty(&master, NULL, NULL, NULL)) == 0){
        chdir(getenv("HOME"));
        putenv("TERM=dumb");

        close(output[0]);
        // Set stdout to output pipe write end
        dup2(output[1], STDOUT_FILENO);
        // Set stderr to output pipe write end
        dup2(output[1], STDERR_FILENO);

        close(input[1]);
        // Set stdin to input pipe read end
        dup2(input[0], STDIN_FILENO);

        execl("/bin/bash", "/bin/bash", "-li", NULL);
        // Should not return
        return -1;
    }
    // Parent
    else if (pid > 0){
        close(output[1]);
        close(input[0]);

        return pid;
    }
    // Error
    else{
        qDebug() << "Fork error";
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    // Initialize bash
    int input[2];
    int output[2];
    pipe(input);
    pipe(output);
    if (initBash(input, output) < 0)
        qDebug() << "Main: Bash initialization failed.";

    qDebug() << "Bash stdout is:" << output[0];
    QApplication a(argc, argv);
    Launcher lcher(input[1], output[0]);
    lcher.show();

    return a.exec();
}
