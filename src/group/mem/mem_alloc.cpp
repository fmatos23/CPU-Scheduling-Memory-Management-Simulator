/*
 *  \author Diogo Carvalho - 113221
 */

#include "somm24.h"

#include <stdint.h>
#include <string.h>

namespace group 
{

// ================================================================================== //

    uint32_t memBiggestFreeBlockSize()
    {
        soProbe(406, "%s()\n", __func__);

        require(memAllocationPolicy != UndefMemoryAllocationPolicy, "Module is not in a valid open state!");
        require(memFreeList != UNDEF_MEM_NODE and memOccupiedList != UNDEF_MEM_NODE, "Module is not in a valid open state!");

        if (memFreeList == NULL)
            return 0;

        uint32_t biggest = memFreeList->block.size;
        for (MemNode* node = memFreeList->next; node != NULL; node = node->next)
        {
            if (node->block.size > biggest)
                biggest = node->block.size;
        }
        return biggest;
    }

// ================================================================================== //

    uint32_t memAlloc(uint32_t size)
    {
        soProbe(404, "%s(%#x)\n", __func__, size);

        require(memAllocationPolicy != UndefMemoryAllocationPolicy, "Module is not in a valid open state!");
        require(memFreeList != UNDEF_MEM_NODE and memOccupiedList != UNDEF_MEM_NODE, "Module is not in a valid open state!");

        MemNode* newNode = memRetrieveNodeFromFreeList(size, memAllocationPolicy);
        if (newNode == NULL)
            throw Exception(EINVAL, __func__);

        memAddNodeToOccupiedList(newNode);
        return newNode->block.start;
    }

// ================================================================================== //

} // end of namespace group

