
#ifndef LOCATION_H_
#define LOCATION_H_

class Location {
public:
	int index;//Key
	string locationName;
	bool chargerInstalled;
	double chargingPrice;
	double distance;
	double cost;
	double chargeAmountAtLocation;

	void printLocation() {
		cout << setw(6) << index << setw(20) << locationName << setw (15);
		if(chargerInstalled)
			cout << "yes";
		else
			cout << "no";

		if(chargerInstalled && chargingPrice == 0)
			cout << setw(28) << "free of charge" << endl;
		else if (!chargerInstalled )
			cout << setw(17) << "N/A" << endl;
		else
		   cout << setw(15) << "$" << setprecision(2) << chargingPrice << "/kWh" << endl;
	}

	bool operator<(const Location& l)  const {
		if(cost != l.cost){					//task 8 & 9
			return cost > l.cost;
		}
		if(distance != l.distance){
			return distance > l.distance;	//task 6
		}
		if(chargingPrice != l.chargingPrice){
			return chargingPrice > l.chargingPrice;	//task 3
		}
		return true;
	}
};



#endif /* LOCATION_H_ */
