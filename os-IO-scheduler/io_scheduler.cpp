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

				IO newIO = new IO(IO_ID, trackNum);
				Event newEvent = new Event(timeStamp, ADD, newIO);
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
	int curTime = 0;
	int curTrack = 0;
	int lastEventFinish = 0;

	while((curEvent = events->getEvent())){
		curTime = curEvent->getTimestamp();
		
		delete curEvent; //free memory
	}

	return lastEventFinish;
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
			else if(optarg[0] == 'f'){ scheduler = new fScan_Scheduler(); }
		} 
		else if (opt == 'v') {
			verbose = true;
		} else {
			cout << "Invalid flag " << opt << endl;
			exit 1;
		}
	}

	string filename = argv[optind];
	
	EventList * events = new EventList();
	vector<IO *> * allIOs = new vector<IO *>();

	initialize(filename, events, allIOs);
	int total_time = simulate(events, scheduler, allIOs, verbose);

	return 0;
}