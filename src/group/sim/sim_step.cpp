/*
 *  \author Luís Rodrigues, 114560
 *  \author Diogo Carvalho, 113221
 */

#include "somm24.h"

namespace group
{

// ================================================================================== //

    bool simStep()
    {
        soProbe(105, "%s()\n", __func__);

        require(simTime != UNDEF_TIME and stepCount != UNDEF_COUNT, "Module not in a valid open state!");
        require(submissionTime != UNDEF_TIME and runoutTime != UNDEF_TIME, "Module is not in a valid open state!");
        require(runningProcess != UNDEF_PID, "Module not in a valid open state!");

        // se não houver nada para fazer, bye bye
        if (submissionTime == NEVER && runoutTime == NEVER) {
            return false;
        }

        // caso não haja um submissionTime ou se o runoutTime for mais cedo, tratar do runout primeiro
        if (submissionTime == NEVER || (runoutTime <= submissionTime)) {
            // RUNOUT DE UM JOB
            simTime = runoutTime;
            pctUpdateState(runningProcess, TERMINATED, simTime, UNDEF_COUNT);

            // limpar a memória do processo que está a rodar
            uint32_t address = pctMemAddress(runningProcess);
            memFree(address);

            // coloca todos os processos swapped em memória até que mais nenhum caiba
            while (1) {
                uint32_t sizeAvailable = memBiggestFreeBlockSize();
                uint16_t swappedProcess = swpFetch(sizeAvailable);
                if (swappedProcess == 0) break;
                uint32_t startAddress = memAlloc(pctMemSize(swappedProcess));
                double lifetime = pctLifetime(swappedProcess);
                rdyInsert(swappedProcess, lifetime);
                pctUpdateState(swappedProcess, READY, simTime, startAddress);
            }
            runningProcess = 0;
            runoutTime = NEVER;
        } else {
            // SUBMISSION DE UM JOB
            simTime = submissionTime;

            // dar fetch a um job (tirando-o da lista) e iniciar o processo
            Job job = jdtFetchNext();
            uint16_t newPid = pctNewProcess(simTime, job.lifetime, job.memSize);
            uint32_t sizeAvailable = memBiggestFreeBlockSize();

            // se não houver memória suficiente, vai para o swap
            if (sizeAvailable < job.memSize) {
                swpInsert(newPid, job.memSize);
                pctUpdateState(newPid, SWAPPED, UNDEF_TIME, UNDEF_COUNT);
            } else {
                uint32_t startAddress = memAlloc(job.memSize);
                rdyInsert(newPid, job.lifetime);
                pctUpdateState(newPid, READY, simTime, startAddress);
            }
            // preparar a próxima submission
            submissionTime = jdtNextSubmission();
        }

        // se não houver processos a rodar, ir buscar um processo ready
        if (runningProcess == 0) {
            uint16_t newPid = rdyFetch();
            if (newPid != 0) {
                pctUpdateState(newPid, RUNNING, simTime, UNDEF_COUNT);
                runningProcess = newPid;
                runoutTime = pctLifetime(newPid) + simTime;
            }
        }
        stepCount++;
        return true;
    }

// ================================================================================== //

} // end of namespace group