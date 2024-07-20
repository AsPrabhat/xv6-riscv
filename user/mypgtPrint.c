#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int arrGlobal[10000];

int main(int argc, char *argv[])
{
    pgtPrint();
    exit(0);
}
