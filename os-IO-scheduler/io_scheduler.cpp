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

int main(int argc, char **argv){
	int opt;
	int total_time = 0;

	while ((opt = getopt (argc, argv, "s:")) != -1) {
		if (opt == 's') {

		} else {
			cout << "Invalid flag " << opt << endl;
			exit 1;
		}
	}

	return 0;
}