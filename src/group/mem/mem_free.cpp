/*
 *  \author Diogo Carvalho - 113221
 */

#include "somm24.h"

#include <stdint.h>

namespace group 
{

// ================================================================================== //

    void memFree(uint32_t address)
    {
        soProbe(405, "%s(%#x)\n", __func__, address);

        require(memAllocationPolicy != UndefMemoryAllocationPolicy, "Module is not in a valid open state!");
        require(memFreeList != UNDEF_MEM_NODE and memOccupiedList != UNDEF_MEM_NODE, "Module is not in a valid open state!");

        MemNode* node = memRetrieveNodeFromOccupiedList(address);
        memAddNodeToFreeList(node);
    }

// ================================================================================== //

} // end of namespace group

