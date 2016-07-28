#ifndef scheduler_header
#define scheduler_header

#include <climits>
#include "io.h"

#ifndef nullptr
#define nullptr __null
#endif

class Scheduler {
	public:
		std::vector<IO *> * runQueue;

		Scheduler(){
			this->runQueue = new std::vector<IO *>();
		}
		//implementation will differ based on the scheduling algorithm
		virtual IO * getNextIO(int curTrack) =0;
		virtual void putIO(IO * io) =0;
};

/**********************************************************
differentiate how to getIO between scheduler algorithms
***********************************************************/

// First In First Out
class FIFO_Scheduler : public Scheduler {
	public:
		FIFO_Scheduler(){}

		//add IO at end of run queue
		void putIO(IO * io){
			runQueue->push_back(io);
		}

		IO * getNextIO(int curTrack){
			IO * returnIO;

			if(runQueue->empty()){
				returnIO = nullptr;	
			} else {
				returnIO = runQueue->front();
				runQueue->erase(runQueue->begin());
			}

			return returnIO;
		}
};

// Shortest Seek Time First
class SSTF_Scheduler : public Scheduler {
	public:
		SSTF_Scheduler(){}

		//add IO at end of run queue
		void putIO(IO * io){
			runQueue->push_back(io);
		}

		IO * getNextIO(int curTrack){
			IO * returnIO;

			if(runQueue->empty()){
				returnIO = nullptr;	
			} else {
				int diff, idx;
				int min = 9999;
				for(int i=0; i < runQueue->size(); i++){
					diff = abs(runQueue->at(i)->trackNum - curTrack);
					if(diff < min){
						min = diff;
						idx = i;
					}
				}
				returnIO = runQueue->at(idx);
				runQueue->erase(runQueue->begin()+idx);
			}

			return returnIO;
		}
};

// // First In First Out
// class FIFO_Scheduler : public Scheduler {
// 	public:
// 		FIFO_Scheduler(){}

// 		//get IO from front of run queue
// 		IO * getNextIO(int curTrack){
// 			IO * returnIO;
// 			if(runQueue->numIOs == 0){
// 				returnIO = nullptr;
// 			} else {
// 				returnIO = runQueue->head;
// 				runQueue->head = runQueue->head->nextIO;
// 				(runQueue->numIOs)--;
// 			}
// 			return returnIO;
// 		}

// 		//add IO at end of run queue
// 		void putIO(IO * io){
// 			if(runQueue->isEmpty()){
// 				runQueue->head = io;
// 				runQueue->tail = runQueue->head;
// 			} else {
// 				io->prevIO = runQueue->tail;
// 				runQueue->tail->nextIO = io;
// 				runQueue->tail = io;
// 			}
// 			(runQueue->numIOs)++;
// 		}
// };

// Shortest Seek Time First
// class SSTF_Scheduler : public Scheduler {
// 	public:
// 		SSTF_Scheduler(){}

// 		IO * getNextIO(int curTrack){
// 			IO * returnIO;

// 			if(runQueue->isEmpty()){
// 				returnIO = nullptr;
// 			} else {
// 				IO * next = runQueue->head;
// 				int min, diff;
// 				min = 9999;

// 				for(int i=0; i < runQueue->numIOs; i++){
// 					diff = abs(next->trackNum - curTrack);
// 					// std::cout << "diff " <<diff << " min " <<min << std::endl;
// 					if(diff < min){
// 						min = diff;
// 						returnIO = next;
// 						//when a min is found, move that IO to the tail
// 						if(i == 0){
// 							runQueue->head = runQueue->head->nextIO;
// 							runQueue->tail->nextIO = returnIO;
// 							returnIO->prevIO = runQueue->tail;
// 							runQueue->tail = returnIO;
// 						} else if(runQueue->numIOs > 1 && (i != runQueue->numIOs - 1)){
// 							returnIO->nextIO->prevIO = returnIO->prevIO;
// 							returnIO->prevIO->nextIO = returnIO->nextIO;
// 							runQueue->tail->nextIO = returnIO;
// 							returnIO->prevIO = runQueue->tail;
// 							runQueue->tail = returnIO;
// 						} else {
// 							//do nothing, it's already at the tail
// 						}
// 					}
// 					next = next->nextIO;
// 				}
// 			}

// 			//the lowest track diff should be at the end of the queue
// 			returnIO = runQueue->tail;
// 			if(runQueue->numIOs > 1){
// 				runQueue->tail = runQueue->tail->prevIO;
// 			} else if (runQueue->numIOs == 1){
// 				runQueue->head = runQueue->tail = nullptr;
// 			}
// 			(runQueue->numIOs)--;

// 			return returnIO;
// 		}

// 		//add IO at end of run queue
// 		void putIO(IO * io){
// 			if(runQueue->isEmpty()){
// 				runQueue->head = io;
// 				runQueue->tail = runQueue->head;
// 			} else {
// 				io->prevIO = runQueue->tail;
// 				// std::cout << "hello" << std::endl;
// 				runQueue->tail->nextIO = io;
// 				runQueue->tail = io;
// 			}
// 			(runQueue->numIOs)++;
// 		}
// };

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