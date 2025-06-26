/*
 *  \author Luís Rodrigues, 114560
 */

#include "somm24.h"

namespace group
{

// ================================================================================== //

    void simRun(uint32_t cnt)
    {
        soProbe(106, "%s(%u)\n", __func__, cnt);

        require(simTime != UNDEF_TIME and stepCount != UNDEF_COUNT, "Module not in a valid open state!");
        require(submissionTime != UNDEF_TIME and runoutTime != UNDEF_TIME, "Module is not in a valid open state!");
        require(runningProcess != UNDEF_PID, "Module is not in a valid open state!");
        // Loop para executar os passos da simulação
        uint32_t executedSteps = 0;
        while (cnt == 0 || executedSteps < cnt)
        {
            
            // Processar um único passo da simulação
            if (!simStep())
            {
             
                break; // Terminar se não houver mais passos a serem processados
            }

            // Incrementar o contador de passos executados
            
            executedSteps++;
        }

        // Atualizar o número total de passos executados no módulo
        stepCount += executedSteps;

       
    }

// ================================================================================== //

} // end of namespace group