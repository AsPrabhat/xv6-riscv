# xv6-riscv Projects

### 1. Task 1: Printing the page table entries
In this task, I have modified the xv6 kernel to implement a new system call named pgtPrint() which will print the page table entries for the current process. Since the total number of page table entries can be very large, the system call will print the entries only if it is valid and the page is allowed access in user mode.  

I also implement a user program (filename as mypgtPrint.c) to invoke the newly added system call. Typing mypgtPrint on xv6 shell prints the page table of given user-level program.

### 2. Task 2: Implement demand paging
The base implementation of xv6 does not implement demand paging and the task is to implement demand paging. I have implemented a simpler version of demand paging where the read-only code associated with the process is mapped during the process creation, but the memory required for heap and globals is not assigned pages during process creation but allocated on demand. Also, the demand paging would be simpler to assume that sufficient memory is available and we do not need to replace/evict any page during the demand paging process.

I have implemented a user program (mydemandPage.c) to exercise this condition and see that it works. The user program uses a large sized global array write/read from this array to check for functioning of demand paging. Also, pgtPrint system call (implemented in Task-1) can be invoked intermittently to check that the page table is expanding as per demand paging.

### 3. task 3: Implement logic to detect which pages have been accessed and/or dirty
In this task, the code base of Task-2 is extended and added a new feature to xv6 that detects and reports which pages have been accessed and/or modified (dirty) to userspace by inspecting the access and modified bits in the RISC-V page table. The RISC-V hardware page walker marks these bits in the PTE whenever it resolves a TLB miss.

A new system call pgaccess() is implemented, that reports which pages have been accessed and/or dirty. The system call takes three arguments. First, it takes the starting virtual address of the first user page to check. Second, it takes the number of pages to check. Finally, it takes a user address to a buffer to store the results into a bitmask/bitmap (a data structure that uses two bits per page and where the first page corresponds to the least significant bit).
