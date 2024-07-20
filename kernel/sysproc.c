#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

#define NPTENTRIES 512

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;
  argint(0, &n);
  addr = myproc()->sz;
  myproc()->sz += n;
  if (n < 0)
    uvmdealloc(myproc()->pagetable, addr, myproc()->sz);
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

void
pgtPrint(pagetable_t pagetable, int level)
{
  if(level >= 3)
    return;
  for(int i = 0; i < 512; i++) {
    pte_t pte = pagetable[i];

    if (pte & PTE_V) {
      if (pte & PTE_U)
      {
        uint64 vaddr = PGROUNDDOWN(i * PGSIZE);
        printf("PTE No: %d, Virtual page address: 0x%x, Physical page address: 0x%x\n", i, vaddr, PTE2PA(pte));
      }
      
    }
    if ((pte & PTE_V) && (pte & (PTE_R|PTE_W|PTE_W)) == 0) {
        // this PTE points to a lower-level page table.
    uint64 child = PTE2PA(pte);
    pgtPrint((pagetable_t)child, level+1);
    }
  }
}

uint64
sys_pgtPrint(void)
{
    pgtPrint(myproc()->pagetable, 0);
    return 0;
}

uint64 
sys_pgaccess(void *, int, uint64)
{
  uint64 start_address;
  int num_pages;
  uint64 user_address;
  argaddr(0, &start_address);
  argint(1, &num_pages);
  argaddr(2, &user_address);

  uint64 access_mask = 0;
  uint64 complement_access = ~PTE_A;
  struct proc *process = myproc();

  for (int i = 0; i < num_pages; ++i)
  {
    pte_t *page_table_entry = walk(process->pagetable, start_address + i * PGSIZE, 0);

    if (*page_table_entry & PTE_A)
    {
      access_mask |= ((uint64)1 << (i * 2));
      *page_table_entry &= complement_access;
    }
    if (*page_table_entry & PTE_A)
    {
      access_mask |= ((uint64)1 << (i * 2 + 1));
    }
  }
  printf("Access mask calculated: %p\n", access_mask);
  copyout(process->pagetable, user_address, (char *)&access_mask, sizeof(access_mask));
  return 0;
}