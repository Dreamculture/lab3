#include "param.h"
#include "types.h"
#include "memlayout.h"
#include "elf.h"
#include "riscv.h"
#include "defs.h"
#include "fs.h"

// Copy from user to kernel.
// Copy len bytes to dst from virtual address srcva in a given page table.
// Return 0 on success, -1 on error.
void
copyin_new(pagetable_t pagetable, pagetable_t kpagetable , pte_t va ,int counter)
{  
  //printf("the counter is %d",counter);
  counter ++;
      // there are 2^9 = 512 PTEs in a page table.
  for(int i = 0; i < 512; i++){
    pte_t pte = pagetable[i];

    if((pte & PTE_V) && (pte & (PTE_R|PTE_W|PTE_X)) == 0){
      // this PTE points to a lower-level page table.
      uint64 child = PTE2PA(pte);
      va = va << 9;
      va = va + i;
      if(va > PLIC)
        return;
      copyin_new((pagetable_t)child, kpagetable ,va ,counter);

      //pagetable[i] = 0;
    } else if(pte & PTE_V){
        uint64 pa = PTE2PA(pte);
        int perm = pte & ((1 << 10)-1);
        perm = perm & ~PTE_U;
        mappages(kpagetable, va, PGSIZE, pa, perm);
        //panic("freewalk: leaf");
    }
  }
  //kfree((void*)pagetable);
    /*
  uint64 n, va0, pa0;

  while(len > 0){
    va0 = PGROUNDDOWN(srcva);
    pa0 = walkaddr(pagetable, va0);
    if(pa0 == 0)
      return -1;
    n = PGSIZE - (srcva - va0);
    if(n > len)
      n = len;
    memmove(dst, (void *)(pa0 + (srcva - va0)), n);

    len -= n;
    dst += n;
    srcva = va0 + PGSIZE;
  }
  return 0;
  */
}

