#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<climits>
using namespace std;

//split a string into vector of strings
void splitString(string s, vector<string>& arr) {
	while (s.find(',') != s.npos) {
		arr.push_back(s.substr(0,s.find(',')));
		s = s.substr(s.find(',')+1, s.length() - s.find(','));
	}
	arr.push_back(s);
}

//transfer a vector of string into int matrix
void getArray(vector<string>& arr, vector<vector<int>> &arrInt) {
	for (auto s : arr) {
		vector<int> integers;
		for (auto c : s) {
			if (isdigit(c)) {
				integers.push_back(c-'0');
			}
			else if (isupper(c)) {
				integers.push_back(c - 'A');
			}
			else if (islower(c)) {
				integers.push_back(c - 'a'+26);
			}
		}
		arrInt.push_back(integers);
	}
}

bool allIncluded(vector<int> included) {
	for (auto n : included) {
		if (n == 0) {
			return false;
		}
	}
	return true;
}

void printVector(vector<vector<int>>list) {
	for (auto row : list) {
		for (auto element : row) {
			cout << " " << element;
		}
		cout << endl;
	}
	cout << endl;
}

void createVector(vector<vector<int>> &cost, int n) {
	for (int i = 0; i < n; i++) {
		vector<int> row(n);
		cost.push_back(row);
	}
}

void createCost(vector<vector<int>>& cost, vector<vector<int>> country, vector<vector<int>>& build, vector<vector<int>>& destroy) {
	for (unsigned int i = 0; i < country.size(); i++) {
		for (unsigned int j = 0; j < country.size(); j++) {
			if (country[i][j] == 0) {
				cost[i][j] = build[i][j];
			}
			else {
				cost[i][j] = 0 - destroy[i][j];
			}
		}
	}

}

int calculateCost(vector<vector<int>> country, vector<vector<int>> MST, vector<vector<int>> build, vector<vector<int>> destroy) {
	int sum = 0;
	for (unsigned int i = 0; i < country.size(); i++) {
		for (unsigned int j = i+1; j < country.size(); j++) {
			if (country[i][j] == 0 && MST[i][j] == 1) {
				sum += build[i][j];
			}
			else if (country[i][j] == 1 && MST[i][j] == 0) {
				sum += destroy[i][j];
			}
		}
	}
	return sum;
}


int main() {
	//read the first line of the input
	string input;
	getline(cin, input);

	//split the input by " " and store the result in three strings
	stringstream ss(input);
	string array1, array2, array3;
	ss >> array1 >> array2 >> array3;

	//split the strings by "," and store the result in vectors
	vector<string> cities;
	vector<string> roadsBuild;
	vector<string> roadsDes;
	splitString(array1, cities);
	splitString(array2, roadsBuild);
	splitString(array3, roadsDes);

	//transfer the string into vector<int>
	vector<vector<int>> country;
	vector<vector<int>> build;
	vector<vector<int>> destroy;
	getArray(cities, country);
	getArray(roadsBuild, build);
	getArray(roadsDes, destroy); 
	/*
	cout << "country" << endl;
	printVector(country);
	cout << "build" << endl;
	printVector(build);
	cout << "destroy" << endl;
	printVector(destroy);
	*/

	vector<vector<int>> cost;
	createVector(cost, country.size());
	createCost(cost, country, build, destroy);

	//cout << "cost table before floyd" << endl;
	//printVector(cost);

	//floyd algorith to get the lowest cost of each pair of node
	//for (unsigned int i = 0; i < country.size(); i++) {
	//	for (unsigned int j = 0; j < country.size(); j++) {
	//		for (unsigned int k = 0; k < country.size(); k++) {
	//			if (cost[i][k] + cost[k][j]  < cost[i][j]&&k!=i&&k!=j&&i!=j) {
	//				cost[i][j]=cost[i][k] + cost[k][j];
	//			}
	//		}
	//	}
	//}
	//cout << "cost table" << endl;
	//printVector(cost);
	// prim algorith to get the minimum spanning tree
	vector<int>included(cities.size());
	vector<vector<int>> MST;
	createVector(MST, cities.size());

	while (!allIncluded(included)) {
		//search to find the minimal cost entry whose nodes are not all included
		int minimalCost = INT_MAX;
		int foundI=0;
		int foundJ=0;
		for (unsigned int i = 0; i < country.size(); i++) {
			for (unsigned int j = 0; j < country.size(); j++) {
				if (i != j) {
					if ((included[i] == 0 && included[j]==1) ||( included[i] == 1&&included[j] == 0)) {
						if (cost[i][j] < minimalCost) {
							minimalCost = cost[i][j];
							foundI = i;
							foundJ = j;
						}
					}
				}
			}
		}
		included[foundI] = 1;
		included[foundJ] = 1;
		MST[foundI][foundJ] = 1;

		MST[foundJ][foundI] = 1;
		//add the two nodes as included and add the edge as part the minimal spanning tree(add into the total cost)
	}

	//cout << "MST table" << endl;
	//printVector(MST);

	int totalCost = calculateCost(country, MST, build, destroy);
	cout << totalCost << endl;
	// get the final cost
}