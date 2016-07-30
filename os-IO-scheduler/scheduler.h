#ifndef scheduler_header
#define scheduler_header

#include <climits>
#include "io.h"

#ifndef nullptr
#define nullptr __null
#endif

typedef enum Direction {
	UP,
	DOWN
} Direction;

Direction swapDirection(Direction curDirection){
	if(curDirection == UP){
		return DOWN;
	} else {
		return UP;
	}
}

class Scheduler {
	public:
		std::vector<IO *> * runQueue;
		std::vector<IO *> * otherRunQueue; //just for fScan
		Direction direction;

		Scheduler(){
			this->runQueue = new std::vector<IO *>();
			this->otherRunQueue = new std::vector<IO *>();
			this->direction = UP;
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

//Scan -- really look
class Scan_Scheduler : public Scheduler {
	public:
		Scan_Scheduler(){}

		//inserted sort based on trackNum
		void putIO(IO * io){
			int put_idx = runQueue->size();

			for(int i = 0; i < runQueue->size(); i++){
				if(runQueue->at(i)->trackNum > io->trackNum){
					put_idx = i;
					break;
				}
			}

			runQueue->insert(runQueue->begin()+put_idx, io);
		}

		IO * getNextIO(int curTrack){
			IO * returnIO;

			if(runQueue->empty()){
				returnIO = nullptr;	
			} else {
				int idx = elevator_helper(curTrack, false, false);
				returnIO = runQueue->at(idx);
				runQueue->erase(runQueue->begin()+idx);
			}

			return returnIO;
		}

		int elevator_helper(int curTrack, bool runUpAlready, bool runDownAlready){
			int rm_idx = -1;

				if(direction == UP && !runUpAlready){
					for(int i = 0; i < runQueue->size(); i++){
						if(runQueue->at(i)->trackNum < curTrack){
							//we are past these
							continue;
						} else {
							rm_idx = i;
							break; //it's sorted already so the first found is the closest
						}
					}
					if(rm_idx == -1){
						direction = swapDirection(direction);
						runUpAlready = true;
					}
				}

				if(direction == DOWN && !runDownAlready){
					for(int i = runQueue->size()-1; i >= 0; i--){
						if(runQueue->at(i)->trackNum > curTrack){
							continue;
						} else {
							rm_idx = i;
							break;
						}
					}

					if(rm_idx == -1){
						direction = swapDirection(direction);
						runDownAlready = true;
					}
				}

				if(rm_idx == -1){
					rm_idx = elevator_helper(curTrack, runUpAlready, runDownAlready);
				}

			return rm_idx;
		}
};

//cscan -- really clook
class cScan_Scheduler : public Scheduler {
	public:
		cScan_Scheduler(){}

		//inserted sort based on trackNum
		void putIO(IO * io){
			int put_idx = runQueue->size();

			for(int i = 0; i < runQueue->size(); i++){
				if(runQueue->at(i)->trackNum > io->trackNum){
					put_idx = i;
					break;
				}
			}

			runQueue->insert(runQueue->begin()+put_idx, io);
		}

		IO * getNextIO(int curTrack){
			IO * returnIO;

			if(runQueue->empty()){
				returnIO = nullptr;	
			} else {
				int rm_idx = 0;

				for(int i = 0; i < runQueue->size(); i++){
					if(runQueue->at(i)->trackNum < curTrack){
						//we are past these
						continue;
					} else {
						rm_idx = i;
						break; //it's sorted already so the first found is the closest
					}
				}

				returnIO = runQueue->at(rm_idx);
				runQueue->erase(runQueue->begin()+rm_idx);
			}

			return returnIO;
		}
};

//fscan -- really flook
class fScan_Scheduler : public Scheduler {
	public:
		fScan_Scheduler(){}

		//inserted sort based on trackNum
		void putIO(IO * io){
			int put_idx = otherRunQueue->size();

			for(int i = 0; i < otherRunQueue->size(); i++){
				if(otherRunQueue->at(i)->trackNum > io->trackNum){
					put_idx = i;
					break;
				}
			}

			otherRunQueue->insert(otherRunQueue->begin()+put_idx, io);
		}

		IO * getNextIO(int curTrack){
			IO * returnIO;
			
			if(runQueue->empty() && otherRunQueue->empty()){
				returnIO = nullptr;	
			} else {
				if(runQueue->empty()){
					std::vector<IO *> * temp = runQueue;
					runQueue = otherRunQueue;
					otherRunQueue = temp;
					direction = UP; //when switching queues in FSCAN you always scan up first from the current position, then down until queue empty
				}

				int idx = elevator_helper(curTrack, false, false);
				returnIO = runQueue->at(idx);
				runQueue->erase(runQueue->begin()+idx);
			}
			
			return returnIO;
		}

		int elevator_helper(int curTrack, bool runUpAlready, bool runDownAlready){
			int rm_idx = -1;
			
			if(direction == UP && !runUpAlready){
				for(int i = 0; i < runQueue->size(); i++){
					if(runQueue->at(i)->trackNum < curTrack){
						//we are past these
						continue;
					} else {
						rm_idx = i;
						break; //it's sorted already so the first found is the closest
					}
				}
				if(rm_idx == -1){
					direction = swapDirection(direction);
					runUpAlready = true;
				}
			}

			if(direction == DOWN && !runDownAlready){
				for(int i = runQueue->size()-1; i >= 0; i--){
					if(runQueue->at(i)->trackNum > curTrack){
						continue;
					} else {
						rm_idx = i;
						break;
					}
				}

				if(rm_idx == -1){
					direction = swapDirection(direction);
					runDownAlready = true;
				}
			}

			if(rm_idx == -1){
				rm_idx = elevator_helper(curTrack, runUpAlready, runDownAlready);
			}

			return rm_idx;
		}
};

#endif