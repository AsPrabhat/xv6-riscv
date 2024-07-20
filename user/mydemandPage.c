#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define N 3000 // global array size - change to see effect. Try 3000, 5000, 10000

int arrGlobal[N];

int main()
{
    arrGlobal[0] = 2; // initialize with any integer value

    fprintf(1, "global addr from user space: %x\n", arrGlobal);

    for (int i = 1; i < N; i++)
    {
        arrGlobal[i] = arrGlobal[i - 1];
        if (i % 1000 == 0)
            pgtPrint();
    }

    fprintf(1, "Printing final page table:\n");
    pgtPrint();
    fprintf(1, "Value: %d\n", arrGlobal[N - 1]);
    exit(0);
}
