#ifndef scheduler_header
#define scheduler_header

#include <climits>
#include "io.h"

#ifndef nullptr
#define nullptr __null
#endif

class Scheduler {
	public:
		IOList * runQueue;
		Scheduler(){
			this->runQueue = new IOList();
		}
		//implementation will differ based on the scheduling algorithm
		virtual IO * getNextIO(int curTrack) =0;
		virtual void putIO(IO * io) =0;
};

/**********************************************************
differentiate how to getIO between scheduler algorithms
***********************************************************/

//First In First Out
class FIFO_Scheduler : public Scheduler {
	public:
		FIFO_Scheduler(){}

		//get IO from front of run queue
		IO * getNextIO(int curTrack){
			IO * returnIO;
			if(runQueue->numIOs == 0){
				returnIO = nullptr;
			} else {
				returnIO = runQueue->head;
				runQueue->head = runQueue->head->nextIO;
				(runQueue->numIOs)--;
			}
			return returnIO;
		}

		//add IO at end of run queue
		void putIO(IO * io){
			if(runQueue->isEmpty()){
				runQueue->head = io;
				runQueue->tail = runQueue->head;
			} else {
				io->prevIO = runQueue->tail;
				runQueue->tail->nextIO = io;
				runQueue->tail = io;
			}
			(runQueue->numIOs)++;
		}
};

// Shortest Seek Time First
class SSTF_Scheduler : public Scheduler {
	public:
		SSTF_Scheduler(){}

		IO * getNextIO(int curTrack){
			IO * returnIO;

			if(runQueue->isEmpty()){
				returnIO = nullptr;
			} else {
				IO * next = runQueue->head;
				int min, diff;
				min = 9999;

				do {
					diff = abs(next->trackNum - curTrack);
					if(diff < min){
						min = diff;
						returnIO = next;
					}
					next = next->nextIO;
				} while(next);
			}

			return returnIO;
		}

		//add IO at end of run queue
		void putIO(IO * io){
			if(runQueue->isEmpty()){
				runQueue->head = io;
				runQueue->tail = runQueue->head;
			} else {
				io->prevIO = runQueue->tail;
				runQueue->tail->nextIO = io;
				runQueue->tail = io;
			}
			(runQueue->numIOs)++;
		}
};

// //Scan -- really look
// class Scan_Scheduler : public Scheduler {
// 	public:
// 		Scan_Scheduler(){}

// 		IO * getNextIO(int curTrack){
// 			IO * returnIO;

// 			return returnIO;
// 		}

// 		void putIO(IO * io){

// 		}
// };

// //cscan -- really clook
// class cScan_Scheduler : public Scheduler {
// 	public:
// 		cScan_Scheduler(){}

// 		IO * getNextIO(int curTrack){
// 			IO * returnIO;

// 			return returnIO;
// 		}

// 		void putIO(IO * io){

// 		}
// };

// //fscan -- really flook
// class fScan_Scheduler : public Scheduler {
// 	public:
// 		fScan_Scheduler(){}

// 		IO * getNextIO(int curTrack){
// 			IO * returnIO;

// 			return returnIO;
// 		}

// 		void putIO(IO * io){

// 		}
// };

#endif