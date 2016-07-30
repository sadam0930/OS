# os-io-scheduler
-------------

# To compile:
`make`

without makefile : g++ -std=c++0x -Wall -o iosched io_scheduler.cpp

# To run:
`./runit.sh myoutput ./iosched`


Synopsis:  
This is a discrete event simulation of an operating system IO scheduler. It takes an input file with information about IOs to schedule and can run several scheduling algorithms: First Come First Serve, Shortest Seek Time First, Scan(Look), CScan(CLook), fScan(fLook)

A process is characterized by 2 parameters:
*Arrival Time (AT) 
*Track Number