#include "kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1

void primes(int *p_parent)
{
    close(p_parent[WR]);
    
    int p_child[2];
    if (pipe(p_child) == -1)
    {
        fprintf(2, "child: pipe() error!\n");
        exit(-1);
    }
    

    int current_number = 0;
    if (read(p_parent[RD], &current_number, sizeof(int)) == 0)
    {
        exit(0);
    }
    

    if (fork() == 0)
    {
        primes(p_child);
    }
    else
    {
        close(p_child[RD]);
        printf("prime %d\n", current_number);
        int prime = current_number;
        while (read(p_parent[RD], &prime, sizeof(int)) != 0)
        {
            if (prime % current_number != 0)
            {
                write(p_child[WR], &prime, sizeof(int));
            }
        }
        close(p_child[WR]);
        wait(0);
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    int p[2];
    if (pipe(p) == -1)
    {
        fprintf(2, "pipe() error!\n");
        exit(-1);
    }

    switch (fork())
    {
        case -1:
            fprintf(2, "fork() error!\n");
            exit(-1);
        case 0:
            // child
            primes(p);
        default:
            // parent
            close(p[RD]);
            for (int i = 2; i <= 35; i++)
            {
                write(p[WR], &i, sizeof(int));
            }
            close(p[WR]);
            wait(0);
    }

    exit(0);
}