#include "kernel/param.h"
#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "kernel/riscv.h"
#include "user.h"

// Global variable declaration
int temp;

int main()
{
  char *arr = malloc(32 * PGSIZE);
  pgtPrint();

  // Initialize a bitmap variable to keep track of page access and dirtiness
  uint64 bitmap = 0;

  // Access the first, second, third, and thirtieth pages of the array 'arr'
  arr[PGSIZE * 0] += 1;
  arr[PGSIZE * 1] += 1;
  arr[PGSIZE * 2] += 1;
  arr[PGSIZE * 30] += 1;

  printf("arr[PGSIZE*6]: %d\n", arr[PGSIZE * 6]);
  temp=arr[PGSIZE * 19];
  pgtPrint();

  // Reset the bitmap variable
  bitmap = 0;

  pgaccess(arr, 32, &bitmap);
  for (int i = 0; i < 32; i++)
  {
    if ((bitmap & ((uint64)1 << (2 * i)))&&(bitmap & ((uint64)1 << (2 * i + 1))))
      printf("Page %d is accessed and dirty\n", i);
    else if (bitmap & ((uint64)1 << (2 * i)))
      printf("Page %d is accessed\n", i);
    else printf("Page %d is not accessed\n", i);
  }

  free(arr);
  return 0;
}