/* ------------------------------------------------------------- */
/* Messages between Scheduler and CPU are always 3 integers long */
/* ------------------------------------------------------------- */

#include <unistd.h>
#include <assert.h>

#ifndef COMMUNICATIONS_H

#include "communications.h"

#endif

/* toCPU: we send:
 - the cpu number (for verification by the CPU receiving function)
 - the jID job ID or the EOSIM tag
 - the job duration (expected = real in this simulation). N.A on EOSIM
*/
void toCPU(int cpu, int jID, int jDuration) {
    int msg[3];
    msg[0] = cpu;
    msg[1] = jID;
    msg[2] = jDuration;
    int nWrite = write(pairs[cpu].wrQ[0], msg, sizeof(msg));
    assert(nWrite == sizeof(msg));
}

/* fromCPU: receive an EOSIM reply message (from the CPU process)
 parameter:
 - the cpu number (we are receiving from)
 return:
 - the cpu number (we have received) FAILS if they do not match!
 - the jID value must be EOSIM
 - the duration - ammount of work performed by the CPU process
*/
void fromCPU(int cpu, int *cpuID, int *jID, int *jDuration) {
    int msg[3];
    int nRead;
    nRead = read(pairs[cpu].rdQ[1], msg, sizeof(msg));
    assert(nRead == 3 * sizeof(int));
    assert(msg[0] == cpu);
    *cpuID = msg[0];
    *jID = msg[1];
    *jDuration = msg[2];
}
