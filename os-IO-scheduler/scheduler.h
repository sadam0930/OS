#ifndef scheduler_header
#define scheduler_header

#include "io.h"

#ifndef nullptr
#define nullptr __null
#endif

class Scheduler {
	public:
		Scheduler()

		virtual IO * getNextIO(){} =0;
		virtual void putIO(IO * io){} =0;
};

/**********************************************************
differentiate how to getProcess between scheduler algorithms
***********************************************************/

//First In First Out
class FIFO_Scheduler : public Scheduler {
	public:
		FIFO_Scheduler(){}

		IO * getNextIO(){
			IO * returnIO;

			return returnIO;
		}

		void putIO(IO * io){

		}
};

//Shortest Seek Time First
class SSTF_Scheduler : public Scheduler {
	public:
		SSTF_Scheduler(){}

		IO * getNextIO(){
			IO * returnIO;

			return returnIO;
		}

		void putIO(IO * io){

		}
};

//Scan -- really look
class Scan_Scheduler : public Scheduler {
	public:
		Scan_Scheduler(){}

		IO * getNextIO(){
			IO * returnIO;

			return returnIO;
		}

		void putIO(IO * io){

		}
};

//cscan -- really clook
class cScan_Scheduler : public Scheduler {
	public:
		cScan_Scheduler(){}

		IO * getNextIO(){
			IO * returnIO;

			return returnIO;
		}

		void putIO(IO * io){

		}
};

//fscan -- really flook
class fScan_Scheduler : public Scheduler {
	public:
		fScan_Scheduler(){}

		IO * getNextIO(){
			IO * returnIO;

			return returnIO;
		}

		void putIO(IO * io){

		}
};

#endif