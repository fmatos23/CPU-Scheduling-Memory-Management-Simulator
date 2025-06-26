/*
 *  \author Diogo Carvalho - 113221
 */

#include "somm24.h"

namespace group 
{

// ================================================================================== //

    void memTerm() 
    {
        soProbe(402, "%s()\n", __func__);

        require(memAllocationPolicy != UndefMemoryAllocationPolicy, "Module is not in a valid open state!");
        require(memFreeList != UNDEF_MEM_NODE and memOccupiedList != UNDEF_MEM_NODE, "Module is not in a valid open state!");

        MemNode* currentNode = memOccupiedList;
        while (currentNode != NULL)
        {
            MemNode* nextNode = currentNode->next;
            free(currentNode);
            currentNode = nextNode;
        }

        currentNode = memFreeList;
        while (currentNode != NULL)
        {
            MemNode* nextNode = currentNode->next;
            free(currentNode);
            currentNode = nextNode;
        }

        memOccupiedList = UNDEF_MEM_NODE;
        memFreeList = UNDEF_MEM_NODE;
        memAllocationPolicy = UndefMemoryAllocationPolicy;
    }

// ================================================================================== //

} // end of namespace group

