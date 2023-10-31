#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"

#define RD 0
#define WR 1

int main(int argc, char *argv[])
{
    int pipe1[2];
    int pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
    {
        fprintf(2, "pipe() error\n");
        exit(1);
    }

    switch (fork())
    {
        case -1:
            fprintf(2, "fork() error\n");
            close(pipe1[RD]);
            close(pipe1[WR]);
            close(pipe2[RD]);
            close(pipe2[WR]);
            exit(1);
        case 0:
            close(pipe1[WR]);    // child read from pipe1
            close(pipe2[RD]);    // child write to pipe2

            char data_child = 'C';

            if (read(pipe1[RD], &data_child, 1) != 1)
            {
                fprintf(2, "child read error\n");
                exit(1); 
            }
            else
            {
                fprintf(1, "%d: received ping\n", getpid());
            }
            if (write(pipe2[WR], &data_child, 1) != 1)
            {
                fprintf(2, "child write error\n");
                exit(1);
            }

            close(pipe1[RD]);
            close(pipe2[WR]);
            exit(0);
        default:
            close(pipe1[RD]);   // parent write to pipe1
            close(pipe2[WR]);   // parent read from pipe2

            char data_parent = 'P';

            if (write(pipe1[WR], &data_parent, 1) != 1)
            {
                fprintf(2, "parent write error\n");
                exit(1);
            }
            if (read(pipe2[RD], &data_parent, 1) != 1)
            {
                fprintf(2, "parent read error\n");
                exit(1);
            }
            else
            {
                fprintf(1, "%d: received pong\n", getpid());
            }

            close(pipe1[WR]);
            close(pipe2[RD]);
            wait(0);
    }
    exit(0);
}