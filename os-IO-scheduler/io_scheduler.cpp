#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <cstring>

#include "scheduler.h" //algorithms
#include "io.h"
#include "event.h"

#ifndef nullptr
#define nullptr __null
#endif

using namespace std;

//global
int total_time = 0;
int tot_movement = 0;

void initialize(string filename, EventList * events, vector<IO *> * allIOs){
	ifstream f;
	f.open(filename);

	if(f.is_open()){
		int IO_ID = 0;
		string instruction;
		//Each line in the file is an instruction
		while(getline(f, instruction)){
			istringstream iss(instruction);
			if(instruction[0] == '#'){
				//ignore commented lines
				continue;
			} else {
				//First int is the timestamp; Second int is track # to read
				int timeStamp, trackNum = 0;
				iss >> timeStamp >> trackNum;

				IO * newIO = new IO(IO_ID, trackNum, timeStamp);
				events->putEvent(timeStamp, newIO, ADD);
				allIOs->push_back(newIO);

				IO_ID++;
			}
		}
	} else {
		cout << "Could not open file: " << filename << endl;
		exit(1);
	}
	f.close();
}

void printVerbose(int curTime, int curTrack, Event * curEvent){
	cout << curTime << setfill(' ') << setw(6) << curEvent->io->IO_ID << " ";
	cout << curEvent->opToString();
	if(curEvent->getOperation() != FINISH){
		cout << " " << curEvent->io->trackNum;
	} else {
		cout << " " << curEvent->io->TT;
	}
	if(curEvent->getOperation() == ISSUE){
		cout << " " << curTrack;
	}
	cout << endl;
}

void simulate(EventList * events, Scheduler * scheduler, vector<IO *> * allIOs, bool	verbose){
	int curTime = 0;
	int curTrack = 0;
	Direction direction = UP;

	Event * curEvent;
	bool callScheduler = false;
	IO * curRunningIO = nullptr;

	while((curEvent = events->getEvent())){
		curTime = curEvent->getTimestamp();
		curEvent->io->timeInPreviousState = curTime - curEvent->io->stateTimeStamp;

		switch(curEvent->getOperation()){
			case ADD:
				scheduler->putIO(curEvent->io);
				callScheduler = true;
				break;
			case ISSUE:
				curEvent->io->WT = curTime - curEvent->io->AT;
				if(curEvent->io->trackNum > curTrack) {
					direction = UP;
					events->putEvent((curTime + curEvent->io->trackNum - curTrack), curEvent->io, FINISH);
				} else {
					direction = DOWN;
					events->putEvent((curTime + curTrack - curEvent->io->trackNum), curEvent->io, FINISH);
				}
				break;
			case FINISH:
				//tally total number of tracks the head had to be moved
				if(direction == UP){
					tot_movement += curEvent->io->trackNum - curTrack;
				} else {
					tot_movement += curTrack - curEvent->io->trackNum;
				}
				curTrack = curEvent->io->trackNum;
				curEvent->io->TT = (curTime - curEvent->io->AT);
				//free current running IO
				curRunningIO = nullptr;
				callScheduler = true;
				total_time = curTime; //overwrite until it's over
				break;
		}

		if(verbose){ printVerbose(curTime, curTrack, curEvent); }

		delete curEvent; //free memory

		if(callScheduler){
			//process all events from the same timestamp first
			if(events->getNextTimestamp() == curTime){
				continue;
			} else {
				callScheduler = false;
				if(curRunningIO == nullptr){
					curRunningIO = scheduler->getNextIO(curTrack);
					// cout<< scheduler->runQueue->numIOs <<endl;
					if(curRunningIO){
						events->putEvent(curTime, curRunningIO, ISSUE);
					} //may need else if for one of the schedulers
				}
			}
		}
	}
}

void print_sum(vector<IO *> * allIOs, int total_time, int tot_movement){
	double avg_turnaround, avg_waittime;
	int max_waittime;
	avg_turnaround = avg_waittime = max_waittime = 0;

	unsigned long numIOs = allIOs->size();
	for (unsigned int i=0; i < numIOs; i++) {
		avg_turnaround += allIOs->at(i)->TT;
		avg_waittime += allIOs->at(i)->WT;

		if(allIOs->at(i)->WT > max_waittime){
			max_waittime = allIOs->at(i)->WT;
		}
	}

	avg_turnaround = (double)avg_turnaround/(double)numIOs;
	avg_waittime = (double)avg_waittime/(double)numIOs;

	printf("SUM: %d %d %.2lf %.2lf %d\n",
			total_time,
			tot_movement,
			avg_turnaround,
			avg_waittime,
			max_waittime); 
}

int main(int argc, char **argv){
	int opt;
	bool verbose;
	Scheduler * scheduler;

	while ((opt = getopt (argc, argv, "vs:")) != -1) {
		if (opt == 's') {
			if(optarg[0] == 'i'){ scheduler = new FIFO_Scheduler(); } 
			else if(optarg[0] == 'j'){ scheduler = new SSTF_Scheduler(); }
			else if(optarg[0] == 's'){ scheduler = new Scan_Scheduler(); }
			else if(optarg[0] == 'c'){ scheduler = new cScan_Scheduler(); }
			// else if(optarg[0] == 'f'){ scheduler = new fScan_Scheduler(); }
		} 
		else if (opt == 'v') {
			verbose = true;
		} else {
			cout << "Invalid flag " << opt << endl;
			exit(1);
		}
	}

	string filename = argv[optind];

	EventList * events = new EventList();
	vector<IO *> * allIOs = new vector<IO *>();
	initialize(filename, events, allIOs);
	simulate(events, scheduler, allIOs, verbose);
	print_sum(allIOs, total_time, tot_movement);

	return 0;
}