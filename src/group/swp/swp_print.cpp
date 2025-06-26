 
/*
 *  \author Luís Rodrigues, 114560
 */

#include "somm24.h"
#include <stdio.h>

namespace group
{

    void swpPrint(FILE *fout)
    {
        soProbe(603, "%s(%p)\n", __func__, fout);

        // Validar o estado do módulo
        require(swappingPolicy == FIFO or swappingPolicy == FirstFit, "Module is not in a valid open state!");
        require(swpList != UNDEF_SWP_NODE and swpTail != UNDEF_SWP_NODE, "Module is not in a valid open state!");
        require(fout != nullptr and fileno(fout) != -1, "Output file stream is invalid!");

        // Imprimir o cabeçalho do módulo
        fprintf(fout, "+=====================+\n");
        fprintf(fout, "|  SWP Module State   |\n");
        if (swappingPolicy == FIFO){
            fprintf(fout, "|       (%s)        |\n", "FIFO");
        }
        else{
            fprintf(fout, "|     (%s)      |\n", "FirstFit");
        }
   
        fprintf(fout, "+-------+-------------+\n");
        fprintf(fout, "|  PID  | memory size |\n");
        fprintf(fout, "+-------+-------------+\n");

        // Iterar pela lista de processos em swap
        SwpNode *current = swpList;
        while (current != NULL) {
            
            int len = snprintf(NULL, 0, "0x%x", current->process.size);
            fprintf(fout, "| %5hu | %*s0x%x |\n", current->process.pid, 11 - len, "", current->process.size);

            current = current->next;
        }

        fprintf(fout, "+=====================+\n");
    }

}