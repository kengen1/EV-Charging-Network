#ifndef EVCHARGING_H_
#define EVCHARGING_H_

class EVCharging {// you may also declare this class as an extension of WeightedGraph
private:
	map<int, Location> locations;//It can also be unordered_map
	int numberOfLocations;
	WeightedGraphType* weightedGraph;
	int task;
    priority_queue<Location> locationsQueue;
	double travelCost = 0.10;
	double chargeAmount = 10;
	double cheapest = 1000;
	double cost = 0;
	string cheapestLocation;
	string taskLocation;



public:
	EVCharging();
	~EVCharging();

	void inputLocations();
	void printLocations();
	void printAdjacencyMatrix();
	void printLocationsWithCharging();	//task 3
	void printAdjacentLocations();		//task 4
	void cheapestAdjacent();			//task 5
	void shortestPath();				//task 6
	void lowestCost();					//task 7
	void originToDestination();			//task 8
	void multipleChargingLocations();	//task 9

	void run();
	int getTask();
	bool isValidTask(int);
};

EVCharging::EVCharging() {
	inputLocations();
	weightedGraph = new WeightedGraphType(numberOfLocations);
}

EVCharging::~EVCharging() {
	delete weightedGraph;
}

void EVCharging::inputLocations() {
	ifstream infile;
	char fileName[50] = "Locations.txt";

//	cout << "Enter charging location file name: ";
//	cin >> fileName;
//	cout << endl;

	infile.open(fileName);

	if (!infile) {
		cout << "Cannot open input file." << endl;
		return;
	}

	int locationIndex = 0;

	while (!infile.eof()) {
		Location s;
		string charger;
		string price;
		while (!infile.eof()) {
			getline(infile, s.locationName, ',');
			getline(infile, charger, ',');
			getline(infile, price);
			s.chargerInstalled = (stoi(charger) == 1) ? true : false;
			s.chargingPrice = stod(price);
			s.index = locationIndex;
			locations[locationIndex] = s;
			locationIndex++;
		}
	}

	numberOfLocations = locationIndex;
}

void EVCharging::printLocations() {
	cout << "List of locations and charging information " << endl;
	cout << setw (8) << "Index" << setw (20) << "Location name" << setw (20) <<"Charging station" << setw(20) << "Charging price" << endl;

	map<int, Location>::iterator it = locations.begin();

	for (; it != locations.end(); it++) {
		it->second.printLocation();
	}

    cout << endl;
}

void EVCharging::printAdjacencyMatrix() {
	cout << "Adjacency matrix (0 means no direct connection, non-zero value represents the distance of adjacent locations)\n" << endl;
	cout << setw(14) << " ";
	for (int i = 0; i < numberOfLocations; i++) {
		cout << setw(14) << locations[i].locationName;
	}
	cout << endl;
	for (int i = 0; i < numberOfLocations; i++) {
		cout << setw(14) << locations[i].locationName;
		for (int j = 0; j < numberOfLocations; j++) {
			cout << setw(14) << (weightedGraph->getWeight(i,j) == DBL_MAX ? 0.0 : weightedGraph->getWeight(i,j));
		}
		cout << endl;
	}
}

void EVCharging::printLocationsWithCharging(){
	map<int, Location>::iterator it = locations.begin();

	for(; it != locations.end(); it++){
		if(it->second.chargerInstalled){
			locationsQueue.push(it->second);
		}
	}

	while(!locationsQueue.empty()){
		Location top = locationsQueue.top();
		top.printLocation();
		locationsQueue.pop();
	}
}

void EVCharging::printAdjacentLocations(){

	cout << "Task 4: List adjacent charging stations. " << endl << "Input a location: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Read the input using getline()
    std::getline(std::cin, taskLocation);

	map<int, Location>::iterator it = locations.begin();
	bool adjExist = false;

	for (; it != locations.end(); it++) {
		if(it->second.locationName == taskLocation){
			list<int> adjList = weightedGraph->getAdjancencyList(it->second.index);
			for(const auto& num: adjList){
				if(locations[num].chargerInstalled){
					cout << locations[num].locationName << endl;
					adjExist = true;
				}
			}
			if(!adjExist){
				cout << "NONE" << endl;
			}
		}
	}
}

