/*
 *  \author Diogo Carvalho - 113221
 */

#include "somm24.h"

#include <stdint.h>

namespace group 
{

// ================================================================================== //

    MemNode *memRetrieveNodeFromFreeList(uint32_t size, MemoryAllocationPolicy policy)
    {
        soProbe(409, "%s(%#x, %d)\n", __func__, size, policy);

        require(memAllocationPolicy != UndefMemoryAllocationPolicy, "Module is not in a valid open state!");
        require(memFreeList != UNDEF_MEM_NODE and memOccupiedList != UNDEF_MEM_NODE, "Module is not in a valid open state!");
        require(policy == BestFit or policy == WorstFit, "Allocation policy must be 'BestFit' or 'WorstFit'");

        MemNode* previousNode = NULL;
        MemNode* candidateNode = NULL;
        MemNode* candidatePreviousNode = NULL;
        MemNode* currentNode = memFreeList;

        // find first node that fits
        while (currentNode != NULL && currentNode->block.size < size)
        {
            previousNode = currentNode;
            currentNode = currentNode->next;
        }

        if (currentNode == NULL)
            candidateNode = NULL;
        else
        {
            // initialize candidate
            candidateNode = currentNode;
            candidatePreviousNode = previousNode;

            // search for best or worst fit
            MemNode* iteratorNode = currentNode->next;
            MemNode* iteratorPreviousNode = currentNode;
            while (iteratorNode != NULL)
            {
                if (iteratorNode->block.size >= size)
                {
                    if ((policy == BestFit  && iteratorNode->block.size < candidateNode->block.size) ||
                        (policy == WorstFit && iteratorNode->block.size > candidateNode->block.size))
                    {
                        candidateNode = iteratorNode;
                        candidatePreviousNode = iteratorPreviousNode;
                    }
                }
                iteratorPreviousNode = iteratorNode;
                iteratorNode = iteratorNode->next;
            }

            // split if bigger than requested size
            if (size < candidateNode->block.size)
            {
                MemNode* newNode = (MemNode*) malloc(sizeof(MemNode));
                newNode->block.start = candidateNode->block.start;
                newNode->block.size = size;
                candidateNode->block.start += size;
                candidateNode->block.size -= size;
                candidateNode = newNode;
            }

            // remove candidateNode from the free list
            else if (candidatePreviousNode == NULL)
                memFreeList = candidateNode->next;
            else
                candidatePreviousNode->next = candidateNode->next;
        }

        return candidateNode;
    }

// ================================================================================== //

    MemNode *memRetrieveNodeFromOccupiedList(uint32_t address)
    {
        soProbe(410, "%s(%#x)\n", __func__, address);

        require(memAllocationPolicy != UndefMemoryAllocationPolicy, "Module is not in a valid open state!");
        require(memFreeList != UNDEF_MEM_NODE and memOccupiedList != UNDEF_MEM_NODE, "Module is not in a valid open state!");

        MemNode* previousOccupiedNode = NULL;
        MemNode* currentOccupiedNode = memOccupiedList;

        while (currentOccupiedNode != NULL)
        {
            if (currentOccupiedNode->block.start == address)
            {
                // remove currentOccupiedNode from occupied
                if (previousOccupiedNode == NULL)
                    memOccupiedList = currentOccupiedNode->next;
                else
                    previousOccupiedNode->next = currentOccupiedNode->next;
                return currentOccupiedNode;
            }
            if (currentOccupiedNode->block.start > address)
                break;

            previousOccupiedNode = currentOccupiedNode;
            currentOccupiedNode = currentOccupiedNode->next;
        }

        throw Exception(EINVAL, __func__);
    }

// ================================================================================== //

} // end of namespace group

