#ifndef io_header
#define io_header

#ifndef nullptr
#define nullptr __null
#endif

typedef enum IOState {
	CREATED,
	READY,
	ISSUED,
	DONE
} IOState;

class IO {
	public:
		int IO_ID, trackNum;
		IOState currentState;
		int timeInPreviousState;
		int stateTimeStamp;
		int AT, TT, WT; //arrival time, turnaround time, wait time
		//Each IO is a node in a linked list
		IO * nextIO;
		IO * prevIO;

		IO(int IO_ID, int trackNum, int arrivalTime){
			this->prevIO = nullptr;
			this->nextIO = nullptr;
			this-> IO_ID = IO_ID;
			this->trackNum = trackNum;
			this->timeInPreviousState = 0;
			this->currentState = CREATED;
			this->stateTimeStamp = arrivalTime;
			this->AT = arrivalTime;
			this->TT = 0;
		}

};

#endif