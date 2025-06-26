/*
 *  \author Luís Rodrigues, 114560
 */

#include "somm24.h"

namespace group
{

    void swpTerm()
    {
        soProbe(602, "%s()\n", __func__);

        // Validar o estado do módulo
        require(swappingPolicy == FIFO || swappingPolicy == FirstFit, "Module is not in a valid open state!");
        require(swpList != UNDEF_SWP_NODE && swpTail != UNDEF_SWP_NODE, "Module is not in a valid open state!");

        // Liberar todos os nós da lista
        SwpNode *current = swpList;
        while (current != NULL) {
            SwpNode *toDelete = current;  // Guardar o nó atual
            current = current->next;      // Avançar para o próximo nó
            free(toDelete);              // Liberar memória do nó atual
        }

        // Redefinir os ponteiros da lista
        swpList = NULL;
        swpTail = NULL;

        // Redefinir a política de swapping
        swappingPolicy = UndefSwappingPolicy;
    }

}
