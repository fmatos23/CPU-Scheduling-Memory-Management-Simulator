/*
 *  \author João Pinho - 113602
 */

#include "somm24.h"
#include <stdio.h>
#include <stdint.h>
#include <unistd.h> 
#include <time.h>  

namespace group
{

    // ================================================================================== //

    void simInit(FILE *fin)
    {
        soProbe(101, "%s(%p)\n", __func__, fin);

        
        require(simTime == UNDEF_TIME and stepCount == UNDEF_COUNT, "Module is not in a valid closed state!");
        require(submissionTime == UNDEF_TIME and runoutTime == UNDEF_TIME, "Module is not in a valid closed state!");
        require(runningProcess == UNDEF_PID, "Module is not in a valid closed state!");
        require(fin == nullptr or fileno(fin) != -1, "fin must be NULL or a valid file pointer");

       
        SimParameters simParams = {
            .jobLoadStream = NULL,
            .jobMaxSize = 0x10000,
            .jobRandomSeed = UNDEF_SEED,
            .jobCount = 0,
            .pidStart = 1001,
            .pidRandomSeed = UNDEF_SEED,
            .memorySize = 0x100000,
            .memoryKernelSize = 0x10000,
            .memoryAllocPolicy = WorstFit,  
            .schedulingPolicy = FCFS,
            .swappingPolicy = FIFO
        };

        
        if (fin != NULL) {
            simConfig(fin, &simParams); 
        }

      
        if (simParams.jobRandomSeed == UNDEF_SEED) {
            simParams.jobRandomSeed = getpid(); 
        }
        if (simParams.pidRandomSeed == UNDEF_SEED) {
            simParams.pidRandomSeed = time(NULL); 
        }

        
        jdtInit();

        
        int jobCount = 0;
        if (simParams.jobLoadStream != NULL) {
            jobCount = jdtLoad(simParams.jobLoadStream, simParams.jobMaxSize);
        } else {
            jobCount = jdtRandomFill(simParams.jobRandomSeed, simParams.jobCount, simParams.jobMaxSize); 
        }


        
        pctInit(simParams.pidStart, jobCount, simParams.pidRandomSeed);

        
        memInit(simParams.memorySize, simParams.memoryKernelSize, simParams.memoryAllocPolicy);
        
        
        rdyInit(simParams.schedulingPolicy);

        
        swpInit(simParams.swappingPolicy);
    
        stepCount =  0;             
        simTime = 0.0;            
        submissionTime = jdtNextSubmission(); 
        runoutTime = NEVER;        
        runningProcess = 0; 
        
    }

// ================================================================================== //

} // end of namespace group