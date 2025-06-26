/*
 *  \author Bruno Santos
 */

#include "somm24.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

namespace group
{

// ================================================================================== //

    uint16_t jdtLoad(FILE *fin, uint32_t maxSize)
{
    soProbe(204, "%s(%p, %#x)\n", __func__, fin, maxSize);

    require(jdtIn != UNDEF_JOB_INDEX and jdtOut != UNDEF_JOB_INDEX, "Module is not in a valid open state!");
    require(jdtCount != UNDEF_JOB_COUNT, "Module is not in a valid open state!");
    require(fin != NULL and fileno(fin) != -1, "fin must be a valid file stream");

    
    
    char line[1024];
    bool errorOccurred = false;
    uint16_t jobsRead = 0;
    bool Jobarea = false;  // Flag para verificar se estamos entre begin e endjobs
    fseek(fin, 0, SEEK_SET); // o ponteiro apontava para o fim por isso voltamos para o inicio do file para o conseguir ler
    

    while (fgets(line, sizeof(line), fin) != NULL) {
        
        

        // procura comentarios e termina o string nesse ponto
        char* commentStart = strchr(line, '#');
        if (commentStart != NULL) {
            *commentStart = '\0';  
        }
        // skip espaços brancos
        char* whitespac = line;
        while (*whitespac == ' ' || *whitespac == '\t') {
            whitespac++;
        }

        //apagar espaços em branco que ficam para trás
        char* end = whitespac + strlen(whitespac) - 1;
        while (end >= whitespac && (*end == ' ' || *end == '\t')) {
            *end = '\0';
            end--;
        }

        // ignorar linhas em branco e comentadas
        if (strlen(whitespac) == 0) {
            continue;
        }

        // procurar begin jobs e end jobs e ativar a flag quando a fla está a um verifica-se se o parsing é válido e caso seja este é adiionado á tabela
        if (strstr(whitespac, "Begin Jobs") != NULL) {
            Jobarea = true;
            continue;
        }
        if (strstr(whitespac, "End Jobs") != NULL) {
            Jobarea = false;
            continue;
            
        }

        
        if (Jobarea) {
            double submissionTime, lifetime;
            uint32_t memSize;

            
            int parsed = sscanf(whitespac, "%lf ; %lf ; %X", &submissionTime, &lifetime, &memSize);
            
            if (parsed != 3 || submissionTime < 0 || lifetime <= 0 || memSize == 0 || memSize > maxSize) {
                fprintf(stderr, "Error parsing job description: %s\n", line);
                errorOccurred = true;//erro semantico ou sintaxe
                continue;
            }
            
            uint16_t idx = (jdtOut + jdtCount) % MAX_JOBS;
            jdtTable[idx] = Job{submissionTime, lifetime, memSize};
            jdtCount++;
            jobsRead++;
            //se estiver cheia paramos
            if (jdtCount >= MAX_JOBS) {
                fprintf(stderr, "Job table is full\n");
                break;
            }
        }
    }
    if (errorOccurred) {
        throw Exception(EINVAL, __func__);
    }

    

    return jobsRead;
}
// ================================================================================== //

} // end of namespace group
