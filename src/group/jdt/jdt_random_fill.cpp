/*
 *  \author Bruno Santos
 */


#include "somm24.h"
#include <stdint.h>

namespace group
{
    // Gerador de numeros random LCG utilizando a seed
    uint32_t simpleRand(uint32_t &seed)
    {
        const uint32_t a = 1664525; 
        const uint32_t c = 1013904223; 
        const uint32_t m = 0xFFFFFFFF; 
        seed = (a * seed + c) & m;
        return seed;
    }

    // Função para gerar valor aleatorio entre low e high com um step definido
    // Funciona de forma simples calculamos o numero de steps entre os dois valores limiar geramos um valor aleatorio no range e calculamos o valor de saida 
    // somando o numero de steps definido
    float rdmfloat(uint32_t &seed, float low, float high, float step)
    {
        
        uint32_t range = static_cast<uint32_t>((high - low) / step);
        uint32_t randInt = simpleRand(seed) % (range + 1);
        return low + randInt * step;
    }

    uint16_t jdtRandomFill(uint32_t seed, uint16_t cnt, uint32_t maxSize)
    {
        soProbe(205, "%s(%u, %u, %#x)\n", __func__, seed, cnt, maxSize);

        require(jdtIn != UNDEF_JOB_INDEX and jdtOut != UNDEF_JOB_INDEX, "Module is not in a valid open state!");
        require(jdtCount != UNDEF_JOB_COUNT, "Module is not in a valid open state!");
        require(cnt == 0 or (cnt >= 2 and cnt <= MAX_JOBS), "Number of jobs is invalid");

        
        float cTime = 0.0f;
        uint16_t jobsRead = 0;

        // cnt=0 geramos entre 2 e Max
        if (cnt == 0)
        {
            cnt = 2 + (simpleRand(seed) % (MAX_JOBS - 2)); 
        }

        for (uint16_t i = 0; i < cnt; ++i)
        {
            // Gerar os submisssion times aleatorios incremento entre 0 e 100 com step 0.1
            float Increment = rdmfloat(seed, 0.0f, 100.0f, 0.1f);
            cTime += Increment;

            // Gerar lifetime de 10 a 1000 com step 0.1
            float lifetime = rdmfloat(seed, 10.0f, 1000.0f, 0.1f); 

            // Gerar size entre 1 e o tamanho máximo
            uint32_t memSize = 1 + (simpleRand(seed) % maxSize);            
            uint16_t idx = (jdtOut + jdtCount) % MAX_JOBS;
            jdtTable[idx] = Job{cTime, lifetime, memSize};
            jdtCount++;
            jobsRead++;
            //verificar se a tabela está cheia
            if (jdtCount >= MAX_JOBS)
            {
                break;
            }
        }

        return jobsRead;
    }
}
