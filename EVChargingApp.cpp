#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <queue>
#include <map>
#include <iomanip>
#include <ctime>
#include <limits>


using namespace std;

#include "Location.h"
#include "WeightedGraph.h"
#include "EVCharging.h"

void runAgain();

int main() {
    srand(time(0));
	EVCharging charging;
	charging.run();
	runAgain();
	return 0;
}

void runAgain(){
	cout << endl << "Run program again? [y][n]";
	char runAgain;
	cin >> runAgain;

	if(runAgain == 'y' || runAgain == 'Y'){
		main();
	}
}