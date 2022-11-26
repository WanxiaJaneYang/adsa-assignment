#include<iostream>
#include<string>
#include<sstream>
#include<vector>

using namespace std;
class HashTable {
public:
	string slots[26];
	int status[26] = { 0 };//0 for never-used, -1 for tombstone and 1 for occupied

public:
	void print();
	void insertKey(string key);
	int* findKey(string key, int* found);//return an array consists of two elements:first indicate if there is an element, the second indicates the index or next available index
	void deleteKey(string key);
};

int main() {
	//receive a line from input
	string input;
	getline(cin, input);

	//split the input by " " and store the result in a vector 
	stringstream ss(input);
	vector<string> instructions;
	string instruction;
	while (ss >> instruction) {
		instructions.push_back(instruction);
	}

	HashTable hashTable;

	//iterate through the vector and execute the corresponding instructions
	string word;
	for (auto ins : instructions) {
		word = ins.substr(1, ins.length() - 1);
		if (ins[0] == 'A') {
			hashTable.insertKey(word);
		}
		else if (ins[0] == 'D') {
			hashTable.deleteKey(word);
		}
	}

	hashTable.print();
}

void HashTable::print()
{
	for (int i = 0; i < 26; i++) {
		//only print if the slot is occupied
		if (status[i] > 0) {
			cout << slots[i] << " ";
		}
	}
}

void HashTable::insertKey(string key)
{
	int* result=new int[2];
	result= findKey(key, result);
	if (*result == -1) {
		int index = *(result + 1);
		slots[index] = key;
		status[index] = 1;
	}
}

int* HashTable::findKey(string key, int* found)
{
	int index = key[key.length() - 1] - 'a';

	if (status[index] <= 0) {
		found[0] = -1;
		found[1] = index;
		return found;
	}
	else if (slots[index] == key) {
		found[0] = 1;
		found[1] = index;
		return found;
	}
	else {
		int i = index + 1;
		while (i != index) {
			if (i > 25) {
				i = i % 26;
			}
			found[1] = i;
			if (status[i] <= 0) {
				found[0] = -1;
				return found;
			}
			else if (slots[i] == key) {
				found[0] = 1;
				return found;
			}
			i++;
		}
		return nullptr;
	}
}

void HashTable::deleteKey(string key)
{
	int* result = new int[2];
	result= findKey(key, result);
	if (*result == 1) {
		int index = *(result + 1);
		slots[index] = "";
		status[index] = -1;
	}
}