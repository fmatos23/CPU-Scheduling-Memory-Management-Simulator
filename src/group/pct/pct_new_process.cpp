/*
 *  \author João Pinho - 113602
 */

#include "somm24.h"
#include <stdint.h>

namespace group 
{

// ================================================================================== //

    uint16_t pctNewProcess(double admissionTime, double lifetime, uint32_t memSize)
    {
        soProbe(304, "%s(%0.1f, %0.1f, %#x)\n", __func__, admissionTime, lifetime, memSize);

        require(pctList != UNDEF_PCT_NODE, "Module is not in a valid open state!");
        require(admissionTime >= 0, "Bad admission time");
        require(lifetime > 0, "Bad lifetime");
        require(memSize > 0, "Bad memory size");

        
        static uint16_t nextPIDIndex = 0;
        require(nextPIDIndex < MAX_JOBS, "No available PIDs!");

        
        PctBlock newProcess;
        newProcess.pid = pctPID[nextPIDIndex++];
        newProcess.state = NEW;
        newProcess.storeTime = UNDEF_TIME;
        newProcess.startTime = UNDEF_TIME;
        newProcess.finishTime = UNDEF_TIME;
        newProcess.memStart = UNDEF_ADDRESS;
        newProcess.admissionTime = admissionTime;
        newProcess.lifetime = lifetime;
        newProcess.memSize = memSize;

        
        PctNode *newNode = (PctNode *)malloc(sizeof(PctNode));
        newNode->pcb = newProcess;
        newNode->next = NULL;

        
        if (pctList == NULL || pctList->pcb.pid > newNode->pcb.pid) {
            newNode->next = pctList;
            pctList = newNode;
        } else {
            PctNode *current = pctList;
            while (current->next != NULL && current->next->pcb.pid < newNode->pcb.pid) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }

        
        return newProcess.pid;
    }

// ================================================================================== //

} // end of namespace group