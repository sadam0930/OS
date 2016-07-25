#ifndef event_header
#define event_header

#include <string>
#include "io.h"

#ifndef nullptr
#define nullptr __null
#endif

typedef enum operation {
	ADD,
	ISSUE,
	FINISH
} operation;

class Event {
	int timestamp;
	operation op;

	public:
		Event * nextEvent;
		IO * io;

		Event(){}
		Event(int timestamp, operation op, IO * io){
			setTimestamp(timestamp);
			setOperation(op);
			this->io = io;
		}
		void setTimestamp(int timestamp){ this->timestamp = timestamp; }
		void setOperation(operation op){ this->op = op; }
		int getTimestamp(){ return this->timestamp; }
		operation getOperation(){ return this->op; }

		std::string opToString(){
			std::string returnString = "";

			switch(this->op){
				case ADD:
					returnString = "add";
					break;
				case ISSUE:
					returnString = "issue";
					break;
				case FINISH:
					returnString = "finish";
					break;
			}

			return returnString;
		}
};

class EventList {
	Event * head;
	int numEvents;

	public:
		EventList() {
			head = nullptr;
			numEvents = 0;
		}
		~EventList(){
			if(numEvents > 0){
				Event * cur = head;
				Event * next;
				do {
					next = cur->nextEvent;
					delete cur;
					cur = next;
				} while(cur);
			}
		}

		//sorted insert
		void putEvent(int timestamp, operation op, IO * io){
			Event * newEvent = new Event(timestamp, op, io);
			Event * cur;

			if(this->isEmpty() || head->getTimestamp() > newEvent->getTimestamp()){
				newEvent->nextEvent = head;
				head = newEvent;
			} else {
				cur = head;
				while(cur->nextEvent && cur->nextEvent->getTimestamp() <= newEvent->getTimestamp()){
					cur = cur->nextEvent;
				}
				newEvent->nextEvent = cur->nextEvent;
				cur->nextEvent = newEvent;
			}
			numEvents++;
		}

		//memory leak! remember to delete event from scheduler
		Event * getEvent(){
			Event * curEvent;
			if(this->isEmpty()){
				curEvent = nullptr;
			} else {
				curEvent = head;
				head = head->nextEvent;
				numEvents--;
			}
			return curEvent;
		}

		int getNextTimestamp(){
			if(numEvents == 0){
				return -1;
			} else {
				return head->getTimestamp();
			}
		}

		bool isEmpty(){
			if(numEvents == 0){
				return true;
			} else {
				return false;
			}
		}
};

#endif