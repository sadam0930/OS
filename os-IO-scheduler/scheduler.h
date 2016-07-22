#ifndef scheduler_header
#define scheduler_header

#include "io.h"

#ifndef nullptr
#define nullptr __null
#endif

class Scheduler {
	public:
		Scheduler()

		virtual IO getNextIO(){} =0;
		virtual void putIO(){} =0;
};

#endif