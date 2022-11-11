/****************************************************************************
 * mm/mm_gran/mm_gran.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#ifndef __MM_MM_GRAN_MM_GRAN_H
#define __MM_MM_GRAN_MM_GRAN_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "config.h"

#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Sizes of things */

#define SIZEOF_GAT(n) \
  ((n + 31) >> 5)
#define SIZEOF_MM_GRAN(n) \
  (sizeof(struct mm_gran) + sizeof(uint32_t) * (SIZEOF_GAT(n) - 1))

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* This structure represents the state of one granule allocation */

struct mm_gran
{
    uint8_t    log2gran;  /* Log base 2 of the size of one granule */
    uint16_t   ngranules; /* The total number of (aligned) granules in the heap */
    uintptr_t  heapstart; /* The aligned start of the granule heap */
    uint32_t   gat[1];    /* Start of the granule allocation table */
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: gran_enter_critical and gran_leave_critical
 *
 * Description:
 *   Critical section management for the granule allocator.
 *
 * Input Parameters:
 *   priv - Pointer to the gran state
 *
 * Returned Value:
 *   gran_enter_critical() may return any error reported by
 *   nxsem_wait_uninterruptible()
 *mm_gran/_s *priv);

/****************************************************************************
 * Name: gran_mark_allocated
 *
 * Description:
 *   Mark a range of granules as allocated.
 *
 * Input Parameters:
 *   priv  - The granule heap state structure.
 *   alloc - The address of the allocation.
 *   ngranules - The number of granules allocated
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void gran_mark_allocated(struct mm_gran *priv, uintptr_t alloc, unsigned int ngranules);

#endif /* __MM_MM_GRAN_MM_GRAN_H */
