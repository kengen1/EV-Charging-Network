
#ifndef H_weightedGraph
#define H_weightedGraph

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cfloat>
#include <functional>

using namespace std;

class WeightedGraphType {
protected:
	int gSize;      //number of vertices
	list<int> *graph; // Store adjacency list
	double **weights; // Store weights of edges
	double *smallestWeight;
	vector<int> path;
public:
	WeightedGraphType(int size = 0);
	~WeightedGraphType();

	list<int> getAdjancencyList(int index) {
		return graph[index];
	}

	double getWeight(int i, int j) {
		return weights[i][j];
	}

	void printAdjacencyList();
	void printAdjacencyMatrix();
	void shortestPath(int vertex);
	void printShortestDistance(int vertex);
	double getDist(int index) const;
	vector<int> getPath();
	vector<int> shortestPath(int vertex, int destination);
};

WeightedGraphType::WeightedGraphType(int size) {
	gSize = 0;
	ifstream infile;
	char fileName[50] = "Weights.txt";

//	cout << "Enter graph adjacency matrix file name: ";
//	cin >> fileName;
//	cout << endl;

	infile.open(fileName);

	if (!infile) {
		cout << "Cannot open input file." << endl;
		return;
	}

	gSize = size;

	graph = new list<int> [gSize];

	weights = new double*[gSize];

	for (int i = 0; i < gSize; i++)
		weights[i] = new double[gSize];

	for (int i = 0; i < gSize; i++) {
		for (int j = 0; j < gSize; j++) {
			double value;
			infile >> value;
			if (value == 0)
				weights[i][j] = DBL_MAX; //system constant - maximum value of double
			else {
				weights[i][j] = value;
				graph[i].push_back(j);
			}
		}
	}
	smallestWeight = new double[size];

	infile.close();
}

WeightedGraphType::~WeightedGraphType() {
	for (int i = 0; i < gSize; i++)
		delete[] weights[i];

	delete[] weights;

	for (int index = 0; index < gSize; index++)
		graph[index].clear();

	delete[] graph;
}

void WeightedGraphType::printAdjacencyMatrix() { //print adjacency matrix for debug purpose
	cout<<"\nAdjacency Matrix" << endl;
	for (int i = 0; i < gSize; i++) {
		for (int j = 0; j < gSize; j++) {
			cout << setw(10) << (weights[i][j] == DBL_MAX ? 0.0 : weights[i][j]); //as adjacency value, zero means no direct connection
		}
		cout << endl;
	}
}

void WeightedGraphType::printAdjacencyList() { //print adjacency list for debug purpose
	cout<<"\nAdjacency List" << endl;
	for (int index = 0; index < gSize; index++) {
		cout << index << ": ";
		for (int e : graph[index])
			cout << e << " ";
		cout << endl;
	}

	cout << endl;
}

void WeightedGraphType::shortestPath(int vertex){

    for (int j = 0; j < gSize; j++)
        smallestWeight[j] = weights[vertex][j];

    bool *weightFound;
    weightFound = new bool[gSize];

    for (int j = 0; j < gSize; j++)
        weightFound[j] = false;

    weightFound[vertex] = true;
    smallestWeight[vertex] = 0;

    for (int i = 0; i < gSize - 1; i++)
    {
        double minWeight = DBL_MAX;
        int v;

        for (int j = 0; j < gSize; j++)
            if (!weightFound[j])
                if (smallestWeight[j] < minWeight)
                {
                    v = j;
                    minWeight = smallestWeight[v];
                }

        weightFound[v] = true;

        for (int j = 0; j < gSize; j++)
            if (!weightFound[j])
                if (minWeight + weights[v][j] < smallestWeight[j]){
                    smallestWeight[j] = minWeight + weights[v][j];
				}
    } //end for
}

vector<int> WeightedGraphType::shortestPath(int vertex, int destination) {
    // Initialize smallestWeight array with weights from the specified vertex to all other vertices
    for (int j = 0; j < gSize; j++)
        smallestWeight[j] = weights[vertex][j];

    // Create a boolean array to keep track of visited vertices
    bool *weightFound;
    weightFound = new bool[gSize];

    // Initialize weightFound array to false for all vertices
    for (int j = 0; j < gSize; j++)
        weightFound[j] = false;

    // Mark the specified vertex as visited and set its smallest weight to 0
    weightFound[vertex] = true;
    smallestWeight[vertex] = 0;

    // Create an array to store the shortest path
    int *path;
    path = new int[gSize];

    // Initialize the path array
    for (int j = 0; j < gSize; j++)
        path[j] = -1; // -1 indicates that a node is not part of the shortest path

    // Find the shortest path for all vertices
    for (int i = 0; i < gSize - 1; i++) {
        // Initialize the minimum weight as maximum value
        double minWeight = DBL_MAX;
        int v;

        // Find the vertex with the smallest weight that has not been visited
        for (int j = 0; j < gSize; j++)
            if (!weightFound[j])
                if (smallestWeight[j] < minWeight) {
                    v = j;
                    minWeight = smallestWeight[v];
                }

        // Mark the vertex as visited
        weightFound[v] = true;

        // Update the smallest weights of adjacent vertices if a shorter path is found
        for (int j = 0; j < gSize; j++)
            if (!weightFound[j])
                if (minWeight + weights[v][j] < smallestWeight[j]) {
                    smallestWeight[j] = minWeight + weights[v][j];
                    path[j] = v; // Update the path array with the previous node for the current vertex
                }
    } // end for

    // Store the shortest path from source to destination
    std::vector<int> shortestPath;
    int currentNode = destination;
    while (currentNode != -1) {
        shortestPath.push_back(currentNode);
        currentNode = path[currentNode];
    }
	shortestPath.push_back(vertex);

    // Reverse the path to get the correct order
    std::reverse(shortestPath.begin(), shortestPath.end());

    // Clean up dynamic memory
    delete[] weightFound;
    delete[] path;

    return shortestPath;
}

void WeightedGraphType::printShortestDistance(int vertex)
{
    cout << "Source Vertex: " << vertex << endl;
    cout << "Shortest distance from source to each vertex."
         << endl;
    cout << "Vertex  Shortest_Distance" << endl;

    for (int j = 0; j < gSize; j++)
        cout << setw(4) << j << setw(12) << smallestWeight[j]
             << endl;
    cout << endl;
} //end printShortestDistance

double WeightedGraphType::getDist(int index) const {
	return smallestWeight[index];
}

vector<int> WeightedGraphType::getPath(){
	return path;
}
#endif