void EVCharging::cheapestAdjacent(){

	cout << "Task 5: Find the adjacent charging stations with lowest charging cost. " << endl << "Input a location: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Read the input using getline()
    std::getline(std::cin, taskLocation);

	map<int, Location>::iterator it = locations.begin();
	bool adjExist = false;

	for (; it != locations.end(); it++) {				//iterate through map of locations
		if(it->second.locationName == taskLocation){
			list<int> adjList = weightedGraph->getAdjancencyList(it->second.index);
			chargeAmount = rand() % 41 + 10;
			//chargeAmount = 20;
			for(const auto& num: adjList){				//iterate through list
				if(locations[num].chargerInstalled){
					double distance = weightedGraph->getWeight(it->second.index, locations[num].index);
					cost = distance * 2 * travelCost + chargeAmount * locations[num].chargingPrice;
					if(locations[num].chargingPrice != 0){
						if(cost < cheapest){
							cheapest = cost;
							cheapestLocation = locations[num].locationName;
						}
						adjExist = true;
					}
					if(locations[num].chargingPrice == 0 && chargeAmount <= 25){
						if(cost < cheapest){
							cheapest = cost;
							cheapestLocation = locations[num].locationName;
						}
						adjExist = true;
					}
				}
			}
		}
	}
	if(adjExist){
		cout << "Charging amount: " << chargeAmount << "kWh" << endl;
		cout << "The cheapest next charging station is " << cheapestLocation << ". Charging cost = " << cheapest;
	}else{
		cout << "NO ADJACENCIES";
	}
}

void EVCharging::shortestPath(){

	cout << "Task 6: Find the nearest charging station. " << endl << "Input a location: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Read the input using getline()
    std::getline(std::cin, taskLocation);

	map<int, Location>::iterator it = locations.begin();

	for (; it != locations.end(); it++) {				//iterate through map of locations
		if(it->second.locationName == taskLocation){
			weightedGraph->shortestPath(it->second.index);
			//weightedGraph->printShortestDistance(it->second.index);

			for(auto& pair : locations){
				pair.second.distance = weightedGraph->getDist(pair.second.index);
				if(pair.second.distance !=0 && pair.second.chargerInstalled)
					locationsQueue.push(pair.second);
			}

			Location shortestDist = locationsQueue.top();
			cout << shortestDist.locationName << " in the distance of " << shortestDist.distance;

		}
	}
}

void EVCharging::lowestCost(){
	//for any given location
	cout << "Task 7: Find the cheapest charging station other than the current location (including return travel cost). " << endl << "Input a location: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, taskLocation);

	//find the CHEAPEST next location with charging
	map<int, Location>::iterator it = locations.begin();
	for(; it != locations.end(); it++){
		if(it->second.locationName == taskLocation){
			weightedGraph->shortestPath(it->second.index);
			chargeAmount = rand() % 41 + 10;


			for(auto& pair : locations){
				pair.second.distance = weightedGraph->getDist(pair.second.index);
				//insert calculation from task 5
				pair.second.cost = pair.second.distance * 2 * travelCost + chargeAmount * pair.second.chargingPrice;

				if(pair.second.distance != 0 && pair.second.chargerInstalled)
					if(pair.second.chargingPrice != 0){
						locationsQueue.push(pair.second);
					}
					if(pair.second.chargingPrice == 0 && chargeAmount <= 25){
						locationsQueue.push(pair.second);
					}
			}

			Location lowestCost = locationsQueue.top();
			cout << "Charging amount: " << chargeAmount << "kWh" << endl << "The cheapest other charging station is " << lowestCost.locationName << ".\n";
			cout << "Charging cost = " <<  chargeAmount * lowestCost.chargingPrice << endl;
			cout << "Travel cost = " <<  lowestCost.distance*2*travelCost << endl;
			cout << "Total cost = " <<  lowestCost.cost;
		}
	}
	//consider two-way travel
}

