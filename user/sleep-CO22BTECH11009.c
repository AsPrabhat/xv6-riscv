#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(2, "Usage: sleep <ticks>\n");
        exit(1);
    }

    int ticks = atoi(argv[1]); // Convert the argument to integer

    if (ticks <= 0)
    {
        fprintf(2, "Invalid number of ticks\n");
        exit(2);
    }

    sleep(ticks); // Call the xv6 system call sleep

    exit(0);
}