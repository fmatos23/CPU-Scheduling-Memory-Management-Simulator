/*
 *  \author Diogo Carvalho - 113221
 */

#include "somm24.h"

#include <stdio.h>
#include <stdint.h>

namespace group 
{

// ================================================================================== //

    void memPrint(FILE *fout)
    {
        soProbe(403, "%s(\"%p\")\n", __func__, fout);

        require(memAllocationPolicy != UndefMemoryAllocationPolicy, "Module is not in a valid open state!");
        require(memFreeList != UNDEF_MEM_NODE and memOccupiedList != UNDEF_MEM_NODE, "Module is not in a valid open state!");
        require(fout != NULL and fileno(fout) != -1, "fout must be a valid file stream");

        fprintf(fout, "+===============================+\n");
        fprintf(fout, "|       MEM module state        |\n");
        if (memAllocationPolicy == WorstFit)
            fprintf(fout, "|          (WorstFit)           |\n");
        else
            fprintf(fout, "|           (BestFit)           |\n");

        fprintf(fout, "+-------------------------------+\n");
        fprintf(fout, "|         occupied list         |\n");
        fprintf(fout, "+---------------+---------------+\n");
        fprintf(fout, "|  start frame  |     size      |\n");
        fprintf(fout, "+---------------+---------------+\n");
        MemNode* node = memOccupiedList;
        while (node != NULL)
        {
            fprintf(fout, "| %#13x | %#13x |\n",
                    (unsigned int) node->block.start,
                    (unsigned int) node->block.size);
            node = node->next;
        }
        fprintf(fout, "+---------------+---------------+\n");
        fprintf(fout, "|            free list          |\n");
        fprintf(fout, "+---------------+---------------+\n");
        fprintf(fout, "|  start frame  |     size      |\n");
        fprintf(fout, "+---------------+---------------+\n");
        node = memFreeList;
        while (node != NULL)
        {
            fprintf(fout, "| %#13x | %#13x |\n",
                    (unsigned int) node->block.start,
                    (unsigned int) node->block.size);
            node = node->next;
        }
        fprintf(fout, "+===============================+\n");
    }

// ================================================================================== //

} // end of namespace group