void EVCharging::originToDestination(){
	string originLocation;
	string destinationLocation;
	cout << "Task 8: Find the cheapest charging station between origin and destination. " << endl << "Input a origin location: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, originLocation);

	cout << "Input a destination location: ";
    std::getline(std::cin, destinationLocation);

	map<int, Location>::iterator it = locations.begin();

	int originIndex;
	int destinationIndex;

	for (; it != locations.end(); it++) {				//iterate through map of locations
		if(it->second.locationName == originLocation){
			originIndex = it->second.index;
		}
		if(it->second.locationName == destinationLocation){
			destinationIndex = it->second.index;
		}
	}

/*-------------------------------------------------------------------------------------------------------*/
	weightedGraph->shortestPath(originIndex);		//find the lowest cost location to travel to
	chargeAmount = rand() % 41 + 10;				//randomize charging amount between 10-50
	//chargeAmount = 17;
	for(auto& pair : locations){
		pair.second.distance = weightedGraph->getDist(pair.second.index);
		//insert calculation from task 5
		pair.second.cost = pair.second.distance * 1 * travelCost + chargeAmount * pair.second.chargingPrice;
		if(pair.second.distance != 0 && pair.second.chargerInstalled)
			locationsQueue.push(pair.second);
	}

	while(chargeAmount > 25 && locationsQueue.top().chargingPrice == 0){		//make sure that free charging stations are not used if the charge amount is greater than 25
		locationsQueue.pop();
	}
	Location lowestCost = locationsQueue.top();

/*-------------------------------------------------------------------------------------------------------*/

	vector<int> shortestPath1;
	vector<int> shortestPath2;
	vector<int> testPath;

		//check to see if any vertices in the shortestPath from the origin to destination have a free charging station (or at least compare their charging cost to lowestCost.index)
	vector<int> pathStartToFinish = weightedGraph->shortestPath(originIndex, destinationIndex);
	bool freeChargeInPath = false;
	for(int i=0; i<pathStartToFinish.size(); i++){
		if(locations[pathStartToFinish[i]].chargingPrice == 0){
			testPath = pathStartToFinish;
			lowestCost = locations[pathStartToFinish[i]];
				freeChargeInPath = true;
		}
	}
	if(!freeChargeInPath){
		shortestPath1 = weightedGraph->shortestPath(originIndex, lowestCost.index);
		shortestPath2 = weightedGraph->shortestPath(lowestCost.index, destinationIndex);

		for(int i=0; i< shortestPath1.size(); i++){
			testPath.push_back(shortestPath1[i]);
			if(shortestPath1[i] == destinationIndex)
				break;
		}
		for(int i=0; i< shortestPath2.size(); i++){
			if(shortestPath2[i] != lowestCost.index)
				testPath.push_back(shortestPath2[i]);
			if(shortestPath1[i] == destinationIndex)
				break;
		}
	}

	int size = (testPath.size())-1;
	double totalDist=0;
	for(int i = 0; i<size; i++){
		double dist = weightedGraph->getWeight(testPath[i], testPath[i+1]);
		totalDist = totalDist + dist;
	}

	cout << "Charging amount: " << chargeAmount << "kWh\n";
	cout << "The cheapest charging station is " << lowestCost.locationName << endl;
	cout << "Charging cost = " <<  chargeAmount * lowestCost.chargingPrice << endl;
	cout << "Travel cost = " <<  totalDist*1*travelCost << endl;
	cout << "Total cost = " <<  totalDist*1*travelCost + chargeAmount * lowestCost.chargingPrice << endl;
	cout << "Travel path ";

	for(int j=0; j< testPath.size(); j++){
		cout << " -> " << locations[testPath[j]].locationName;
	}
}

