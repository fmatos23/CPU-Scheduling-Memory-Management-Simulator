/*
 *  \author Luís Rodrigues, 114560
 */

#include "somm24.h"

namespace group
{

    uint16_t swpFetch(uint32_t sizeAvailable)
    {
        soProbe(605, "%s(%#x)\n", __func__, sizeAvailable);

        // Validar o estado do módulo
        require(swappingPolicy == FIFO || swappingPolicy == FirstFit, "Module is not in a valid open state!");
        require(swpList != UNDEF_SWP_NODE && swpTail != UNDEF_SWP_NODE, "Module is not in a valid open state!");

        if (swpList == NULL) {
            return 0; // Lista vazia, nada para buscar
        }

        SwpNode *prev = NULL;
        SwpNode *current = swpList;

        // Política FIFO: Remover o primeiro que couber
        if (swappingPolicy == FIFO) {
            if (current->process.size <= sizeAvailable) {
                uint16_t pid = current->process.pid;
                swpList = current->next;
                if (swpList == NULL) {
                    swpTail = NULL; // Lista ficou vazia
                }
                free(current);
                return pid;
            }
        }

        // Política FirstFit: Encontrar o primeiro que caiba
        else if (swappingPolicy == FirstFit) {
            while (current != NULL) {
                if (current->process.size <= sizeAvailable) {
                    uint16_t pid = current->process.pid;

                    // Remover o nó da lista
                    if (prev == NULL) {
                        swpList = current->next;
                        if (swpList == NULL) {
                            swpTail = NULL; // Lista ficou vazia
                        }
                    } else {
                        prev->next = current->next;
                        if (current == swpTail) {
                            swpTail = prev; // Atualizar cauda
                        }
                    }

                    free(current);
                    return pid;
                }
                prev = current;
                current = current->next;
            }
        }

        return 0; // Nenhum processo coube
    }

}
