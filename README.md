# SRT-Divider
Sweeney-Robertson-Toucher (SRT) Divider with RCA
------------------------------------------------------
SRT-Div is a program that simulates an SRT divider which
shiftes over 0's and 1's and uses an ripple carry
add/subtractor.

The simulator begins with 8-bit numerator/denominator,
performs the shift and add/subtract operations until
quotient is found and then repeats for MAX_RUNS. Efficiency
is measured as number of bits divided by number of add/subtract 
operations (#bits/#addSubs) for each run and is then averaged
across all runs. The process is repeated for 
10,12,14,...,64 (MAX_BITS) bits.

Compilation and execution is straight forward but a 
usage file is included in the repo.

List of files in this repo:

--> usage.txt :              Description of how to compile and run

--> SRT-Div.c :              Simulation source file

--> SRT-Div :                SRT Divider binary

--> SRT-divider-diagram.png: Divider operation diagram

--> run.txt :                Typical output

--> README.md :              This readme file