void EVCharging::multipleChargingLocations(){
	vector<Location> cheapestLocations;

	string originLocation;
	string destinationLocation;
	cout << "Task 9: Find the best way of charging (multiple charging). " << endl << "Input a origin location: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, originLocation);

	cout << "Input a destination location: ";
    std::getline(std::cin, destinationLocation);

	map<int, Location>::iterator it = locations.begin();

	int originIndex;
	int destinationIndex;

	for (; it != locations.end(); it++) {				//iterate through map of locations
		if(it->second.locationName == originLocation){
			originIndex = it->second.index;
		}
		if(it->second.locationName == destinationLocation){
			destinationIndex = it->second.index;
		}
	}


/*-------------------------------------------------------------------------------------------------------*/
	//shortestPath(originIndex)
	weightedGraph->shortestPath(originIndex);

	//generate random charging amount
	chargeAmount = rand() % 41 + 10;
	chargeAmount = 20;
	for(auto& pair : locations){
		pair.second.distance = weightedGraph->getDist(pair.second.index);
		pair.second.cost = pair.second.distance * 1 * travelCost + chargeAmount * pair.second.chargingPrice;
		if(pair.second.distance != 0 && pair.second.chargerInstalled)
			locationsQueue.push(pair.second);
	}

	//if charging amount is greater than 25
	if(chargeAmount > 25){
		//find the lowest cost location from source
		cheapestLocations.push_back(locationsQueue.top());

		//if that location is free charging then push it into the vector of lowest cost
		if(locationsQueue.top().chargingPrice == 0){
			//minus 25 from the charge amount and recalculate the cost for that location
			double freeCharge = 25;
			cheapestLocations.front().chargeAmountAtLocation = 25;
			cheapestLocations.front().cost = cheapestLocations.front().distance * 1 * travelCost + freeCharge * cheapestLocations.front().chargingPrice;

			//look at the next best location for charging and push it into the vector also
			locationsQueue.pop();
			cheapestLocations.push_back(locationsQueue.top());
			//recalculate the new cost based on the remaining charge amount
			cheapestLocations.back().cost = cheapestLocations.front().distance * 1 * travelCost + chargeAmount * cheapestLocations.front().chargingPrice;
			cheapestLocations.back().chargeAmountAtLocation = chargeAmount-25;
		}

	}else{
		//else push it into the vector of lowest cost
		cheapestLocations.push_back(locationsQueue.top());
		cheapestLocations.front().chargeAmountAtLocation = chargeAmount;
	}

	vector<int> testPath;
	//check to see if any vertices in the shortestPath from the origin to destination have 2 free charging stations

	vector<int> pathStartToFinish = weightedGraph->shortestPath(originIndex, destinationIndex);
	bool freeChargeInPath = false;
	int freeChargeCount = 0;
	vector<Location> cheapestLocationsInOriginalPath;

	for (int i = 0; i < pathStartToFinish.size(); i++) {
		if (locations[pathStartToFinish[i]].chargingPrice == 0) {
			freeChargeCount++;
			cheapestLocationsInOriginalPath.push_back(locations[pathStartToFinish[i]]);

			//if the path does, make the testPath this path
			if (freeChargeCount == 2) {
				testPath = pathStartToFinish;
				//remove the items in the cheapestLocations vector and push in the two charging locations of this path
				cheapestLocations = cheapestLocationsInOriginalPath;
				freeChargeInPath = true;
				break;
			}
		}
	}

	//if not,
		//create a path from source to lowestCost 1
		//create a path from lowestCost 1 to lowestCost 2
		//create a path from lowestCost2 to destination
		int x = originIndex;

		if(!freeChargeInPath){
			for(int i=0; i < cheapestLocations.size(); i++){	//for every cheap charging station
				vector<int> shortestPath = weightedGraph->shortestPath(x, cheapestLocations[i].index);
				for(int j=0; j< shortestPath.size(); j++){
					if(shortestPath[j] == cheapestLocations[i].index)
						break;

					if(shortestPath[j] == cheapestLocations.back().index)
						break;

					testPath.push_back(shortestPath[j]);

				}
				x = cheapestLocations[i].index;
			}

			vector<int>shortestPath2 = weightedGraph->shortestPath(cheapestLocations.back().index, destinationIndex);

			for(int i=0; i< shortestPath2.size(); i++){
				testPath.push_back(shortestPath2[i]);
				if(shortestPath2[i] == destinationIndex)
					break;
			}
		}

		int size = (testPath.size());
		double totalDist=0;
		double dist =0;
		for(int i = 0; i<size-1; i++){
			dist = weightedGraph->getWeight(testPath[i], testPath[i+1]);
			totalDist = totalDist + dist;
		}

	double chargingCost=0;
	cout << "Charging amount: " << chargeAmount << "kWh\n";
	cout << "Best way of charging is : \n";
		for(int i=0; i< cheapestLocations.size(); i++){
			chargingCost = chargingCost + cheapestLocations[i].chargeAmountAtLocation * cheapestLocations[i].chargingPrice;
			cout << "Charging " << cheapestLocations[i].chargeAmountAtLocation << "kWH at " << cheapestLocations[i].locationName << endl;
		}
	cout << "Charging cost = " <<  chargingCost << endl;
	cout << "Travel cost = " <<  totalDist*1*travelCost << endl;
	cout << "Total cost = " <<  totalDist*1*travelCost + chargingCost << endl;
	cout << "Travel path ";

	for(int j=0; j< testPath.size(); j++){
		cout << " -> " << locations[testPath[j]].locationName;
	}

/*-------------------------------------------------------------------------------------------------------*/
}

void EVCharging::run(){
	task = getTask();

	switch(task){
		case 1:
			printLocations();
			break;
		case 2:
			printAdjacencyMatrix();
			break;
		case 3:
			printLocationsWithCharging();
			break;
		case 4:
			printAdjacentLocations();
			break;
		case 5:
			cheapestAdjacent();
			break;
		case 6:
			shortestPath();
			break;
		case 7:
			lowestCost();
			break;
		case 8:
			originToDestination();
			break;
		case 9:
			multipleChargingLocations();
			break;
		case 10:
			cout << "test output for case 10";
			break;

	}
}

int EVCharging::getTask(){
	int t;
	do{
        cout << "=================================" << endl;
        cout << "Choose a task to run:" << endl;
        cout << "[1]     Print location information"<< endl;
        cout << "[2]     Print adjacency matrix" << endl;
        cout << "[3]     Task 3 - List all charging stations in ascending order of charging price" << endl;
        cout << "[4]     Task 4 - List adjacent charging stations"<< endl;
        cout << "[5]     Task 5 - Find the adjacent charging station with the lowest travel & charging cost"<< endl;
        cout << "[6]     Task 6 - Find the nearest charging station"<< endl;
        cout << "[7]     Task 7 - Find the cheapest charging station other than the current location"<< endl;
        cout << "[8]     Task 8 - Find the cheapest charging station between origin and destination (single charging)"<< endl;
        cout << "[9]     Task 9 - Find the best way of charging (multiple charging)"<< endl;
        cout << "[10]     Quit"<< endl;

		cin >> t;

	}while(!isValidTask(t));

	return t;
}

bool EVCharging::isValidTask(int t){
	if(t >= 0 && t <= 10)
		return true;
	else{
		cout << "Invalid input, please try again." << endl;
		return false;
	}
}

#endif /* EVCHARGING_H_ */
