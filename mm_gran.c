#include "config.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>

#include "gran.h"
#include "mm_gran.h"

#ifdef CONFIG_GRAN

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: gran_initialize
 *
 * Description:
 *   Set up one granule allocator instance.  Allocations will be aligned to
 *   the alignment size (log2align; allocations will be in units of the
 *   granule size (log2gran). Larger granules will give better performance
 *   and less overhead but more losses of memory due to quantization waste.
 *   Additional memory waste can occur from alignment; log2align should be
 *   set to 0 unless you are using the granule allocator to manage DMA
 *   or page-aligned memory and your hardware has specific memory alignment
 *   requirements.
 *
 *   General Usage Summary.  This is an example using the GCC section
 *   attribute to position a DMA heap in memory (logic in the linker script
 *   would assign the section .dmaheap to the DMA memory.
 *
 *   uint32_t g_dmaheap[DMAHEAP_SIZE] locate_data(.dmaheap);
 *
 *   The heap is created by calling gran_initialize().  Here the granule size
 *   is set to 64 bytes (2**6) and the alignment to 16 bytes (2**4):
 *
 *     GRAN_HANDLE handle = gran_initialize(g_dmaheap, DMAHEAP_SIZE, 6, 4);
 *
 *   Then the GRAN_HANDLE can be used to allocate memory:
 *
 *     uint8_t *dma_memory = (uint8_t *)gran_alloc(handle, 47);
 *
 *   The actual memory allocates will be 64 byte (wasting 17 bytes) and
 *   will be aligned at least to (1 << log2align).
 *
 *   NOTE: The current implementation also restricts the maximum allocation
 *   size to 32 granules.  That restriction could be eliminated with some
 *   additional coding effort.
 *
 * Input Parameters:
 *   heapstart - Start of the granule allocation heap
 *   heapsize  - Size of heap in bytes
 *   log2gran  - Log base 2 of the size of one granule.  0->1 byte,
 *               1->2 bytes, 2->4 bytes, 3->8 bytes, etc.
 *   log2align - Log base 2 of required alignment.  0->1 byte,
 *               1->2 bytes, 2->4 bytes, 3->8 bytes, etc.  Note that
 *               log2gran must be greater than or equal to log2align
 *               so that all contiguous granules in memory will meet
 *               the minimum alignment requirement. A value of zero
 *               would mean that no alignment is required.
 *
 * Returned Value:
 *   On success, a non-NULL handle is returned that may be used with other
 *   granule allocator interfaces.
 *
 ****************************************************************************/

struct mm_gran *gran_initialize(void *heapstart, size_t heapsize, uint8_t log2gran, uint8_t log2align)
{
    struct mm_gran    *gran;
    uintptr_t          heapend;
    uintptr_t          alignedstart;
    unsigned long      mask;
    unsigned long      alignedsize;
    unsigned int       ngranules;

    /* 
     * Check parameters if debug is on.  Note the size of a granule is
     * limited to 2**31 bytes and that the size of the granule must be greater
     * than or equal to the alignment size.
     */
    assert(heapstart && heapsize > 0 && log2gran > 0 && log2gran < 32 && log2gran >= log2align);

    /* Get the aligned start of the heap */
    mask         = (1 << log2align) - 1;
    alignedstart = ((uintptr_t)heapstart + mask) & ~mask;

    /* Determine the number of granules */
    mask         = (1 << log2gran) - 1;
    heapend      = (uintptr_t)heapstart + heapsize;
    alignedsize  = (heapend - alignedstart) & ~mask;
    ngranules    = alignedsize >> log2gran;

    /*
     * Allocate the information structure with a granule table of the
     * correct size.
     */
#if 0
    gran = (struct mm_gran *)malloc(SIZEOF_MM_GRAN(ngranules));
#else
    gran = (struct mm_gran *)heapstart;

    unsigned int gran_head = SIZEOF_MM_GRAN(ngranules);
    heapstart = (char *)heapstart + gran_head;
    heapsize = heapsize - gran_head;

    /* Get the aligned start of the heap */
    mask         = (1 << log2align) - 1;
    alignedstart = ((uintptr_t)heapstart + mask) & ~mask;    

    /* Determine the number of granules */
    mask         = (1 << log2gran) - 1;
    heapend      = (uintptr_t)heapstart + heapsize;
    alignedsize  = (heapend - alignedstart) & ~mask;
    ngranules    = alignedsize >> log2gran;
#endif

    if (gran)
    {
        /* Initialize non-zero elements of the granules heap info structure */
        gran->log2gran  = log2gran;
        gran->ngranules = ngranules;
        gran->heapstart = alignedstart;
    }

    return gran;
}

/****************************************************************************
 * Name: gran_release
 *
 * Description:
 *   Uninitialize a gram memory allocator and release resources held by the
 *   allocator.
 *
 * Input Parameters:
 *   handle - The handle previously returned by gran_initialize
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

void gran_release(struct mm_gran *gran)
{
    assert(gran != NULL);

    free(gran);
}

#endif /* CONFIG_GRAN */
