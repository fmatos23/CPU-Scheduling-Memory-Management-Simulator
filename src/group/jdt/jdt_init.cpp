/*
 *  \author Bruno Santos
 */

#include "somm24.h"

namespace group
{

// ================================================================================== //

    void jdtInit()
    {
        soProbe(201, "%s()\n", __func__);

        require(jdtIn == UNDEF_JOB_INDEX and jdtOut == UNDEF_JOB_INDEX, "Module is not in a valid closed state!");
        require(jdtCount == UNDEF_JOB_COUNT, "Module is not in a valid closed state!");

        
        for (uint16_t i = 0; i < MAX_JOBS; i++) {
            jdtTable[i] = Job{0.0, 0.0, 0}; // Valores default para inicializacao
        }

        // inicializar valores das variaveis
        jdtIn = 0;          
        jdtOut = 0;         
        jdtCount = 0;       

        
    }

// ================================================================================== //

} // end of namespace group

