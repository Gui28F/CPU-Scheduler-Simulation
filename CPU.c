/* ---------------------------------------------- 
   This program should be exec'ed by the Scheduler
   messages must be:
    - received on stdin
    - sent on stdout
 ------------------------------------------------ */


/* ------------------------------------------------------------- */
/* Messages between Scheduler and CPU are always 3 integers long */
/* ------------------------------------------------------------- */

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>

#ifndef GLOBALS_H
#include "globals.h"
#endif

int myCPUid;	// my CPU id, derived from argv[1]

/* fromSCH: receive a message (from the Scheduler process)
 return:
 - the cpu number (for verification by the CPU receiving function)
 - the job ID
 - the job duration (expected = real in this simulation)
*/
void fromSCH(int *cpuID, int *jID, int *jDuration)
{

 int msg[3];
  int nRead;
   nRead  = read(0, msg, sizeof(msg));
   assert(nRead == 3*sizeof(int));
  *cpuID = msg[0];
  assert(*cpuID == myCPUid);
  *jID = msg[1];
   *jDuration = msg[2];
}

/* toSCH: send a message (to the Scheduler process)
 parameters:
 - the cpu number (i.e., myCPUid or another variable with the same value)
 - the EOSIM tag
 - the ammount of work this CPU has done, i.e., SUM of all durations
*/
void toSCH(int cpu, int jID, int jDuration)
{
    int msg[3];
  msg[0] = cpu;
  msg[1] = jID;
  msg[2] = jDuration;
  int writeReturn = write(1, msg, sizeof(msg));
  assert(writeReturn != -1);	
}

/* ---------------------------------------------- 
   This program should be exec'ed by the Scheduler
   messages must be:
    - received on stdin
    - sent on stdout
 ------------------------------------------------ */
int main(int argc, char *argv[]) {

  int cpuID, jID, jDuration;
  unsigned int CPUconsumed= 0;

  myCPUid= atoi(argv[1]);

  for (;;) {
    fromSCH(&cpuID, &jID, &jDuration);

    if (jID == EOSIM) break;
     usleep(1000*jDuration);

    CPUconsumed+= jDuration;
  }

  toSCH(cpuID, EOSIM, CPUconsumed);
}
