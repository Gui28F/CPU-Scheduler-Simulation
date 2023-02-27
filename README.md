# CPU-Scheduler-Simulation

In this project I create a system that will take a set of input data lines, that represent
jobs to be ran in a simulated batch system. Below, you can see a short example of the data:<br>
J 1 R 30<br>
J 2 R 21<br>
J 3 R 22<br>
... ...<br>
In this example, job with job ID 1 (the J 1) is expected to run for 30 units (say, seconds) of time (the R 30);
all the other lines have the same format. The number of lines is some n, where 1 <= n <= MAX_JQ_SIZE.<br>
The “system” is composed of two distinct programs: the JobSch and the CPU. The JobSch is the
“master” program, i.e., the program that you start at your terminal, as depicted below 
$ ./JobSch F 1 jobData.txt (1)<br>
In the example above the F stands for FIFO, i.e., the policy the scheduler will use to run the jobs will be
FIFO; if that character was an S, not an F, that would stand for a SJF policy. The 1 means that the simulated
“computer” will have only one CPU (the maximum number of CPUs is 4), and jobData.txt is
the name of the data file for the simulation. So, the execution line will always have a total of 3 arguments
(not counting the program name).<br>
There is a slightly different way to execute the JobSch program, typing “./JobSch F 1 -”. What the
above dash (-) symbol means is that the input will be read from stdin; that is, the execution of the
following command,
$ cat jobData.txt | ./JobSch F 1 -
is equivalent to the above execution at (1).
