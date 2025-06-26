/*
 *  \author Francisco Matos 113726
 */

#include "somm24.h"

namespace group
{

    // Custom function to check if a value is infinity
    bool isInf(double value) {
        return value == INFINITY || value == -INFINITY;
    }

    void simPrint(FILE *fout, uint32_t satelliteModules)
    {
        soProbe(103, "%s(\"%p\")\n", __func__, fout);

        require(fout != nullptr && fileno(fout) != -1, "fout must be a valid file stream");

        char runningProcessStr[16];
        char submissionTimeStr[16];
        char runoutTimeStr[16];

        if (runningProcess == UNDEF_PID || runningProcess == 0) {
            snprintf(runningProcessStr, sizeof(runningProcessStr), "---");
        } else {
            snprintf(runningProcessStr, sizeof(runningProcessStr), "%d", runningProcess);
        }

        if (isInf(submissionTime)) {
            snprintf(submissionTimeStr, sizeof(submissionTimeStr), "NEVER");
        } else {
            snprintf(submissionTimeStr, sizeof(submissionTimeStr), "%.1f", submissionTime);
        }

        if (isInf(runoutTime)) {
            snprintf(runoutTimeStr, sizeof(runoutTimeStr), "NEVER");
        } else {
            snprintf(runoutTimeStr, sizeof(runoutTimeStr), "%.1f", runoutTime);
        }

        jdtPrint(fout);
        fprintf(fout, "\n");

        pctPrint(fout);
        fprintf(fout, "\n");

        memPrint(fout);
        fprintf(fout, "\n");

        rdyPrint(fout);
        fprintf(fout, "\n");

        swpPrint(fout);
        fprintf(fout, "\n");

        fprintf(fout, "+====================================================================================+\n");
        fprintf(fout, "+ -------------------------------- SIM Module State -------------------------------- +\n");
        fprintf(fout, "+====================================================================================+\n");
        fprintf(fout, "| simulation time |  step count  | running process | next submission |  next runout  |\n");
        fprintf(fout, "+-----------------+--------------+-----------------+-----------------+---------------+\n");
        fprintf(fout, "| %15.1f | %12u | %15s | %15s | %13s |\n",
                simTime,
                stepCount,
                runningProcessStr,
                submissionTimeStr,
                runoutTimeStr);
        fprintf(fout, "+====================================================================================+\n");

    }

}