/*
 *  \author Bruno Santos
 */

#include "somm24.h"

#include <stdio.h>
#include <stdint.h>

namespace group 
{

// ================================================================================== //

    void jdtPrint(FILE *fout)
    {
        soProbe(203, "%s(%p)\n", __func__, fout);

        require(jdtIn != UNDEF_JOB_INDEX and jdtOut != UNDEF_JOB_INDEX, "Module is not in a valid open state!");
        require(jdtCount != UNDEF_JOB_COUNT, "Module is not in a valid open state!");
        require(fout != NULL and fileno(fout) != -1, "fout must be a valid file stream");

        
        fprintf(fout, "+=====================================+\n");
        fprintf(fout, "|          JDT module state           |\n");
        fprintf(fout, "+------------+------------+-----------+\n");
        fprintf(fout, "| submission |  lifetime  |  memory   |\n");
        fprintf(fout, "|    time    |            | requested |\n");
        fprintf(fout, "+------------+------------+-----------+\n");

        
        for (uint16_t i = 0; i < jdtCount; i++) {
            uint16_t idx = (jdtOut + i) % MAX_JOBS;
            const Job &job = jdtTable[idx];
            fprintf(fout, "| %10.1f | %10.1f | %#9x |\n", job.submissionTime, job.lifetime, job.memSize);

        }

        fprintf(fout, "+=====================================+\n");
    }

// ================================================================================== //

} // end of namespace group

