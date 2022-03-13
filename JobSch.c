#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <unistd.h>
//include <sys/wait.h>

// Global definitions
#ifndef GLOBALS_H
#include "globals.h"
#endif

// Definitions for scheduler/cpu communications
#ifndef COMMUNICATIONS_H
#include "communications.h"
#endif

// Definitions for scheduler data structures
#ifndef SCHEDULER_H
#include "scheduler.h"
#endif



/* -------------------------------------------- */
void createCPUs( int nCPUs ) {

  char cpuID[2];	// will contain the CPU number when exec'ing the
			// CPU program. In the exec(), that number is a char

  int pip[2];
	
  for (int cpu= 0; cpu < nCPUs; cpu++) {
	cpuID[0] = '0' + cpu;
	cpuID[1] = '\0';				
	for(int i = 0; i < 2; i++){
		if(pipe(pip) == -1){
			perror("ERRO PIP\n");	
			abort();
		}
		pairs[cpu].rdQ[i] = pip[0];
		pairs[cpu].wrQ[i] = pip[1];
	}
	int pid = fork();
	if(pid){
		close(pairs[cpu].rdQ[0]);
		close(pairs[cpu].wrQ[1]);
	}else{
	   	for(int i = 0; i <= cpu;i++){
			close(pairs[i].rdQ[1]);
			close(pairs[i].wrQ[0]);
		  }
		close(0);
		dup(pairs[cpu].rdQ[0]);
		close(pairs[cpu].rdQ[0]);
		close(1);
		dup(pairs[cpu].wrQ[1]);
		close(pairs[cpu].wrQ[1]);
		execl("./CPU", "CPU", cpuID, NULL);
		perror("EXEC FAILS");
		exit(1);
	}
  }
}

#define MAX(x,y) (x > y?x:y)

void report(int nCPUs, struct cpuInfo cpuData[]) {

  int cpuID, jID, jDuration, totConsumed= 0, totalSimulationTime= 0;

  for (int cpu= 0; cpu < nCPUs; cpu++) {

    fromCPU(cpu, &cpuID, &jID, &jDuration);

    cpuData[cpu].usage+= jDuration; totConsumed+= jDuration;
    totalSimulationTime=MAX(totalSimulationTime,cpuData[cpu].usage);

    printf("CPU[%d] usage= %d\n", cpu, cpuData[cpu].usage);
  }
  printf("Total Simulation Time=%d\n", totalSimulationTime);
  printf("Average CPU consumed for %d CPUs= %.1f%%\n",\
	 nCPUs, 100*(float)totConsumed/totalSimulationTime);
}


int main(int argc, char *argv[]) {

  char policy=argv[1][0];	// F or S
  int nCPUs= atoi(argv[2]);	// 1 or > 1, up to 4
  char *filename= argv[3];	// name of the file with Job data
				// or the character "-" if reading from stdin

  int l, cpu;

  struct cpuInfo cpuData[nCPUs];
  memset(cpuData, 0, sizeof(struct cpuInfo)*nCPUs);

  struct jobQ jobQueue[MAX_JQ_SIZE];
  memset(jobQueue, 0, sizeof(struct jobQ)*MAX_JQ_SIZE);

  createCPUs(nCPUs);

  int lines=  readJobSubmission(filename, jobQueue);

    if (policy == 'S'){
	//Bubble Sort algorithm
	for (int i = 1; i < lines ; i++) {
    		for (int j = lines-1; j >= i;  j--) {
      			if (jobQueue[j-1].duration > jobQueue[j].duration) {
        			struct jobQ temp = jobQueue[j-1];
        			jobQueue[j-1] = jobQueue[j];
        			jobQueue[j] = temp;
      			}
    		}
  	}
    }
    

 
  // Schedule the jobs (FIFO version, assumes job runtime == duration)
  l= 0; cpu= 0;
  while (l < lines) {
    printf("jQ[%d] jID %d run %d\n", l, jobQueue[l].jID,\
	     jobQueue[l].duration); 
    dispatch(l, jobQueue, cpu, cpuData);
    l++;
    cpu= (cpu+1) % nCPUs;
  }

  // send SIMulation END messages (EOSIM) to CPUs
  for (int cpu= 0; cpu < nCPUs; cpu++)  toCPU(cpu, EOSIM, EOSIM);

  // Get the replies from each CPU, compute and print results
  report(nCPUs, cpuData);

wait(NULL);
  return 0;
}

