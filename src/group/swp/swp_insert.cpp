/*
 *  \author Luís Rodrigues, 114560
 */

#include "somm24.h"

namespace group
{

    void swpInsert(uint16_t pid, uint32_t size)
    {
        soProbe(604, "%s(%hu, %u)\n", __func__, pid, size);

        // Validar o estado do módulo
        require(swappingPolicy == FIFO || swappingPolicy == FirstFit, "Module is not in a valid open state!");
        require(swpList != UNDEF_SWP_NODE && swpTail != UNDEF_SWP_NODE, "Module is not in a valid open state!");
        require(pid > 0, "A valid process ID must be greater than zero");
        require(size > 0, "Memory size must be greater than zero");

        // Criar um novo nó
        SwpNode *newNode = new SwpNode();  // Alocar memória para o novo nó
        newNode->process.pid = pid;       // Definir o PID
        newNode->process.size = size;     // Definir o tamanho
        newNode->next = NULL;          // Último nó não aponta para nada

        // Adicionar à fila
        if (swpList == NULL) {
            // Se a fila está vazia
            swpList = newNode;  // Cabeça aponta para o novo nó
            swpTail = newNode;  // Fim aponta para o novo nó
        } else {
            // Caso contrário, adicione ao final
            swpTail->next = newNode;  // Último nó atual aponta para o novo
            swpTail = newNode;        // Atualiza o ponteiro do final
        }

    }

}
