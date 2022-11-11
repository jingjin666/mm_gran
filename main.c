#include <stdio.h>

#include "mm_gran.h"
#include "gran.h"

#define PAGE_SIZE   4096
#define PAGE_SHIFT  12
#define HEAP_SIZE   1024*1024

char __attribute__ ((aligned(PAGE_SIZE))) heap[HEAP_SIZE];

struct graninfo g_graninfo;
struct mm_gran *g_gran;

int main()
{
	printf("heap base = %p\n", heap);

    g_gran = gran_initialize(heap, HEAP_SIZE, PAGE_SHIFT, PAGE_SHIFT);

    void *addr1 ;
    void *addr2 ;
    void *addr3 ;
    void *addr4 ;

    addr1 = gran_alloc(g_gran, 1024);
    printf("addr1 = %p\n", addr1);
    addr2 = gran_alloc(g_gran, 1024);
    printf("addr2 = %p\n", addr2);
    addr3 = gran_alloc(g_gran, 1024);
    printf("addr3 = %p\n", addr3);
    addr4 = gran_alloc(g_gran, 1024);
    printf("addr4 = %p\n", addr4);

    gran_free(g_gran, addr1, 1024);
    gran_free(g_gran, addr2, 1024);

    gran_info(g_gran, &g_graninfo);
    printf("total page = %d, free page = %d, mx free page = %d\n", g_graninfo.ngranules, g_graninfo.nfree, g_graninfo.mxfree);

	printf("end\n");
	return 0;
}