/*
 *  \author Francisco Matos 1137126
 */

#include "somm24.h"

namespace group
{

// ================================================================================== //

    void rdyInsert(uint16_t pid, double lifetime)
    {
        soProbe(504, "%s(%hu, %.1f)\n", __func__, pid, lifetime);

        require(schedulingPolicy == FCFS or schedulingPolicy == SPN, "Module is not in a valid open state!");
        require(rdyList != UNDEF_RDY_NODE and rdyTail != UNDEF_RDY_NODE, "Module is not in a valid open state!");
        require(pid != 0, "a valid process ID must be greater than zero");
        require(lifetime > 0, "a valid process lifetime must be greater than zero");

        /* TODO POINT: Replace next instruction with my code */
        RdyNode *newNode = (RdyNode *)malloc(sizeof(RdyNode));
        if (newNode == NULL) {
            throw Exception(ENOMEM, "Memory allocation failed!");
        }
        // Preencher os dados do processo
        newNode->process.pid = pid;
        newNode->process.lifetime = lifetime;
        newNode->next = NULL;
        // Inserir o nó na lista
        if (schedulingPolicy == FCFS)
        {
            // Inserção no final da lista (FCFS)
            if (rdyList == NULL)
            {
                // Lista está vazia
                rdyList = rdyTail = newNode;
            }
            else
            {
                // Inserir no final
                rdyTail->next = newNode;
                rdyTail = newNode;
            }
        }
        else if (schedulingPolicy == SPN)
        {
            // Inserção ordenada (SPN)
            if (rdyList == NULL or lifetime < rdyList->process.lifetime)
            {
                // Inserir no início da lista
                newNode->next = rdyList;
                rdyList = newNode;
                if (rdyTail == NULL)
                    rdyTail = newNode; // Lista tinha apenas um elemento
            }
            else
            {
                // Inserir no meio ou no final
                RdyNode *current = rdyList;
                while (current->next != NULL and current->next->process.lifetime < lifetime)
                {
                    current = current->next;
                }
                // Inserir o nó
                newNode->next = current->next;
                current->next = newNode;
                // Atualizar o tail, se necessário
                if (newNode->next == NULL)
                    rdyTail = newNode;
            }
        }
    }

// ================================================================================== //

} // end of namespace group
