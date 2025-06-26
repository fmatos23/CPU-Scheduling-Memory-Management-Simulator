/*
 *  \author Francisco Matos 113726
 */

#include "somm24.h"
#include <stdio.h>
#include <stdint.h>

namespace group
{

// ================================================================================== //

        void rdyPrint(FILE *fout)
        {

            soProbe(503, "%s(%p)\n", __func__, fout);

            // Validate module state and file stream
            require(schedulingPolicy == FCFS or schedulingPolicy == SPN, "Module is not in a valid open state!");
            require(rdyList != UNDEF_RDY_NODE and rdyTail != UNDEF_RDY_NODE, "Module is not in a valid open state!");
            require(fout != nullptr and fileno(fout) != -1, "fout must be a valid file stream");


    		fprintf(fout, "+====================+\n");
    		fprintf(fout, "|  RDY Module State  |\n");

    		// Adjust formatting based on schedulingPolicy
    		if (schedulingPolicy == SPN) {
        		fprintf(fout, "|       (%3s)        |\n", "SPN");
    		} else {
        		fprintf(fout, "|       (%4s)       |\n", "FCFS");
   		 	}

    		fprintf(fout, "+-------+------------+\n");
    		fprintf(fout, "|  PID  |  lifetime  |\n");
    		fprintf(fout, "+-------+------------+\n");

            // Print ready queue contents
            for (RdyNode *node = rdyList; node != NULL; node = node->next) {
                fprintf(fout, "| %5d | %10.1f |\n", node->process.pid, node->process.lifetime);
            }
            fprintf(fout, "+====================+\n");
        }
// ================================================================================== //

} // end of namespace group
