
#define COMMUNICATIONS_H

#ifndef GLOBALS_H
#include "globals.h"
#endif

/* Use this struct as a template for 2 pipes for 1 CPU */
struct CPUqueuePair {
  int rdQ[2];
  int wrQ[2];
};
//structure based on struct CPUqueuePair that support more than 1 CPU
struct CPUqueuePair pairs[MAX_CPUs];


/* **
  Prototypes of communications functions used by the Scheduler program
** */
void toCPU(int cpu, int jID, int jDuration);

void fromCPU(int cpu, int *cpuID, int *jID, int *jDuration);
