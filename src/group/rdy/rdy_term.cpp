/*
 *  \author Francisco Matos 113726
 */

#include "somm24.h"

namespace group
{

// ================================================================================== //

    void rdyTerm()
    {
        soProbe(502, "%s()\n", __func__);

        require(schedulingPolicy == FCFS or schedulingPolicy == SPN, "Module is not in a valid open state!");
        require(rdyList != UNDEF_RDY_NODE and rdyTail != UNDEF_RDY_NODE, "Module is not in a valid open state!");

        // Release dynamically allocated memory
        RdyNode *current = rdyList;
        while (current != NULL) {
            RdyNode *next = current->next;
            free(current);
            current = next;
        }

        // Set the internal data structure to the closed state
        rdyList = UNDEF_RDY_NODE;
        rdyTail = UNDEF_RDY_NODE;
        schedulingPolicy = UndefSchedulingPolicy;
    }

// ================================================================================== //

} // end of namespace group
