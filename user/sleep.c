#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int agrc, char *argv[])
{
    if (agrc < 2)
    {
        fprintf(2, "Usage: sleep numbers\n");
        exit(1);    // exit with failure
    }

    double sleep_time = atoi(argv[1]);

    sleep(sleep_time);

    exit(0);    // exit with success
}