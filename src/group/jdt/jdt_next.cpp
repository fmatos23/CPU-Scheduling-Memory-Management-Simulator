/*
 *  \author Bruno Santos
 */


#include "somm24.h"

namespace group
{

// ================================================================================== //
    double jdtNextSubmission()
    {
        soProbe(206, "%s()\n", __func__);

        require(jdtIn != UNDEF_JOB_INDEX and jdtOut != UNDEF_JOB_INDEX, "Module is not in a valid open state!");
        require(jdtCount != UNDEF_JOB_COUNT, "Module is not in a valid open state!");

    
    if (jdtCount == 0)
    {
        return NEVER;
    }

    double submissionTime = jdtTable[jdtOut].submissionTime;

    
    return submissionTime;
}
//=========================================================================== //

    Job jdtFetchNext()
    {
        soProbe(207, "%s()\n", __func__);

        // Ensure the module is in a valid open state
        require(jdtIn != UNDEF_JOB_INDEX and jdtOut != UNDEF_JOB_INDEX, "Module is not in a valid open state!");
        require(jdtCount != UNDEF_JOB_COUNT, "Module is not in a valid open state!");

        
        if (jdtCount == 0)
        {
            throw Exception(EINVAL, __func__);
        }
        Job job = jdtTable[jdtOut];       
        jdtOut = (jdtOut + 1) % MAX_JOBS;  
        jdtCount--;

        return job;
    }

// ================================================================================== //

} // end of namespace group
