/*
 *  \author Francisco Matos 113726
 */

#include "somm24.h"

namespace group
{

// ================================================================================== //

    uint16_t rdyFetch()
    {
        soProbe(505, "%s()\n", __func__);

        require(schedulingPolicy == FCFS or schedulingPolicy == SPN, "Module is not in a valid open state!");
        require(rdyList != UNDEF_RDY_NODE and rdyTail != UNDEF_RDY_NODE, "Module is not in a valid open state!");

        if (rdyList == NULL) {
            return 0; // List is empty
        }

        // Remove the first entry from the list
        RdyNode *node = rdyList;
        uint16_t pid = node->process.pid;
        rdyList = node->next;

        // If the list is now empty, update rdyTail
        if (rdyList == NULL) {
            rdyTail = NULL;
        }

        // Free the memory of the removed node
        free(node);

        return pid;
    }

// ================================================================================== //

} // end of namespace group
