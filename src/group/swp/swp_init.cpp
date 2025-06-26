/*
 *  \author Luís Rodrigues, 114560
 */

#include "somm24.h"

namespace group
{

    void swpInit(SwappingPolicy policy)
    {
        const char *policyStr;
        switch (policy)
        {
            case FIFO: policyStr = "FIFO"; break;
            case FirstFit: policyStr = "FirstFit"; break;
            case UndefSwappingPolicy: policyStr = "UndefSwappingPolicy"; break;
            default: policyStr = "InvalidPolicy"; break;
        }
        soProbe(601, "%s(%s)\n", __func__, policyStr);

        // Garantir que o módulo está num estado fechado
        require(swappingPolicy == UndefSwappingPolicy, "Module must be in a closed state!");
        require(swpList == UNDEF_SWP_NODE && swpTail == UNDEF_SWP_NODE, "Internal list pointers must be undefined!");
        require(policy == FIFO || policy == FirstFit, "Invalid swapping policy!");

        // Inicializar o estado do módulo
        swappingPolicy = policy; // Configura a política de swapping
        swpList = NULL;       // Lista inicializada como vazia
        swpTail = NULL;       // Ponteiro para o último nó também é nulo
    }

}
