/*
 *  \author João Pinho - 113602
 */

#include "somm24.h"


namespace group 
{

// ================================================================================== //
    
    
    void pctInit(uint16_t pid0, uint16_t cnt, uint32_t seed) 
    {
        soProbe(301, "%s(%hu, %hu, %u)\n", __func__, pid0, cnt, seed);
        
       
        require(pctList == UNDEF_PCT_NODE, "The module is not in a valid closed state");
        require(cnt > 1 and cnt <= MAX_JOBS, "cnt must be > 1 and <= MAX_JOBS");
        
        for (uint16_t i = 0; i < cnt; i++) {
            pctPID[i] = pid0 + i;
        }
        srand(seed);
        for (uint16_t i = 0; i < cnt; i++) {
            
            uint16_t j = rand() % cnt;
            uint16_t temp = pctPID[i];
            pctPID[i] = pctPID[j];
            pctPID[j] = temp;        
        }

        for (uint16_t i = cnt; i < MAX_JOBS; i++) {
            pctPID[i] = 0;
        }

        pctList = NULL;
        

       
    }

// ================================================================================== //

} // end of namespace group
