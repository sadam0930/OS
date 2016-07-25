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
				events->putEvent(timeStamp, ADD, newIO);
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

int simulate(EventList * events, Scheduler * scheduler, vector<IO *> * allIOs, bool	verbose){
	int curTime = 1;
	int curTrack = 0;
	int lastEventFinish = 0;
	Event * curEvent;
	bool callScheduler = false;
	IO * curRunningIO = nullptr;

	while((curEvent = events->getEvent())){
		curTime = curEvent->getTimestamp();
		curEvent->io->timeInPreviousState = curTime - curEvent->io->stateTimeStamp;

		switch(curEvent->getOperation()){
			case ADD:

				callScheduler = true;
				break;
			case ISSUE:

				break;
			case FINISH:

				callScheduler = true;
				break;
		}

		if(verbose){ cout << curTime << setfill(' ') << setw(6) << curEvent->io->IO_ID << " " << curEvent->opToString() << " " << curEvent->io->trackNum << endl; }

		delete curEvent; //free memory

		if(callScheduler){
			//process all events from the same timestamp first
			if(events->getNextTimestamp() == curTime){
				continue;
			} else {
				//toDo: get next IO and run it
			}
		}
	}

	return lastEventFinish;
}

void print_sum(vector<IO *> * allIOs, int total_time){

}

int main(int argc, char **argv){
	int opt;
	bool verbose;
	Scheduler * scheduler;

	while ((opt = getopt (argc, argv, "vs:")) != -1) {
		if (opt == 's') {
			if(optarg[0] == 'i'){ scheduler = new FIFO_Scheduler(); } 
			// else if(optarg[0] == 'j'){ scheduler = new SSTF_Scheduler(); }
			// else if(optarg[0] == 's'){ scheduler = new Scan_Scheduler(); }
			// else if(optarg[0] == 'c'){ scheduler = new cScan_Scheduler(); }
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
	int total_time = simulate(events, scheduler, allIOs, verbose);
	print_sum(allIOs, total_time);

	return 0;
}