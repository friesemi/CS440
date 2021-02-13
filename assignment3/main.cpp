#include <bits/stdc++.h>
#include <bitset>
#include <string>
#include <fstream>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

void writeToFile(vector<string> result)
{
	ofstream outputFile ("EmployeeIndex.txt");
	int i = 0;

	if (outputFile.is_open()){
		while(i != result.size()){
			outputFile << result.at(i) << ", ";
			outputFile << result.at(i+1) << ", ";
			outputFile << result.at(i+2) << ", ";
			outputFile << result.at(i+3) << endl;

			i += 4;
		}
		outputFile.close();
	}
}

void writeBitsBuckets(int numBits, int numBuckets) {
	ofstream outputFile ("BitsandBuckets.txt");

	if (outputFile.is_open()) {
		outputFile << numBits << endl;
		outputFile << numBuckets << endl;

		outputFile.close();
	}
}

int hashAndGetBits(int id, int numBits)
{
	// Convert to binary
	//cout << id << endl;
	string binary = bitset<32>(id).to_string();
	//cout << binary << endl;

	// Grab n least significant bits
	string keyBin = binary.substr(binary.length() - numBits);
	//cout << keyBin << endl;

	// Convert to decimal
	int key = bitset<32>(keyBin).to_ulong();
  //cout << key << endl;

	return key;
}

vector<long> updateBucketArray(vector<long> bucketArray, int numBuckets) {
	fstream myFile("EmployeeIndex.txt");
	string dummy;

	for (int i = 1; i < numBuckets; i++) {
		getline(myFile, dummy);
		bucketArray.at(i) = myFile.tellg();
	}

	return bucketArray;
}

int bitFlip(int key) {
	string binary = bitset<32>(key).to_string();
	for (int i = 0; i < binary.length(); i++) {
		if (binary[i] == '1') {
			binary[i] = '0';
			break;
		}
	}
	key = bitset<32>(binary).to_ulong();

	return key;
}

vector<long> store(int key, int numBuckets, vector<string> result, vector<long> bucketArray) {
	int bytesToBucket;
	string bucket = "", currLine;
	fstream myFile("EmployeeIndex.txt");
	long pos;

	// Check if bit flip is needed
	cout << key << endl;
	if (key+1 > numBuckets) {
		key = bitFlip(key);
	}
	cout << key << endl << endl;


	// Get position of bucket in file
	pos = bucketArray.at(key);


	if (myFile.is_open()) {
		// Read in bucket
	  myFile.seekg(pos);
		getline(myFile, bucket);

		// Append result to end of bucket
		for (int i = 0; i < 4; i++) {
			bucket.append(result.at(i));
			bucket.append(" ");
		}
		bucket.append("; ");

		cout << bucket << endl;

		// Write bucket back to where you pulled it from
		ofstream makeFile("temp.txt");
		makeFile.close();
		fstream tempFile("temp.txt");

		if (tempFile.is_open()) {
			myFile.seekg(0, ios::beg);

			// Copy old buckets over
			for (int i = 0; i < key; i++) {
				getline(myFile, currLine);
				tempFile << currLine << endl;
			}

			// Copy in changed bucket
			tempFile << bucket;
			// If not putting in last bucket, tack endl back on
			if (key + 1 != numBuckets) {
				tempFile << endl;
			}

			// Throw out old version of changed bucket
			getline(myFile, currLine);

			// Copy in rest of file
			while (getline(myFile, currLine)) {
				tempFile << currLine;
			}

			tempFile.close();
		}
		myFile.close();

		// Remove old file
		remove("EmployeeIndex.txt");

		// Rename new file
		rename("temp.txt", "EmployeeIndex.txt");

		// Update bucket array
		bucketArray = updateBucketArray(bucketArray, numBuckets);
	}

	return bucketArray;
}

vector<long> initIndex(vector<long> bucketArray) {
	// Make file
	ofstream makeFile("EmployeeIndex.txt");
	makeFile.close();

	fstream myFile("EmployeeIndex.txt");

	if (myFile.is_open()) {
		myFile.seekg(0, ios::beg);
		bucketArray.push_back(myFile.tellg());
		myFile << " ";

		myFile.close();
	}

	return bucketArray;
}

vector<long> checkForCapacity(int* numBuckets, int numRecords, int* numBits, vector<long> bucketArray) {
	fstream myFile("EmployeeIndex.txt");

	// Add a bucket if above 80% capacity
	cout << "TEST" << endl;
	if ((numRecords / ((*numBuckets)*4) ) > 0.8) {

		cout << "TEST" << endl;
		(*numBuckets)++;

		if ((*numBuckets) > (2^(*numBits))) {
			(*numBits)++;
		}

		// Allocate new space
		myFile.seekg(0, ios::end);
		myFile << endl;
		bucketArray.push_back(myFile.tellg());
		myFile << " ";

		// Move flip bits
	}
}

void getNextLineAndHash(istream& str)
{
	string cell, line;
	vector<long> bucketArray;
	int id, key, numBits = 1, numBuckets = 1, numRecords = 0;

	bucketArray = initIndex(bucketArray);

	// This iterates through the whole file line by line
	while(getline(str, line)){
		vector<string> result;
		stringstream lineStream(line);

		// Get the data for current line
		while(getline(lineStream,cell, ','))
		{
			result.push_back(cell);
		}

		// Use the id to get the key for which bucket it should be placed in
		id = stoi(result.at(0));
		key = hashAndGetBits(id, numBits);

		// Find memory address with key from bucket array (DO BUCKET ARRAY AT END)
		// getMemAddr(key, bucketArray)

		bucketArray = store(key, numBuckets, result, bucketArray);
		numRecords++;

		cout << "TEST" << endl;
		bucketArray = checkForCapacity(&numBuckets, numRecords, &numBits, bucketArray);
	}
  // Write numBits and numBuckets to a file for use with lookup
	writeBitsBuckets(numBits, numBuckets);
	//writeToFile(result);
}

// This will read from CSV file and create hash
void createHashIndex()
{
	vector<string> result;
	ifstream file ("test.csv");
	if (file.is_open()){
		getNextLineAndHash(file);

		file.close();
	}
}

void getHashIndex(string lookupId)
{
	// Hash id to get array index

	// This reads the output file and finds the info from the id
	ifstream readFile ("EmployeeIndex.txt");
	string data;

	while(!readFile.eof()){
		getline(readFile, data);
		if(data.rfind(lookupId, 8) != string::npos)
			cout << data << endl;
	}
}

//Driver method to test map class
int main()
{
	//User input for Creation or Lookup
	while(1){
		string input, split;
		cout << "Enter a command (C or L) with the id if necessary: ";
		getline(cin, input);
		stringstream splitter(input);
		while (getline(splitter, split, ' ')){
			if (split == "C"){
				cout << "Creating Hash Index" << endl;
				createHashIndex();
				break;
				}
			else if (split == "L"){
				getline(splitter, split, ' ');
				cout << "Looking up " << split << endl;
				getHashIndex(split);
			}
			else {
				cout << "Incorrect Inputs" << endl;
			}
		}
	}
	return 0;
}
