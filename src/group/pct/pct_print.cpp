/*
 *  \author João Pinho - 113602
 */

#include "somm24.h"
#include <stdio.h>
#include <stdint.h>

namespace group 
{

// ================================================================================== //
    const char *processStateToStr(ProcessState state);
    void pctPrint(FILE *fout)
    {
        soProbe(303, "%s(%p)\n", __func__, fout);

        require(pctList != UNDEF_PCT_NODE, "Module is not in a valid open state!");
        require(fout != NULL and fileno(fout) != -1, "fout must be a valid file stream");

        fprintf(fout, "+======================================================================================================================+\n");
        fprintf(fout, "|                                                   PCT module state                                                   |\n");
        fprintf(fout, "+-------+-------------+-------------+-------------+------------+------------+-------------+--------------+-------------+\n");
        fprintf(fout, "|  PID  |    state    |  admission  |  lifetime   | store time | start time | finish time | memory start | memory size |\n");
        fprintf(fout, "+-------+-------------+-------------+-------------+------------+------------+-------------+--------------+-------------+\n");

        
        PctNode *current = pctList;
        while (current != NULL) {
            
            const char *stateStr = processStateToStr(current->pcb.state);

            
            const char *finishTimeStr;
            char finishTimeVal[32];  

            if (current->pcb.finishTime == UNDEF_TIME) {
                finishTimeStr = "UNDEF";
            } else {
                snprintf(finishTimeVal, sizeof(finishTimeVal), "%0.1f", current->pcb.finishTime);
                finishTimeStr = finishTimeVal;  
            }

           
            const char *storeTimeStr;
            char storeTimeVal[32]; 

            if (current->pcb.storeTime == UNDEF_TIME) {
                storeTimeStr = "UNDEF";
            } else {
                snprintf(storeTimeVal, sizeof(storeTimeVal), "%0.1f", current->pcb.storeTime);
                storeTimeStr = storeTimeVal;  
            }

            const char *startTimeStr;
            char startTimeVal[32];  

            if (current->pcb.startTime == UNDEF_TIME) {
                startTimeStr = "UNDEF";
            } else {
                snprintf(startTimeVal, sizeof(startTimeVal), "%0.1f", current->pcb.startTime);
                startTimeStr = startTimeVal;  
            }
            
            const char *memStartStr;
            
            if (current->pcb.memStart == UNDEF_ADDRESS) {
                memStartStr = "UNDEF";
                  fprintf(fout, "| %5hu | %-11s | %11.1f | %11.1f | %10s | %10s | %11s |   %10s |   %#9x |\n",
                    current->pcb.pid,
                    stateStr,
                    current->pcb.admissionTime,
                    current->pcb.lifetime,
                    storeTimeStr,
                    startTimeStr,
                    finishTimeStr,
                    memStartStr,
                    current->pcb.memSize);

                current = current->next;
            } else {
                 fprintf(fout, "| %5hu | %-11s | %11.1f | %11.1f | %10s | %10s | %11s |   %#10x |   %#9x |\n",
                    current->pcb.pid,
                    stateStr,
                    current->pcb.admissionTime,
                    current->pcb.lifetime,
                    storeTimeStr,
                    startTimeStr,
                    finishTimeStr,
                    current->pcb.memStart,
                    current->pcb.memSize);

                current = current->next;
               
            }
           
           
        }

     
        fprintf(fout, "+======================================================================================================================+\n");
    }

// ================================================================================== //

 
    const char *processStateToStr(ProcessState state)
    {
        switch (state)
        {
        case NEW:
            return "NEW";
        case READY:
            return "READY";
        case RUNNING:
            return "RUNNING";
        case SWAPPED:
            return "SWAPPED";
        case TERMINATED:
            return "TERMINATED";
        default:
            return "UNKNOWN";
        }
    }

// ================================================================================== //

} // end of namespace group