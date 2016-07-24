#ifndef io_header
#define io_header

#ifndef nullptr
#define nullptr __null
#endif

class IO {
	public:
		int IO_ID, trackNum;
		//Each IO is a node in a linked list
		IO * nextIO;
		IO * prevIO;

		IO(int IO_ID, int trackNum){
			this-> IO_ID = IO_ID;
			this->trackNum = trackNum;
		}

};

class IOList {
	public:
		IO * head;
		IO * tail;
		int numIOs;

		IOList(){
			head = nullptr;
			tail = head;
			numIOs = 0;
		}
		~IOList(){
			if(numIOs > 0){
				IO * cur = head;
				IO * next;
				do {
					next = cur->nextIO;
					delete cur;
					cur = next;
				} while(cur);
			}
		}

		IO * findIO(int IO_ID){
			IO * foundIO = nullptr;
			IO * cur = head;

			while(cur){
				if(cur->IO_ID == IO_ID){
					foundIO = cur;
					break;
				}
				cur = cur->nextIO;
			}

			return foundIO;
		}

		bool isEmpty(){
			if(numIOs == 0){
				return true;
			} else {
				return false;
			}
		}
};

#endif