/*
 *  \author Francisco Matos 113726
 */

#include "somm24.h"
#include <string.h>

namespace group
{

    // Helper function to trim whitespaces
    char* trimWhitespace(char* str) {
        // Remove leading spaces
        while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r' || *str == '\f' || *str == '\v') str++;
        char* end = str + strlen(str) - 1;
        // Remove trailing spaces
        while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r' || *end == '\f' || *end == '\v')) end--;
        *(end + 1) = '\0';
        return str;
    }

    // Helper function to compare keys case-insensitively
    bool compareKey(const char* key, const char* reference) {
        return strcasecmp(key, reference) == 0;
    }

    // ================================================================================== //

    void simConfig(FILE *fin, SimParameters *spp)
    {
        soProbe(104, "%s(%p, %p)\n", __func__, fin, spp);

        require(simTime == UNDEF_TIME && stepCount == UNDEF_COUNT, "Module is not in a valid closed state!");
        require(submissionTime == UNDEF_TIME && runoutTime == UNDEF_TIME, "Module is not in a valid closed state!");
        require(runningProcess == UNDEF_PID, "Module is not in a valid closed state!");
        require(fin != nullptr && fileno(fin) != -1, "fin must be a valid file stream");
        require(spp != nullptr, "spp must be a valid pointer");

        char line[256];
        bool jobSection = false;
        bool errorOccurred = false;
        bool jobCountRead = false;
        bool jobRandomSeedRead = false;

        while (fgets(line, sizeof(line), fin) != NULL) {
            // Remove comments
            char *comment = strchr(line, '#');
            if (comment) {
                *comment = '\0';
            }

            // Trim leading and trailing whitespaces
            char *trimmed = trimWhitespace(line);
            if (!*trimmed) {
                continue; // Skip empty lines
            }

            // Check for job section tags
            if (compareKey(trimmed, "Begin Jobs")) {
                jobSection = true;
                continue;
            }
            if (compareKey(trimmed, "End Jobs")) {
                jobSection = false;
                continue;
            }
            if (jobSection) {
                continue;
            }

            if (sscanf(trimmed, "MemorySize = %x", &spp->memorySize) == 1) {
                continue;
            }
            if (sscanf(trimmed, "MemoryKernelSize = %x", &spp->memoryKernelSize) == 1) {
                continue;
            }
            if (sscanf(trimmed, "JobMaxSize = %x", &spp->jobMaxSize) == 1) {
                continue;
            }
            if (sscanf(trimmed, "JobRandomSeed = %d", &spp->jobRandomSeed) == 1) {
                jobRandomSeedRead = true;
                continue;
            }
            if (sscanf(trimmed, "PIDStart = %hu", &spp->pidStart) == 1) {
                continue;
            }
            if (sscanf(trimmed, "PIDRandomSeed = %d", &spp->pidRandomSeed) == 1) {
                continue;
            }
            if (sscanf(trimmed, "JobCount = %hu", &spp->jobCount) == 1) {
                jobCountRead = true;
                continue;
            }

            char policy[32] = {0};
            if (sscanf(trimmed, "MemoryAllocationPolicy = %31s", policy) == 1) {
                if (compareKey(policy, "BestFit")) {
                    spp->memoryAllocPolicy = BestFit;
                } else if (compareKey(policy, "WorstFit")) {
                    spp->memoryAllocPolicy = WorstFit;
                } else {
                    fprintf(stderr, "Error: Invalid MemoryAllocationPolicy: %s\n", policy);
                    errorOccurred = true;
                }
                continue;
            }

            if (sscanf(trimmed, "SchedulingPolicy = %31s", policy) == 1) {
                if (compareKey(policy, "FCFS")) {
                    spp->schedulingPolicy = FCFS;
                } else if (compareKey(policy, "SPN")) {
                    spp->schedulingPolicy = SPN;
                } else {
                    fprintf(stderr, "Error: Invalid SchedulingPolicy: %s\n", policy);
                    errorOccurred = true;
                }
                continue;
            }

            if (sscanf(trimmed, "SwappingPolicy = %31s", policy) == 1) {
                if (compareKey(policy, "FIFO")) {
                    spp->swappingPolicy = FIFO;
                } else if (compareKey(policy, "FirstFit")) {
                    spp->swappingPolicy = FirstFit;
                } else {
                    fprintf(stderr, "Error: Invalid SwappingPolicy: %s\n", policy);
                    errorOccurred = true;
                }
                continue;
            }

            fprintf(stderr, "Error parsing line: %s\n", trimmed);
            errorOccurred = true;
        }

        // se nenhum destes parâmetros foi definido por isso a load vai ser chamada com o ficheiro de onde lemos as configs
        if (!jobCountRead && !jobRandomSeedRead) {
            spp->jobLoadStream = fin;
        }

        // Check for semantic errors
        if (spp->memorySize <= spp->memoryKernelSize) {
            fprintf(stderr, "Error: MemorySize must be greater than MemoryKernelSize\n");
            errorOccurred = true;
        }
        if (jobCountRead && (spp->jobCount < 1 || spp->jobCount > MAX_JOBS)) {
            fprintf(stderr, "Error: JobCount must be >= 1 and <= %d\n", MAX_JOBS);
            errorOccurred = true;
        }

        // Throw exception if there were errors
        if (errorOccurred) {
            throw Exception(EINVAL, "simConfig: error parsing configuration file");
        }

    }

    // ================================================================================== //

} // end of namespace group