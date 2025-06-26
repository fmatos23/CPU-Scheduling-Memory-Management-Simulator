/*
 *  \author João Pinho - 113602
 */

#include "somm24.h"

namespace group 
{

// ================================================================================== //

    void pctTerm() 
    {
        soProbe(302, "%s()\n", __func__);

        
        require(pctList != UNDEF_PCT_NODE, "Module is not in a valid open state!");
        
        
        pctList = NULL;
        for (int i = 0; i < MAX_JOBS; i++) {
            pctPID[i] = UNDEF_PID;
        }

        pctList = UNDEF_PCT_NODE;
    }

// ================================================================================== //

} // end of namespace group
