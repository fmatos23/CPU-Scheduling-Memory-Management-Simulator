/*
 *  \author Diogo Carvalho - 113221
 */

#include "somm24.h"

#include <stdint.h>

namespace group 
{

// ================================================================================== //

    void memAddNodeToFreeList(MemNode *p)
    {
        soProbe(407, "%s(%p)\n", __func__, p);

        require(memAllocationPolicy != UndefMemoryAllocationPolicy, "Module is not in a valid open state!");
        require(memFreeList != UNDEF_MEM_NODE and memOccupiedList != UNDEF_MEM_NODE, "Module is not in a valid open state!");
        require(p != nullptr, "p must be a valid pointer");

        MemNode *previous = NULL;
        MemNode *current = memFreeList;

        // insert p in ascending order by 'start'
        while (current != NULL && current->block.start <= p->block.start)
        {
            previous = current;
            current = current->next;
        }
        p->next = current;
        if (previous != NULL)
            previous->next = p;
        else
            memFreeList = p;

        // merge forward
        if (current != NULL && (p->block.start + p->block.size == current->block.start))
        {
            p->block.size += current->block.size;
            p->next = current->next;
            free(current);
        }

        // merge backward
        if (previous != NULL && (previous->block.start + previous->block.size == p->block.start))
        {
            previous->block.size += p->block.size;
            previous->next = p->next;
            free(p);
            p = previous;
        }
    }

// ================================================================================== //

    void memAddNodeToOccupiedList(MemNode *p)
    {
        soProbe(408, "%s(%p)\n", __func__, p);

        require(memAllocationPolicy != UndefMemoryAllocationPolicy, "Module is not in a valid open state!");
        require(memFreeList != UNDEF_MEM_NODE and memOccupiedList != UNDEF_MEM_NODE, "Module is not in a valid open state!");
        require(p != nullptr, "p must be a valid pointer");

        MemNode *previous = NULL;
        MemNode *current = memOccupiedList;

        // insert p in ascending order by 'start'
        while (current != NULL && current->block.start <= p->block.start)
        {
            previous = current;
            current = current->next;
        }
        p->next = current;
        if (previous != NULL)
            previous->next = p;
        else
            memOccupiedList = p;
    }

// ================================================================================== //

} // end of namespace group

